#include "transition.hpp"

/* Constructor. */
FaceState::FaceState(int eyesX, int eyesY, int eyesH, int mouthX, int mouthY, int mouthZ) : mEyesX(eyesX), mEyesY(eyesY),
  mEyesH(eyesH), mMouthX(mouthX), mMouthY(mouthY), mMouthZ(mouthZ)
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

int FaceState::getEyesH(void)
{
  return mEyesH;
}

void FaceState::setEyesH(int h)
{
  mEyesH = h;
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

int FaceState::getMouthZ(void)
{
  return mMouthZ;
}

void FaceState::setMouthZ(int z)
{
  mMouthZ = z;
}

Transition::Transition(FaceState &start, FaceState &end, int duration) : mStartState(start), mEndState(end),
  mCurrentState(start.getEyesX(), start.getEyesY(), start.getEyesH(), start.getMouthX(), start.getMouthY(), start.getMouthZ()),
  mDuration(duration), mFrameNumber(0)
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
    mCurrentState.setEyesH(mStartState.getEyesH() + (mFrameNumber*(mEndState.getEyesH() - mStartState.getEyesH()) / mDuration));
    mCurrentState.setMouthX(mStartState.getMouthX() + (mFrameNumber*(mEndState.getMouthX() - mStartState.getMouthX()) / mDuration));
    mCurrentState.setMouthY(mStartState.getMouthY() + (mFrameNumber*(mEndState.getMouthY() - mStartState.getMouthY()) / mDuration));
    mCurrentState.setMouthZ(mStartState.getMouthZ() + (mFrameNumber*(mEndState.getMouthZ() - mStartState.getMouthZ()) / mDuration));

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
