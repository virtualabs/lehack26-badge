#include "face.hpp"

/**
 * Initialize an Eye object on screen at position (x,y).
 */

Eye::Eye(int x, int y) {
  m_x = x;
  m_y = y;
  m_w = EYE_W;
  m_h = EYE_H;
}


/**
 * Initialize an Eye object on screen at position (x,y) of size (w,h).
 */
 
Eye::Eye(int x, int y, int w, int h) {
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
}

/**
 * Destructor.
 */
Eye::~Eye() {
}

/**
 * Getters.
 */
int Eye::x(void) {
  return m_x;
}

int Eye::y(void) {
  return m_y;
}

int Eye::w(void) {
  return m_w;
}

int Eye::h(void) {
  return m_h;
}

void Eye::update(int x, int y, int w, int h) {
  m_x = x;
  m_y = y;
  m_w = w;
  m_h = h;
}

/**
 * Draws the eye on screen.
 */

void Eye::draw(TFT_eSprite &screen) {
  int h = m_h - 20*abs(m_x - 120)/120;
  screen.fillSmoothRoundRect(m_x, m_y - h/2, m_w, h, 10, TFT_WHITE, TFT_SKYBLUE);
}

/***********************
 * Mouth
 **********************/

Mouth::Mouth(int x, int y, int w, int z) : m_x(x), m_y(y), m_w(w), m_z(z)
{
}

Mouth::~Mouth()
{
}

int Mouth::x(void)
{
  return m_x;
}

int Mouth::y(void)
{
  return m_y;
}

int Mouth::w(void)
{
  return m_w;
}

int Mouth::z(void)
{
  return m_z;
}

void Mouth::update(int x, int y, int w, int z)
{
  m_x = x;
  m_y = y;
  m_w = w;
  m_z = z;
}

void Mouth::draw(TFT_eSprite &screen)
{
  int x = 15 + ((m_x*210)/240);
  int w = m_w - 20*abs(m_x - 120)/120;
  int y = m_y - 10*abs(m_x - 120)/120;
  screen.drawWideLine(x - w/2 - 10, y, x - w/2, y + 10, 10, TFT_WHITE, TFT_WHITE);
  screen.drawWideLine(x + w/2, y + 10, x + w/2 + 10, y, 10, TFT_WHITE, TFT_WHITE);
  screen.drawWideLine(x - w/2, y + 10, x + w/2, y+10, 10, TFT_WHITE, TFT_WHITE);
}

/***********************
 * Face management.
 ***********************/

PalFace::PalFace() : m_right_eye(EYE_RIGHT_DEF_X, EYE_RIGHT_DEF_Y), m_left_eye(EYE_LEFT_DEF_X, EYE_LEFT_DEF_Y),
  m_mouth(MOUTH_X, MOUTH_Y, MOUTH_W, MOUTH_Z)
{
}

PalFace::~PalFace() {

}

void PalFace::moveEyes(int x, int y) {
  m_right_eye.update(x + EYE_SPACING/2, y - m_right_eye.h()/2, m_right_eye.w(), m_right_eye.h());
  m_left_eye.update(x - EYE_SPACING/2 - m_left_eye.w(), y - m_left_eye.h()/2, m_left_eye.w(), m_left_eye.h());
}

void PalFace::moveMouth(int x, int y) {
  m_mouth.update(x, y, m_mouth.w(), m_mouth.z());
}

void PalFace::changeEyes(int w, int h) {
  m_right_eye.update(m_right_eye.x(), m_right_eye.y(), w, h);
  m_left_eye.update(m_left_eye.x(), m_left_eye.y(), w, h);
}

void PalFace::draw(TFT_eSprite &screen) {
  screen.fillScreen(BG_COLOR);
  m_left_eye.draw(screen);
  m_right_eye.draw(screen);
  m_mouth.draw(screen);
}
