
#include <SPI.h>
#include <TFT_eSPI.h>
#include "face.hpp"
#include "animator.hpp"
#include "scanner.hpp"

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

uint32_t mark = 0;
uint32_t frames = 0;
int eyes_y = 15, dir=1;
char fps_str[64];

// Paramètre du channel 0 du PWM
const int freq = 5000; // 5000 Hz
const int ledChannel = 0;
const int resolution = 8; // Résolution de 8 bits

volatile int button_pressed = 0;

char device_report[24];

TFT_eSPI tft = TFT_eSPI();
PalFace face = PalFace();
Animator animator(face);

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

void setup() {

  // Enable backlight full (no pwm)
  // Configure le channel 0
  pinMode(9, OUTPUT);
  analogWrite(9, 128);

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

  scanner_start();
}

void loop() {
  // put your main code here, to run repeatedly:
  frames++;
  if ((millis() - mark) > 1) {
    animator.animate();
    //if (animator.animate()) {
      face.draw(buffer);
      snprintf(device_report, 24, "devs: %d", scanner_get_count()); 
      buffer.drawString(device_report, 80, 80);
      buffer.pushSprite(0, 0); 
    //}
    mark = millis();
  }

  if (button_pressed == 2)
  {
    button_pressed = 0;
    FaceState *start_state = new FaceState(
      animator.getCurrentState().getEyesX(),
      animator.getCurrentState().getEyesY(),
      animator.getCurrentState().getMouthX(),
      animator.getCurrentState().getMouthY()
    );
    FaceState *end_state = new FaceState(160, 120, 160, MOUTH_Y);
    Transition *t = new Transition(*start_state, *end_state, 5);
    animator.setTransition(t);
  } 
  else if (button_pressed == 1)
  {
    button_pressed = 0;
    FaceState *start_state = new FaceState(
      animator.getCurrentState().getEyesX(),
      animator.getCurrentState().getEyesY(),
      animator.getCurrentState().getMouthX(),
      animator.getCurrentState().getMouthY()
    );
    FaceState *end_state = new FaceState(80, 120, 80, MOUTH_Y);
    Transition *t = new Transition(*start_state, *end_state, 5);
    animator.setTransition(t);
  }

 
  #if 0
  /* Draw eyes */
  tft.fillSmoothRoundRect(REYE_X, EYE_Y, EYE_SIZE, EYE_SIZE+40, 10, TFT_WHITE, TFT_SKYBLUE);
  tft.fillSmoothRoundRect(LEYE_X, EYE_Y, EYE_SIZE, EYE_SIZE+40, 10, TFT_WHITE, TFT_SKYBLUE);

  /* Draw eye brows */
  tft.drawSmoothArc(REYE_X + EYE_SIZE/2, EYE_Y+40, 85, 80, 140, 155, TFT_WHITE, TFT_SKYBLUE, true);
  tft.drawSmoothArc(LEYE_X + EYE_SIZE/2, EYE_Y+40, 85, 80, 205, 220, TFT_WHITE, TFT_SKYBLUE, true);

  /* And mouth. */
  tft.drawWideLine(LEYE_X - 20, EYE_Y+EYE_SIZE+80, LEYE_X-15, EYE_Y+EYE_SIZE+85, 10, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(LEYE_X-15, EYE_Y+EYE_SIZE+85, REYE_X+EYE_SIZE+15, EYE_Y+EYE_SIZE+85, 10, TFT_WHITE, TFT_WHITE);
  tft.drawWideLine(REYE_X+EYE_SIZE+15, EYE_Y+EYE_SIZE+85, REYE_X+EYE_SIZE+20, EYE_Y+EYE_SIZE+80, 10, TFT_WHITE, TFT_WHITE);

  #endif
}
