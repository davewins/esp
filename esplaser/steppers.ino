
static int stepsPerRevolution = 200;  // steps per revolution
boolean calibrated = false;
boolean calibration = false;

int photodiodeValue = 0;    // photodiode value needed for calibration
int photodiodeThreshold = 550; // min value when laser points to diode

int returnY = 109;  // Y steps needed to return home from calibration
int returnX = 12;  // X steps needed to return home from calibration


#define MOTOR_X     MOTOR2
#define MOTOR_Y     MOTOR1

//Handle HTTP Requests
//Home the Stepper Moters!
void homing() {
  //laseron();
  Serial.println("Calibrating the stepper motors");
  calibrate();

  //int i=0;
  //int laserValue=0;
  
  /*while(i<20) {
      laseron();
      laserValue = analogRead(LASERDETECTOR);
      Debug.printf("Stepper Count %d, LaserDetector Value= %d\n", i, laserValue);
      delay(500);
      laseroff();
      laserValue = analogRead(LASERDETECTOR);
      Debug.printf("Stepper Count %d, LaserDetector Value= %d\n", i, laserValue);
      i++;
  }*/
  
  
  laseroff();
  const char* response="{\"ResponseBody\":\"Stepper motors homed\"}";
  WebServer.send(200, "application/json", response);
}

boolean calibrate() {
  calibration = true;
  setupMotors();
  //cLedOn();
  Debug.println("XY Calibration");

  //stepY(returnY, 1);
  //stepX(returnX, 1);
  laseron();
  for (int a = 0; a <= stepsPerRevolution; a++)
  {
    //Debug.printf("outside loop a = %d",a);

    for (int i = 0; i <= stepsPerRevolution; i++)
    {
      //Debug.printf("inside loop i = %d",i);
      //serialEvent();
      //if (!(calibration))
      //{
      //  notSuccessful();
      //  return false;
      //}
      //laseron();
      //photodiodeValue = map(analogRead(LASERDETECTOR), 0, 1023, 0, 255);
      photodiodeValue = analogRead(LASERDETECTOR);
      Serial.printf("Homing Laser Value: %d\n", photodiodeValue);
      //delay(50);
      if (photodiodeValue >= photodiodeThreshold)
      {
        //laseroff();
        //rotateHome();
        Serial.println("Calibrated");
        //cLedOff();
        calibration = false;
        calibrated = true;
          // Disable any motors (saves power)
          mcp.pinMode(MOTOR1, INPUT);
          mcp.pinMode(MOTOR2, INPUT);
        return true;
      }
      stepX(1, 1);
    }
    stepY(1, 1);
  }
  notSuccessful();
  // Disable any motors (saves power)
  mcp.pinMode(MOTOR1, INPUT);
  mcp.pinMode(MOTOR2, INPUT);
}



/*void calibrate()
{
  setupMotors();
  stepY(200,1);
  delay(1000);
  stepX(200,1);
  delay(1000);
  stepY(200,2);
  delay(1000);
  stepX(200,2);
  mcp.pinMode(MOTOR1, INPUT);
  mcp.pinMode(MOTOR2, INPUT);
}*/

boolean notSuccessful()
{
  Serial.printf("Calibration not successful.\n");
  laseroff();
  //cLedOff();
  //rotateHome();
  calibrated = true;
  calibration = false;
  return false;
}

void rotateHome()
{
  stepY(returnY, 2);
  stepX(returnX, 2);
  // Disable any motors (saves power)
  mcp.pinMode(MOTOR_Y, INPUT);
  mcp.pinMode(MOTOR_X, INPUT);
}

//-----stepper-and-laser-control----------

void setupMotors()
{
    //Set up MOTOR_Y
    pinMode(STEP1, OUTPUT);
    digitalWrite(STEP1, LOW);
    mcp.pinMode(DIRN1, OUTPUT);
    mcp.pinMode(SPEED1_MS3, OUTPUT);
    mcp.pinMode(SPEED1_MS2, OUTPUT);
    mcp.pinMode(SPEED1_MS1, OUTPUT);
    mcp.pinMode(MOTOR_Y, OUTPUT);
    // Set microstepping to Full step mode
    mcp.digitalWrite(SPEED1_MS1, LOW);
    mcp.digitalWrite(SPEED1_MS2, LOW);
    mcp.digitalWrite(SPEED1_MS3, LOW);
    mcp.digitalWrite(MOTOR_Y, HIGH); // Enable motor, ready for stepping

    //Set up MOTOR_X
    pinMode(STEP2, OUTPUT);
    digitalWrite(STEP2, LOW);
    mcp.pinMode(DIRN2, OUTPUT);
    mcp.pinMode(SPEED2_MS3, OUTPUT);
    mcp.pinMode(SPEED2_MS2, OUTPUT);
    mcp.pinMode(SPEED2_MS1, OUTPUT);
    // Set microstepping to Full step mode
    mcp.digitalWrite(SPEED2_MS1, LOW);
    mcp.digitalWrite(SPEED2_MS2, LOW);
    mcp.digitalWrite(SPEED2_MS3, LOW);
    mcp.pinMode(MOTOR_X, OUTPUT);
    mcp.digitalWrite(MOTOR_X, HIGH); // Enable motor, ready for stepping
}

void stepX(int steps, int newDirection)
{
  if (newDirection == 1)
    mcp.digitalWrite(DIRN2, LOW); // Pull DIRN pin LOW to move "forward"
  else
    mcp.digitalWrite(DIRN2, HIGH); // Pull DIRN pin HIGH to move "backward"
  //Debug.printf("Stepping X %d Steps in direction %d\n",steps, newDirection);
  for (int a = 0; a <= steps; a++)
  {
    digitalWrite(STEP2, HIGH); // Trigger one step, at least 1us pulse width
    digitalWrite(STEP2, LOW);  // Pull step pin low so it can be triggered again
    delayMicroseconds(1000);
  }
}

void stepY(int steps, int newDirection)
{
  if (newDirection == 1)
    mcp.digitalWrite(DIRN1, LOW); // Pull DIRN pin LOW to move "forward"
  else
    mcp.digitalWrite(DIRN1, HIGH); // Pull DIRN pin LOW to move "backward"
  //Debug.printf("Stepping Y %d Steps in direction %d\n",steps, newDirection);
  for (int a = 0; a <= steps; a++)
  {
    digitalWrite(STEP1, HIGH); // Trigger one step, at least 1us pulse width
    digitalWrite(STEP1, LOW);  // Pull step pin low so it can be triggered again
    delayMicroseconds(1000);
  }
}


/*
void moveX(int newPosition)
{
  x.moveTo(newPosition);

  while (x.distanceToGo() != 0)
  {
    x.run();
  }
}

void moveY(int newPosition)
{
  y.moveTo(newPosition);

  while (y.distanceToGo() != 0)
  {
    y.run();
  }
}

void moveXY(int newXPosition, int newYPosition)
{
  x.moveTo(newXPosition);
  y.moveTo(newYPosition);

  while (x.distanceToGo() != 0 || y.distanceToGo() != 0)
  {
    x.run();
    y.run();
  }
}

*/
