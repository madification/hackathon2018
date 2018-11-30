//Includes
#include "globals.h"

// create servo objects to control the pan and tilt servos
Servo panServo, tiltServo;

MovementHandler mvmtHdlr(panVal, tiltVal, panServo, tiltServo);


// This is the main Pixy object
Pixy2 pixy;
PixyHandler pxyHdlr(pixy);


// instantiate a desired object and a discovered object
Target desired;
Target discovered;


void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();

  //initialize servos
  panServo.attach(PAN);  // attaches/activates the pan servo on pin PAN
  tiltServo.attach(TILT);  // attaches/activates the tilt servo on pin TILT

  //initalize digital pins
  pinMode(LASER, OUTPUT);      //set the LASER Pin to an output
  digitalWrite(LASER, LOW);

  // Set up the LED
  pinMode(MYLED, OUTPUT);
  digitalWrite(MYLED, LOW);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set up the button
  pinMode(BUTTON, INPUT);
  pinMode(EXIT, INPUT);

  //Now that everything is set up, lets do a quick buzz test
  //write servo positions to some position other than 'home'
  panServo.write(120);  //sets the pan servo position to the default 'home' value
  tiltServo.write(120);//sets the tilt servo position to the default 'home' value
  delay(1000);

  digitalWrite(LASER, HIGH);

  //write servo positions to some position other than 'home'
  panServo.write(70);  //sets the pan servo position to the default 'home' value
  delay(1000);
  digitalWrite(LASER, LOW);

  //write initial servo positions to set the servos to 'home'
  panServo.write(90);  //sets the pan servo position to the default 'home' value
  tiltServo.write(90);//sets the tilt servo position to the default 'home' value
  delay(1000);


  Serial.println("Initialized");

}

void loop()
{
  // See if we've commanded to exit
  if (!exitCondition)
  {
    // Check to see if we've commanded to begin
    if (digitalRead(BUTTON))
    {
      buttonClicked = true;
    }

    // grab blocks, hopefully only one, to set our desired
    pixy.ccc.getBlocks();

    // check if there's more than one, hopefully not
    if (pixy.ccc.numBlocks > 1)
    {
      Serial.print(pixy.ccc.numBlocks);
      Serial.println(" blocks detected");
      Serial.println("Selecting block ");
      Serial.println(pixy.ccc.blocks[0].m_signature);
      // Set our desired object
      if (pixy.ccc.blocks[0].m_signature != 33 || pixy.ccc.blocks[0].m_signature != 0)
      {
        desired.populateTarget(pixy.ccc.blocks[0].m_signature, pixy.ccc.blocks[0].m_index, pixy.ccc.blocks[0].m_x, pixy.ccc.blocks[0].m_y);
      }
    }
    else
    {
      // Set our desired object

      if (pixy.ccc.blocks[0].m_signature != 33 && pixy.ccc.blocks[0].m_signature != 0)
      {
        desired.populateTarget(pixy.ccc.blocks[0].m_signature, pixy.ccc.blocks[0].m_index, pixy.ccc.blocks[0].m_x, pixy.ccc.blocks[0].m_y);
      }
      Serial.print("Selectedblock ");
      Serial.println(desired.getSignature());
    }


    if (buttonClicked)
    {
      // Show that we're in our 'operate' loop
      digitalWrite(MYLED, HIGH);

      //Serial.print("Now searching for block ");
      //Serial.println(desired.getSignature());

      // grab blocks!
      discovered = pxyHdlr.getUpdatedBlockData(desired.getSignature(), desired.getIndex());
      //Serial.print("Discovered block ");
      //Serial.println(discovered.getSignature());

      double xerror = CENTEREDX - discovered.getX();
      double yerror = CENTEREDY - discovered.getY();

      // We've found our object, we need to drive it to the center of our frame
      boolean outsideTolerance = true;
      while (outsideTolerance)
      {
        // Check our error to see if we've made it to our goal
        if ((xerror < 10 && xerror > -10) && (yerror < 10 && yerror > -10))
        {
          Serial.println("Within tolerance of + or - 10 of centered ");
          outsideTolerance = false;

        }

        mvmtHdlr.panPorportional(xerror);
        mvmtHdlr.tiltPorportional(yerror);

        //Serial.print("discovered's x val ");
        //Serial.println(discovered.getX());

        // Pixy x range = 0-316
        // Pixy y range = 0-208

        // get our updated position
        discovered = pxyHdlr.getUpdatedBlockData(desired.getSignature(), desired.getIndex());

        if (discovered.getX() == 0 && discovered.getY() == -1)
        {
          //lost the block, start over
          Serial.println("x = 0, y = -1");
          outsideTolerance = false;
          continue;
        }
        xerror = CENTEREDX - discovered.getX();
        //Serial.print("x = ");
        //Serial.println(discovered.getX());
        //Serial.print("xerror = ");
        //Serial.println(xerror);
        yerror = CENTEREDY - discovered.getY();
        Serial.print("y = ");
        Serial.println(discovered.getY());
        Serial.print("yerror = ");
        Serial.println(yerror);
      }
    }

    exitCondition = digitalRead(EXIT);
  }
  else
  {
    // We want to be able to restart. If exit button held > 3 seconds, reset
    digitalWrite(MYLED, LOW);
    buttonClicked = false;
    unsigned long currT = millis();
    while (digitalRead(EXIT))
    {
      // do nothing and let time pass
    }
    if ((millis() - currT) > 3000)
    {
      Serial.println("elapsed time = ");
      Serial.print(millis() - currT);
      exitCondition = false;
    }
  }
}
