#include "settings.hpp"
#include "qrcode.hpp"

/* Function exposed in scanner.cpp. */
extern void scanner_set_threshold(int threshold);

/**
 * Constructors
 **/

Settings::Settings() : Settings(BACKLIGHT_DEFAULT, THRESHOLD_DEFAULT)
{
}

Settings::Settings(int backlight, int threshold) : mBacklight(backlight), mThreshold(threshold), mCurrentItem(MenuItem::NONE),
  mCursor(0)
{
}

/**
 * Getters
 **/

int Settings::getBacklightLevel(void)
{
  return mBacklight;
}

int Settings::getThreshold(void)
{
  return mThreshold;
}

bool Settings::isMenuVisible(void)
{
  return (mCurrentItem != MenuItem::NONE);
}

/**
 * Setters 
 **/
void Settings::backlightToCursor(void)
{
  switch (mBacklight)
  {
    case 128:
      mCursor = 3;
      break;

    case 64:
      mCursor = 2;
      break;

    case 32:
      mCursor = 1;
      break;

    case 16:
    default:
      mCursor = 0;
      break;
  }
}

void Settings::cursorToBacklight(void)
{
  switch (mCursor)
  {
    case 0:
      mBacklight = 16;
      break;

    case 1:
      mBacklight = 32;
      break;

    case 2:
      mBacklight = 64;
      break;

    default:
    case 3:
      mBacklight = 128;
      break;
  }
}

void Settings::thresholdToCursor(void)
{
  switch (mThreshold)
  {
    case -100:
      mCursor = 3;
      break;

    case -80:
      mCursor = 2;
      break;

    case -60:
      mCursor = 1;
      break;

    case -40:
    default:
      mCursor = 0;
      break;
  }
}

void Settings::cursorToThreshold(void)
{
  switch (mCursor)
  {
    case 0:
      mThreshold = -40;
      break;

    case 1:
      mThreshold = -60;
      break;

    case 2:
      mThreshold = -80;
      break;

    default:
    case 3:
      mThreshold = -100;
      break;
  }
}

/**
 * Event handler (buttons)
 **/
void Settings::onButtonPressed(int button)
{
  switch (mCurrentItem)
  {
    case MenuItem::NONE:
      {
        if (button == BTN_LEFT)
        {
          mCurrentItem = MenuItem::MAIN;
          mCursor = 0;
        }
      }
      break;

    case MenuItem::MAIN:
      {
        handleMainMenu(button);
      }
      break;

    case MenuItem::BACKLIGHT:
      {
        handleBacklightMenu(button);
      }
      break;

    case MenuItem::THRESHOLD:
      {
        handleThresholdMenu(button);
      }
      break;

    case MenuItem::QRCODE:
      {
        handleQrcodeMenu(button);
      }
      break;

    default:
      mCurrentItem = MenuItem::NONE;
      break;
  }
}

void Settings::handleMainMenu(int button)
{
  switch (button)
  {
    case BTN_RIGHT:
      {
        /* Select the current sub-menu, or exit if requested. */
        switch (mCursor)
        {
          /* Backlight. */
          case 0:
            mCurrentItem = MenuItem::BACKLIGHT;
            backlightToCursor();
            break;

          /* Detection. */
          case 1:
            mCurrentItem = MenuItem::THRESHOLD;
            thresholdToCursor();
            break;

          /* QR Code */
          case 2:
            mCurrentItem = MenuItem::QRCODE;
            break;

          /* Exit */
          case 3:
            mCurrentItem = MenuItem::NONE;
            break;

          default:
            break;

        }
      }
      break;

    case BTN_LEFT:
      {
        mCursor = (mCursor + 1) % 4;
      }
      break;

    default:
      break;
  }
}

void Settings::handleBacklightMenu(int button)
{
  switch (button)
  {
    case BTN_LEFT:
      {
        /* Return to main menu with backlight selected. */
        mCurrentItem = MenuItem::MAIN;
        mCursor = 0;
      }
      break;

    case BTN_RIGHT:
      {
        mCursor = (mCursor + 1) % 4;
        cursorToBacklight();
        analogWrite(9, mBacklight);
      }
      break;
  }
}

void Settings::handleThresholdMenu(int button)
{
  switch (button)
  {
    case BTN_LEFT:
      {
        /* Return to main menu with backlight selected. */
        mCurrentItem = MenuItem::MAIN;
        mCursor = 1;
      }
      break;

    case BTN_RIGHT:
      {
        mCursor = (mCursor + 1) % 4;
        cursorToThreshold();
        scanner_set_threshold(mThreshold);
      }
      break;
  }
}

