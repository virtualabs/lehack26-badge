#ifndef __INC_TRANSITION_HPP
#define __INC_TRANSITION_HPP

class FaceState
{
  public:

    FaceState(int eyes_x, int eyes_y, int mouth_x, int mouth_y);
    ~FaceState();

    int getEyesX(void);
    void setEyesX(int x);
    int getEyesY(void);
    void setEyesY(int y);
    int getMouthX(void);
    void setMouthX(int x);
    int getMouthY(void);
    void setMouthY(int y);

  private:

    /* Eye parameters. */
    int mEyesX;
    int mEyesY;
    int mMouthX;
    int mMouthY;
    
};

class Transition
{
  public:
    Transition(FaceState &start, FaceState &end, int duration);
    
    FaceState &nextState(void);
    bool isEnded(void);

  private:

    FaceState &mStartState;
    FaceState mCurrentState;
    FaceState &mEndState;
    int mDuration;
    int mFrameNumber;
};


#endif /* __INC_TRANSITION_HPP */
