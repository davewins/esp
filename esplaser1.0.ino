
#include <ESPEssentials.h>

#include "RemoteDebug.h"  //https://github.com/JoaoLopesF/RemoteDebug


// Adafruit MCP23017 I/O expander library
// https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;
// ESP GPIOs (GPIO13 and A0 pins are spare)
// use pinMode() / digitalRead() / digitalWrite()
#define STEP1       14
#define STEP2       12

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


//Turn on the Laser!
void laser_on() {
  mcp.digitalWrite(LASERPIN, HIGH);
  Debug.println("Laser ON");
  WebServer.send(200, "text/plain", "Laser On");
}

//Turn off the Laser!
void laser_off() {
  mcp.digitalWrite(LASERPIN, LOW);
  Debug.println("Laser OFF");
  WebServer.send(200, "text/plain", "Laser Off");
}

//Flash the Laser!
void flash_laser(int timepersecond, int totalseconds) {
  //mcp.digitalWrite(GPB6, LOW);
  Debug.println("Flashing Laser");
  WebServer.send(200, "text/plain", "Laser Flashing");
}

void handle_laser_flash() {
  int timespersecond = (WebServer.arg("timespersecond").toInt());
  int totalseconds = (WebServer.arg("totalseconds").toInt());
  WebServer.send(200, "text/plain", "Laser Flashing");

}

// Process FORM arguments from main motors web page
void html_motors() {
  //  if (captivePortal()) return;
  bool motor1 = (WebServer.arg("motor1") == "1");
  int speed1 = WebServer.arg("speed1").toInt();
  int step1 = WebServer.arg("step1").toInt();
  bool forward1 = (WebServer.arg("dirn1") != "1");
  bool limit1 = (WebServer.arg("limit1") == "1");
  bool motor2 = (WebServer.arg("motor2") == "1");
  int speed2 = WebServer.arg("speed2").toInt();
  int step2 = WebServer.arg("step2").toInt();
  bool forward2 = (WebServer.arg("dirn2") != "1");
  bool limit2 = (WebServer.arg("limit2") == "1");
  int secs = WebServer.arg("time").toInt();
  run_motors(motor1, speed1, step1, forward1, limit1,
             motor2, speed2, step2, forward2, limit2, secs);
  WebServer.send(200, "text/plain", "OK");
  Debug.println("html_motors(): finished");
}

// Run the motors using parameters from the web form
void run_motors(bool motor1, int speed1, int step1, bool forward1, bool limit1,
                bool motor2, int speed2, int step2, bool forward2, bool limit2,
                int secs) {
  // Setup motor #1
  if (motor1) {
    Debug.printf("run_motors(): motor1=%d %d %d %d\r\n", speed1, step1, forward1, limit1);
    pinMode(STEP1, OUTPUT);
    digitalWrite(STEP1, LOW);
    mcp.pinMode(DIRN1, OUTPUT);
    mcp.pinMode(SPEED1_MS3, OUTPUT);
    mcp.pinMode(SPEED1_MS2, OUTPUT);
    mcp.pinMode(SPEED1_MS1, OUTPUT);
    mcp.pinMode(MOTOR1, OUTPUT);
    mcp.digitalWrite(DIRN1, forward1 ? LOW : HIGH); // Pull DIRN pin LOW to move "forward"
    step1 = step1 < 5 ? step1 - 1 : 7; // Microstepping mode 1-5 convert to bitfield 0-7
    mcp.digitalWrite(SPEED1_MS1, step1 & 1 ? HIGH : LOW);
    mcp.digitalWrite(SPEED1_MS2, step1 & 2 ? HIGH : LOW);
    mcp.digitalWrite(SPEED1_MS3, step1 & 4 ? HIGH : LOW);
    mcp.digitalWrite(MOTOR1, HIGH); // Enable motor, ready for stepping
  };

  // Setup motor #2
  if (motor2) {
    Debug.printf("run_motors(): motor2=%d %d %d %d\r\n", speed2, step2, forward2, limit2);
    pinMode(STEP2, OUTPUT);
    digitalWrite(STEP2, LOW);
    mcp.pinMode(DIRN2, OUTPUT);
    mcp.pinMode(SPEED2_MS3, OUTPUT);
    mcp.pinMode(SPEED2_MS2, OUTPUT);
    mcp.pinMode(SPEED2_MS1, OUTPUT);
    mcp.pinMode(MOTOR2, OUTPUT);
    mcp.digitalWrite(DIRN2, forward2 ? LOW : HIGH); // Pull DIRN pin LOW to move "forward"
    step2 = step2 < 5 ? step2 - 1 : 7; // Microstepping mode 1-5 convert to bitfield 0-7
    mcp.digitalWrite(SPEED2_MS1, step2 & 1 ? HIGH : LOW);
    mcp.digitalWrite(SPEED2_MS2, step2 & 2 ? HIGH : LOW);
    mcp.digitalWrite(SPEED2_MS3, step2 & 4 ? HIGH : LOW);
    mcp.digitalWrite(MOTOR2, HIGH); // Enable motor, ready for stepping
  };

  // Run motors for 'secs'
  // timer subtraction works correctly even when micros() wraps (every 71 mins)
  unsigned long tstart, t1, t2, tyield, t;
  tstart = tyield = t1 = t2 = micros();
  while (micros() - tstart < secs * 1000000) {
    t = micros();
    if (motor1 && t - t1 >= 1000000 / speed1) {
      if (limit1 && mcp.digitalRead(LIMIT1) == LOW) {
        Debug.println("run_motors(): Limit switch pressed to stop motor #1!");
        motor1 = false;
        continue;
      };
      digitalWrite(STEP1, HIGH); // Trigger one step, at least 1us pulse width
      digitalWrite(STEP1, LOW);  // Pull step pin low so it can be triggered again
      t1 = t;
    };
    if (motor2 && t - t2 >= 1000000 / speed2) {
      if (limit2 && mcp.digitalRead(LIMIT2) == LOW) {
        Debug.println("run_motors(): Limit switch pressed to stop motor #2!");
        motor2 = false;
        continue;
      };
      digitalWrite(STEP2, HIGH); // Trigger one step, at least 1us pulse width
      digitalWrite(STEP2, LOW);  // Pull step pin low so it can be triggered again
      t2 = t;
    };
    // Run 'housekeeping' tasks (e.g., WiFi) every 0.5 sec
    if (t - tyield >= 500000) {
      tyield = t;
      yield();
    };
    // Wait upto 0.0002 sec before next iteration (max speed=5000 steps/sec)
    if (micros() - t < 200) delayMicroseconds(200 - (micros() - t));
  };

  // Disable any motors (saves power)
  mcp.pinMode(MOTOR1, INPUT);
  mcp.pinMode(MOTOR2, INPUT);
}


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

  WebServer.on("/motors", HTTP_GET, html_motors);
  WebServer.on("/laseron", HTTP_GET, laser_on);
  WebServer.on("/laseroff", HTTP_GET, laser_off);
  WebServer.on("/laserflash", HTTP_GET, handle_laser_flash);

  mcp.begin ();
  //pinMode(GPA5, OUTPUT);
  //pinMode(GPA6, OUTPUT);
  //pinMode(GPB5, OUTPUT);
  mcp.pinMode(GPB6, OUTPUT);
}

void loop()
{
  handleESPEssentials();
  Debug.handle();
}
