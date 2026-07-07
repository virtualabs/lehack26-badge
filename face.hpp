#ifndef __INC_FACE_HPP
#define __INC_FACE_HPP

#include <stdint.h>
#include <TFT_eSPI.h>

#define EYE_SPACING         20
#define EYE_SIZE            15
#define EYE_LEFT_DEF_X      ((240 - 2*EYE_SIZE - EYE_SPACING)/2)
#define EYE_RIGHT_DEF_X     (EYE_LEFT_DEF_X + EYE_SIZE + EYE_SPACING)
#define EYE_Y               (60)
#define EYE_RIGHT_DEF_Y     EYE_Y
#define EYE_LEFT_DEF_Y      EYE_Y
#define EYE_W               (15)
#define EYE_H               (60)
#define MOUTH_X             (120)
#define MOUTH_Y             (170)
#define MOUTH_W             (60)
#define MOUTH_Z             (10)
#define BG_COLOR            TFT_SKYBLUE

typedef enum {
  RIGHT,
  LEFT
} EyeSide;

class Eye
{

  public:

    /* Constructor. */
    Eye(EyeSide side, int x, int y, uint16_t bgColor);
    Eye(EyeSide side, int x, int y, int w, int h, uint16_t bgColor);

    /* Destructor. */
    ~Eye();

    /* Methods. */
    int x(void);
    int y(void);
    int w(void);
    int h(void);
    void update(int x, int y, int w, int h);
    void setBgColor(uint16_t color);
    void draw(TFT_eSprite &screen);

  private:
    /* Side. */
    EyeSide m_side;

    /* Eye coordinates. */
    int m_x;
    int m_y;

    /* Eye width and height. */
    int m_w;
    int m_h;

    /* Background color. */
    uint16_t m_bgColor;
    
};

class Mouth
{

  public:

    Mouth(int x, int y, int w, int z);
    ~Mouth();

    int x();
    int y();
    int w();
    int z();

    void update(int x, int y, int w, int h);
    void draw(TFT_eSprite &screen);

  private:

    int m_x;
    int m_y;
    int m_w;
    int m_z;
    
};


class PalFace
{
  public:

    PalFace(uint16_t bgColor);
    ~PalFace();

    /* Methods. */
    void draw(TFT_eSprite &screen);
    void setBgColor(uint16_t color);

    /* Control face. */
    void moveEyes(int x, int y);
    void moveMouth(int x, int y, int z);
    void changeEyes(int w, int h);
  
  private:

    Eye m_right_eye;
    Eye m_left_eye;
    Mouth m_mouth;
    uint16_t m_bgColor;
};

#endif /* __INC_FACE_HPP */
