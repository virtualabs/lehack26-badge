#ifndef __INC_TRANSITION_HPP
#define __INC_TRANSITION_HPP

class FaceState
{
  public:

    FaceState(int eyesX, int eyesY, int eyesH, int mouthX, int mouthY, int mouthZ);
    ~FaceState();

    int getEyesX(void);
    void setEyesX(int x);
    int getEyesY(void);
    void setEyesY(int y);
    int getEyesH(void);
    void setEyesH(int h);
    int getMouthX(void);
    void setMouthX(int x);
    int getMouthY(void);
    void setMouthY(int y);
    int getMouthZ(void);
    void setMouthZ(int z);

  private:

    /* Eye parameters. */
    int mEyesX;
    int mEyesY;
    int mEyesH;
    int mMouthX;
    int mMouthY;
    int mMouthZ;
    
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
