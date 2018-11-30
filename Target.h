#ifndef __TARGET_H__
#define __TARGET_H__

class Target
{
  private:
  int signature;
  int index;
  int x;
  int y;

  public:
  Target()
  {
    signature = -1;
    index = -1;
    x = -1;
    y = -2;
  }

//This is basically acting as an overloaded constructor, but allows us to essentially fill an object that was already instantiated
void populateTarget(int sig, int ndx, int ex, int why)
{
  signature = sig;
  index = ndx;
  x = ex;
  y = why;  
}
  
  void setSignature(int sig)
  {
    signature = sig;
  }

  int getSignature()
  {
    return signature;
  }

  int getIndex()
  {
    return index;
  }

  // I don't think I'll ever use the remaining functions
  void setX(int ex)
  {
    x = ex;
  }
  
  int getX()
  {
    return x;
  }

  void setY(int why)
  {
    y = why;
  }

  int getY()
  {
    return y;
  }
  
  
};

#endif
