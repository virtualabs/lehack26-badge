#include "animator.hpp"

Animator::Animator(PalFace &face) : m_face(face), m_transition(NULL), m_state(120, 120, EYE_H, MOUTH_X, MOUTH_Y, MOUTH_Z), oldX(0), oldY(0)
{
  m_face.moveEyes(m_state.getEyesX(), m_state.getEyesY());
}

Animator::~Animator()
{
}

void Animator::setTransition(Transition *pTransition)
{
  if (m_transition == NULL)
  {
    m_transition = pTransition;
  }
}

bool Animator::animate(void)
{
  bool need_refresh = false;
  
  /* Compute next frame parameters. */
  if (m_transition != NULL)
  {
    m_state = m_transition->nextState();

    if ((oldX != m_state.getEyesX()) || (oldY != m_state.getEyesY()))
    {
      m_face.moveEyes(m_state.getEyesX(), m_state.getEyesY());
      m_face.changeEyes(EYE_W, m_state.getEyesH());
      m_face.moveMouth(m_state.getMouthX(), m_state.getMouthY(), m_state.getMouthZ());
      oldX = m_state.getEyesX();
      oldY = m_state.getEyesY();
      need_refresh = true;
    }

    /* If transition has ended, remove it. */
    if (m_transition->isEnded())
    {
      free(m_transition);
      m_transition = NULL;
    }
  }

  return need_refresh;
}

FaceState &Animator::getCurrentState(void)
{
  return m_state;
}
