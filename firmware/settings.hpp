#ifndef __INC_SETTINGS_HPP
#define __INC_SETTINGS_HPP

#include <TFT_eSPI.h>

#define THRESHOLD_DEFAULT   (-80)
#define BACKLIGHT_DEFAULT   (128)

#define BTN_LEFT   1
#define BTN_RIGHT  2

typedef enum {
  NONE = -1,
  MAIN,
  BACKLIGHT,
  THRESHOLD,
  QRCODE,
  EXIT
} MenuItem;

class Settings
{
  public:
    
    Settings();
    Settings(int backlight, int threshold);
    void draw(TFT_eSprite &screen);

    void onButtonPressed(int button);

    bool isMenuVisible(void);
    int getBacklightLevel(void);
    int getThreshold(void);

  private:

    void handleMainMenu(int button);
    void handleBacklightMenu(int button);
    void handleThresholdMenu(int button);
    void handleQrcodeMenu(int button);

    void drawMainMenu(TFT_eSprite &screen);
    void drawBacklightMenu(TFT_eSprite &screen);
    void drawThresholdMenu(TFT_eSprite &screen);
    void drawQrcodeMenu(TFT_eSprite &screen);

    void drawMenuItem(TFT_eSprite &screen, char *pItem, int x, int y, bool inverted);

    /* Helpers */
    void cursorToBacklight(void);
    void backlightToCursor(void);
    void cursorToThreshold(void);
    void thresholdToCursor(void);

    MenuItem mCurrentItem;
    int mCursor;
    int mBacklight;
    int mThreshold;
};

#endif /* __INC_SETTINGS_HPP */