void Settings::handleQrcodeMenu(int button)
{
  /* Any button press leads to main menu. */
  mCurrentItem = MenuItem::MAIN;
}

/**
 * Render
 **/

void Settings::draw(TFT_eSprite &screen)
{
  switch (mCurrentItem)
  {
    case MenuItem::BACKLIGHT:
      {
        /* Clear screen.*/
        screen.fillScreen(TFT_BLACK);

        /* Draw submenu. */
        drawBacklightMenu(screen);
      }
      break;

    case MenuItem::THRESHOLD:
      {
        /* Clear screen.*/
        screen.fillScreen(TFT_BLACK);

        /* Draw submenu. */
        drawThresholdMenu(screen);
      }
      break;

    case MenuItem::QRCODE:
      {
        /* Clear screen.*/
        screen.fillScreen(TFT_BLACK);

        /* Draw submenu. */
        drawQrcodeMenu(screen);
      }
      break;
    
    case MenuItem::MAIN:
      {
        /* Clear screen.*/
        screen.fillScreen(TFT_BLACK);

        /* Draw main menu. */
        drawMainMenu(screen);
      }
      break;

    default:
      break;
  }
}

void Settings::drawMenuItem(TFT_eSprite &screen, char *pItem, int x, int y, bool inverted)
{
  /* Draw background. */
  screen.fillSmoothRoundRect(40, y-14, 160, 28, 5, inverted?TFT_WHITE:TFT_BLACK, TFT_BLACK);

  /* Draw text. */
  screen.setTextColor(inverted?TFT_BLACK:TFT_WHITE);
  screen.setTextDatum(MC_DATUM);
  screen.drawString(pItem, x, y);
}

void Settings::drawMainMenu(TFT_eSprite &screen)
{
  screen.setTextDatum(MC_DATUM);
  screen.setTextSize(2);
  
  /* Title */
  screen.setTextColor(TFT_SKYBLUE);
  screen.drawString("Settings", 120, 30);

  /* Show menu items. */
  screen.setTextColor(TFT_WHITE);
  drawMenuItem(screen, "Backlight", 120, 70, mCursor==0);
  drawMenuItem(screen, "Detection", 120, 110, mCursor==1);
  drawMenuItem(screen, "About", 120, 150, mCursor==2);
  drawMenuItem(screen, "Exit", 120, 190, mCursor==3);
}

void Settings::drawBacklightMenu(TFT_eSprite &screen)
{
  screen.setTextDatum(MC_DATUM);
  screen.setTextSize(2);
  
  /* Title */
  screen.setTextColor(TFT_SKYBLUE);
  screen.drawString("< Backlight", 120, 30);

  /* Draw a selector.  */
  screen.fillSmoothRoundRect(35 + ((155 * mCursor)/3), 110, 20, 20, 2, TFT_WHITE, TFT_BLACK);
  screen.fillSmoothRoundRect(40, 117, 160, 6, 2, TFT_WHITE, TFT_BLACK);
}

void Settings::drawThresholdMenu(TFT_eSprite &screen)
{
  screen.setTextDatum(MC_DATUM);
  screen.setTextSize(2);
  
  /* Title */
  screen.setTextColor(TFT_SKYBLUE);
  screen.drawString("< Detection", 120, 30);
  screen.setTextSize(1);
  screen.setTextColor(TFT_WHITE);
  screen.drawString("-   Sensitivity   +", 120, 150);

  /* Draw a selector.  */
  screen.drawWedgeLine(40, 120, 200, 120, 1, 6, TFT_WHITE, TFT_BLACK);
  screen.fillRoundRect(35 + ((155 * mCursor)/3), 105, 20, 30, 2, TFT_WHITE);
}

void Settings::drawQrcodeMenu(TFT_eSprite &screen)
{
  screen.setTextDatum(MC_DATUM);
  screen.setTextSize(2);
  
  /* Title */
  screen.setTextColor(TFT_SKYBLUE);
  screen.drawString("< About", 120, 30);

  /* Draw QRCODE. */
  screen.fillRect((240-127)/2 - 4, 56, 127+8, 127+8, TFT_WHITE);
  screen.drawXBitmap((240-127)/2, 60, badge_qrcode, QRCODE_WIDTH, QRCODE_HEIGHT, TFT_BLACK);
}
