
#include "ESPEssentials.h"

#include <RemoteDebug.h>  //https://github.com/JoaoLopesF/RemoteDebug


// Adafruit MCP23017 I/O expander library
// https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;
// ESP GPIOs (GPIO13 and A0 pins are spare)
// use pinMode() / digitalRead() / digitalWrite()
#define STEP1       14
#define STEP2       12
#define LASERDETECTOR A0

// MCP GPIOs
// use mcp.pinMode() / mcp.digitalRead() / mcp.digitalWrite()
#define DIRN1       0   // GPA0
#define MOTOR1      1   // GPA1
#define SPEED1_MS3  2   // GPA2
#define SPEED1_MS2  3   // GPA3
#define SPEED1_MS1  4   // GPA4
#define GPA5        5   // spare
#define GPA6        6   // spare
#define LIMIT1      7   // GPA7
#define DIRN2       8   // GPB0
#define MOTOR2      9   // GPB1
#define SPEED2_MS3  10  // GPB2
#define SPEED2_MS2  11  // GPB3    
#define SPEED2_MS1  12  // GPB4
#define GPB5        13  // spare
#define GPB6        14  // spare
#define LIMIT2      15  // GPB7
#define LASERPIN    GPB6


RemoteDebug Debug;


void setup()
{
  initESPEssentials("ESPLaser");// Initialize the server (telnet or web socket) of RemoteDebug
  Debug.begin("ESPLaser");
  // OR
  //Debug.begin(HOST_NAME, startingDebugLevel);
  // Options
  Debug.setResetCmdEnabled(true); // Enable the reset command
  //Debug.showProfiler(true); // To show profiler - time between messages of Debug
  
  WebServer.on("/reset_wifi", HTTP_GET, [&]() {
    WebServer.send(200, "text/plain", "Wifi settings reset.");
    Wifi.resetSettings();
  });

  WebServer.on("/motors", HTTP_GET, html_motors);               //inside originalStepperCode.ino
  WebServer.on("/laseron", HTTP_GET, laser_on);                 //inside laserfunctions.ino
  WebServer.on("/laseroff", HTTP_GET, laser_off);               //inside laserfunctions.ino
  WebServer.on("/laserflash", HTTP_GET, handle_laser_flash);    //inside laserfunctions.ino
  WebServer.on("/laserread", HTTP_GET, handle_laser_read);      //inside laserfunctions.ino  
  WebServer.on("/homing", HTTP_GET, homing);                    //inside steppers.ino

  mcp.begin ();
  mcp.pinMode(LASERPIN, OUTPUT);
  pinMode(LASERDETECTOR, INPUT); // analog

}

void loop()
{
  handleESPEssentials();
  Debug.handle();
}
