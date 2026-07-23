
#include <SPI.h>
#include <TFT_eSPI.h>
#include "face.hpp"
#include "animator.hpp"
#include "scanner.hpp"
#include "settings.hpp"

#define TFT_MOSI 19
#define TFT_SCLK 18
#define TFT_RST 20 // The ESP32 pin GPIO27 connected to the RST pin of the circular TFT display
#define TFT_DC 14 // The ESP32 pin GPIO25 connected to the DC pin of the circular TFT display
#define TFT_CS 15 

// Defines default eyes position
#define EYE_SPACING   20
#define EYE_SIZE      15
#define LEYE_X        ((240 - 2*EYE_SIZE - EYE_SPACING)/2)
#define REYE_X        (LEYE_X + EYE_SIZE + EYE_SPACING)
#define EYE_Y         (60)

#define SCREEN_FPS    24
#define REFRESH_DELAY 17

#define SW_LEFT       7
#define SW_RIGHT      6         

#define MOOD_COOLDOWN 400

uint32_t mark = 0;
uint32_t frames = 0;
int eyes_y = 15, dir=1;
char fps_str[64];

// Paramètre du channel 0 du PWM
const int freq = 5000; // 5000 Hz
const int ledChannel = 0;
const int resolution = 8; // Résolution de 8 bits

volatile int button_pressed = 0;
volatile int idle_delay = 0;
volatile bool happy = true;
volatile int mood_cooldown = MOOD_COOLDOWN;

// Create an handle for the queue
QueueHandle_t moodQueue = NULL;

char device_report[24];

TFT_eSPI tft = TFT_eSPI();
PalFace face = PalFace(TFT_SKYBLUE);
Animator animator(face);
Settings settings = Settings();

TFT_eSprite buffer = TFT_eSprite(&tft);

// Interrupt Service Routine (ISR)
void ARDUINO_ISR_ATTR button_handler() {
  if (digitalRead(SW_LEFT) == 0) {
    button_pressed = 1;
  } else if (digitalRead(SW_RIGHT) == 0) {
    button_pressed = 2;
  } else {
    button_pressed = 0;
  }
}

void update_idle_delay() {
  if (happy) {
    idle_delay = 30 + (esp_random() % 50);
  } else {
    idle_delay = 5 + (esp_random() % 15);
  }
}

void make_angry(void) {
  uint8_t mood = 0;
  xQueueSend(moodQueue, &mood, portMAX_DELAY);

}

void setup() {

  /* Configure backlight. */
  pinMode(9, OUTPUT);
  analogWrite(9, settings.getBacklightLevel());

  /* Buttons. */
  pinMode(SW_LEFT, INPUT_PULLUP);
  pinMode(SW_RIGHT, INPUT_PULLUP);
  attachInterrupt(SW_LEFT, button_handler, FALLING);
  attachInterrupt(SW_RIGHT, button_handler, FALLING);
  
  // put your setup code here, to run once:
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(BG_COLOR);
  
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.setTextSize(2);

  mark = millis();
  frames = 0;

  buffer.createSprite(240, 240);
  face.draw(buffer);
  buffer.pushSprite(0, 0);  

  /* Initialize our mood queue. */
  // Create queue (5 items, each uint16_t)
  moodQueue = xQueueCreate(10, 1);

  scanner_start();

  update_idle_delay();
}

void loop() {
  uint8_t moodNotif = 1;

  // put your main code here, to run repeatedly:
  frames++;
  if ((millis() - mark) > 1) {
    
    /* Decrement idle delay. */
    if (idle_delay > 0)
    {
      idle_delay--;
    }


    /* Handle mood cooldown. */
    if (!happy)
    {
      if (mood_cooldown > 0)
      {
        mood_cooldown--;
      }
      else
      {
        uint8_t mood = 1;
        xQueueSend(moodQueue, &mood, portMAX_DELAY);
      }
    }

    /* display menu OR animated face. */
    if (settings.isMenuVisible())
    {
      settings.draw(buffer);
    }
    else
    {
      animator.animate();
      face.draw(buffer);
    }

    /* Update screen. */
    buffer.pushSprite(0, 0); 

    mark = millis();
  }


  /* Process notifications from queue. */
  if (!animator.hasTransition())
  {
    if (xQueueReceive(moodQueue, &moodNotif, 0) == pdTRUE)
    {
      if (moodNotif == 0) {
        mood_cooldown = MOOD_COOLDOWN;
        happy = false;
        FaceState *start_state = new FaceState(
          animator.getCurrentState().getEyesX(),
          animator.getCurrentState().getEyesY(),
          animator.getCurrentState().getEyesH(),
          animator.getCurrentState().getMouthX(),
          animator.getCurrentState().getMouthY(),
          animator.getCurrentState().getMouthZ()
        );
        FaceState *end_state = new FaceState(120, 120, 25, 120, MOUTH_Y-20, -10);
        Transition *t = new Transition(*start_state, *end_state, 2);
        animator.setTransition(t);
        face.setBgColor(TFT_RED);
        animator.animate();
      } else {
        happy = true;
        FaceState *start_state = new FaceState(
          animator.getCurrentState().getEyesX(),
          animator.getCurrentState().getEyesY(),
          animator.getCurrentState().getEyesH(),
          animator.getCurrentState().getMouthX(),
          animator.getCurrentState().getMouthY(),
          animator.getCurrentState().getMouthZ()
        );
        FaceState *end_state = new FaceState(160, 120, EYE_H, 160, MOUTH_Y, MOUTH_Z);
        Transition *t = new Transition(*start_state, *end_state, 5);
        animator.setTransition(t);
        face.setBgColor(TFT_SKYBLUE);
        animator.animate();
      }
    }
    else if (idle_delay == 0)
    {
      /* Pick a random position for our face. */
      uint32_t x = 80 + (esp_random() % 80);
      uint32_t y = 120 + (esp_random() % 20);
      
      FaceState *start_state = new FaceState(
        animator.getCurrentState().getEyesX(),
        animator.getCurrentState().getEyesY(),
        animator.getCurrentState().getEyesH(),
        animator.getCurrentState().getMouthX(),
        animator.getCurrentState().getMouthY(),
        animator.getCurrentState().getMouthZ()
      );
      FaceState *end_state = new FaceState(x, y, animator.getCurrentState().getEyesH(), x, 40+y, animator.getCurrentState().getMouthZ());
      if (happy) {
        Transition *t = new Transition(*start_state, *end_state, 5 + (esp_random() % 10));
        animator.setTransition(t);
      } else {
        Transition *t = new Transition(*start_state, *end_state, 3 + (esp_random() % 6));
        animator.setTransition(t);
      }
    
      /* Select a new value for idle_delay. */
      update_idle_delay();
    }
  }

  if (button_pressed != 0)
  {
    delay(150);
    settings.onButtonPressed(button_pressed);
    button_pressed = 0; /* Re-arm our button IRQ. */
  }
}
