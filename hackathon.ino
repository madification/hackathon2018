//Includes
#include <Servo.h>           //include the servo library for working with servo objects
#include <Pixy2.h>
#include "Target.h"
#include "PixyHandler.h"

//Defines
#define PAN 10              //Pan Servo Digital Pin
#define TILT 11             //Tilt Servo Digital Pin
#define LASER 2             //Laser Digital Pin
#define LED 4               //Digital pin to control external LED
#define BUTTON 12           //Digital pin to read button
#define EXIT 25             //Digital pin to read button
#define SLIDER 24

// create servo objects to control the pan and tilt servos
Servo panServo, tiltServo;

// This is the main Pixy object and my custom handler for it
Pixy2 pixy;
PixyHandler pxyHdlr(pixy);

// instantiate a desired object
Target desired;
Target discovered;

// just some global vars
int panVal = 90;   //current positional value being sent to the pan servo.
int tiltVal = 90;  //current positional value being sent to the tilt servo.
boolean doneOnce = false;
boolean buttonClicked = false;
boolean exitCondition = false;

void setup()
{
  // Setup the ability to print b/c lesbeyonest, you need it
  Serial.begin(115200);
  Serial.print("Starting...\n");

  // Initialize our pixy cam
  pixy.init();

  // Setup our motors
  panServo.attach(PAN);  // attaches/activates the pan servo on pin PAN
  tiltServo.attach(TILT);  // attaches/activates the tilt servo on pin TILT

  // Setup the laser
  pinMode(LASER, OUTPUT);      //set the LASER Pin to an output
  digitalWrite(LASER, LOW);

  // Setup the LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setup the buttons
  pinMode(BUTTON, INPUT);
  pinMode(EXIT, INPUT);

  // Finally setup the slider switch
  pinMode(SLIDER, INPUT);


  // Now lets start up our servos do a check to make sure everything's working
  // and end such that we will always start our actual process in 'home position'
  panServo.write(120);
  tiltServo.write(120);
  delay(1000);

  digitalWrite(LASER, HIGH);

  //write servo positions to some position other than 'home'
  panServo.write(70);  //sets the pan servo position to the default 'home' value
  delay(1000);
  digitalWrite(LASER, LOW);

  //write initial servo positions to set the servos to 'home'
  panServo.write(90);
  tiltServo.write(90);
}

void loop() {

  if (!digitalRead(SLIDER))
  {
    // If slider is false, this is the opportunity to train
    // Continually read blocks and...
    pixy.ccc.getBlocks();

    // ...check to see if we've signaled we're ready to train.
    //When button is clicked, the human user is indicating it's time to save block info
    if (digitalRead(BUTTON))
    {
      // Setting a boolean like this prevents us being affected by a bouncing switch
      buttonClicked = true;
    }

    // Once the button is clicked, save off our data
    if (buttonClicked)
    {

      // check if there's more than one, hopefully not
      if (pixy.ccc.numBlocks > 1)
      {
        Serial.print(pixy.ccc.numBlocks);
        Serial.println(" blocks detected");
        Serial.println("Selecting block ");
        Serial.println(pixy.ccc.blocks[0].m_signature);
        // Set our desired object
        desired.populateTarget(pixy.ccc.blocks[0].m_signature, pixy.ccc.blocks[0].m_index, pixy.ccc.blocks[0].m_x, pixy.ccc.blocks[0].m_y);
      }
      else
      {
        // Set our desired object
        desired.populateTarget(pixy.ccc.blocks[0].m_signature, pixy.ccc.blocks[0].m_index, pixy.ccc.blocks[0].m_x, pixy.ccc.blocks[0].m_y);
        Serial.print("Selectedblock ");
        Serial.println(desired.getSignature());
      }
      // We've got our info saved, now don't write over it unless the user
      // indicates they want to do so by clicking the button again
      buttonClicked = false;
    }
  }
  else // slider has been switched indicating we've trained and are ready to begin!
  {
    
  }
}
