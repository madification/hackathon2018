#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <Servo.h>           //include the servo library for working with servo objects
#include <Pixy2.h>
#include "Target.h"
#include "PixyHandler.h"
#include "MovementHandler.h"

//Defines
#define PAN 10              //Pan Servo Digital Pin
#define TILT 11             //Tilt Servo Digital Pin
#define LASER 2             //Laser Digital Pin
#define MYLED 22
#define BUTTON 26
#define EXIT 25
#define MAX_PULSE  1750
#define MIN_PULSE  1250
#define CENTER_PULSE 1500

#define CENTEREDX 158
#define CENTEREDY 104

int panVal = 90;   //current positional value being sent to the pan servo.
int tiltVal = 90;  //current positional value being sent to the tilt servo.


boolean doneOnce = false;
boolean buttonClicked = false;
boolean exitCondition = false;

#endif
