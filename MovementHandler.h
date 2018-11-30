#ifndef __MHANDLER_H__
#define __MHANDLER_H__

//Includes
#include <Servo.h>
#include "globals.h"

class MovementHandler
{
  private:
  Servo panServo, tiltServo;
  
    int currPanVal;
    int currTiltVal;

    const float Pp = 0.05;
    const float Pt = 0.05;
    const int D = 1;

  public:
    // Constructor
    MovementHandler(int panVal, int tiltVal, Servo pan, Servo tilt)
    {
      currPanVal = panVal;
      currTiltVal = tiltVal;
      panServo = pan;
      tiltServo = tilt;
    }

    int panTo(int panPos)
    {
      // make sure we're within our operational range
      if (panPos < 0)
      {
        panPos = 0;
      }
      else if (panPos > 180)
      {
        panPos = 180;
      }
      
      //Write our new value to our servo
      panServo.write(panPos);

      //Update our current value
      currPanVal = panPos;
      
      return currPanVal;
    }

     int tiltTo(int tiltPos)
    {
      // make sure we're within our operational range
      if (tiltPos < 0)
      {
        tiltPos = 0;
      }
      else if (tiltPos > 180)
      {
        tiltPos = 180;
      }
      
      //Write our new value to our servo
      tiltServo.write(tiltPos);

      //Update our current value
      currTiltVal = tiltPos;
      
      return tiltPos;
    }

    int panPorportional(double error)
    {
      int newVal = currPanVal + Pp * error;

      //Write our new value to our servo
      panTo(newVal);

      //Update our current value
      currPanVal = newVal;
      
      return newVal;
    }

    int tiltPorportional(double error)
    {
      int newVal = currTiltVal - Pt * error;
      
      //Write our new value to our servo
      tiltTo(newVal);

      //Update our current value
      currTiltVal = newVal;
      
      return newVal;
    }

    int moveLeft(int ticks)
    {
      int newVal = currPanVal + ticks;
      
      // make sure we're within our operational range
      if (newVal < 0)
      {
        newVal = 0;
      }
      else if (newVal > 180)
      {
        newVal = 180;
      }
      
      //Write our new value to our servo
      panServo.write(newVal);

      //Update our current value
      currPanVal = newVal;
      
      return newVal;
    }

    int moveRight(int ticks)
    {
      int newVal = currPanVal - ticks;
      
      // make sure we're within our operational range
      if (newVal < 0)
      {
        newVal = 0;
      }
      else if (newVal > 180)
      {
        newVal = 180;
      }
      
      //Write our new value to our servo
      panServo.write(newVal);

      //Update our current value
      currPanVal = newVal;
      
      return newVal;
    }

    int moveUp(int ticks)
    {
      int newVal = currTiltVal - ticks;
      
      // make sure we're within our operational range
      if (newVal < 0)
      {
        newVal = 0;
      }
      else if (newVal > 180)
      {
        newVal = 180;
      }
      
      //Write our new value to our servo
      tiltServo.write(newVal);

      //Update our current value
      currTiltVal = newVal;
      
      return newVal;
    }

    int moveDown(int ticks)
    {
      int newVal = currTiltVal + ticks;
      
      // make sure we're within our operational range
      if (newVal < 0)
      {
        newVal = 0;
      }
      else if (newVal > 180)
      {
        newVal = 180;
      }
      
      //Write our new value to our servo
      tiltServo.write(newVal);

      //Update our current value
      currTiltVal = newVal;
      
      return newVal;
    }

};
#endif
