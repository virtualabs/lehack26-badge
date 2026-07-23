#ifndef __INC_ANIMATOR_HPP
#define __INC_ANIMATOR_HPP

#include "face.hpp"
#include "transition.hpp"

class Animator
{
  public:

    Animator(PalFace &face);
    ~Animator();

    void setTransition(Transition *pTransition);
    bool animate(void);
    FaceState &getCurrentState(void);
    bool hasTransition(void);

  private:
    int oldX, oldY;
    PalFace &m_face;
    FaceState m_state;
    Transition *m_transition;
};

#endif /* __INC_ANIMATOR_HPP */
