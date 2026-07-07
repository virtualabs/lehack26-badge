#include "transition.hpp"

/* Constructor. */
FaceState::FaceState(int eyesX, int eyesY, int mouthX, int mouthY) : mEyesX(eyesX), mEyesY(eyesY),
  mMouthX(mouthX), mMouthY(mouthY)
{
}

FaceState::~FaceState()
{
}

int FaceState::getEyesX(void)
{
  return mEyesX;
}

void FaceState::setEyesX(int x)
{
  mEyesX = x;
}

int FaceState::getEyesY(void)
{
  return mEyesY;
}

void FaceState::setEyesY(int y)
{
  mEyesY = y;
}

int FaceState::getMouthY(void)
{
  return mMouthY;
}

void FaceState::setMouthY(int y)
{
  mMouthY = y;
}

int FaceState::getMouthX(void)
{
  return mMouthX;
}

void FaceState::setMouthX(int x)
{
  mMouthX = x;
}

Transition::Transition(FaceState &start, FaceState &end, int duration) : mStartState(start), mEndState(end),
  mCurrentState(start.getEyesX(), start.getEyesY(), start.getMouthX(), start.getMouthY()), mDuration(duration), mFrameNumber(0)
{
  
}

FaceState &Transition::nextState(void)
{
  /* Increment frame number. */

  if (mFrameNumber < mDuration)
  {
    /* Increment frame number. */
    mFrameNumber++;

    /* Update current state. */
    mCurrentState.setEyesX(mStartState.getEyesX() + (mFrameNumber*(mEndState.getEyesX() - mStartState.getEyesX()) / mDuration));
    mCurrentState.setEyesY(mStartState.getEyesY() + (mFrameNumber*(mEndState.getEyesY() - mStartState.getEyesY()) / mDuration));
    mCurrentState.setMouthX(mStartState.getMouthX() + (mFrameNumber*(mEndState.getMouthX() - mStartState.getMouthX()) / mDuration));
    mCurrentState.setMouthY(mStartState.getMouthY() + (mFrameNumber*(mEndState.getMouthY() - mStartState.getMouthY()) / mDuration));

    /* Return our current state. */
    return mCurrentState;
  }
  else
  {
    /* Transition done, return end state. */
    return mEndState;
  }
}

bool Transition::isEnded(void)
{
  return mFrameNumber == mDuration;
}
