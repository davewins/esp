
//Handle HTTP Requests
//Turn on the Laser!
void laser_on() {
  laseron();
  const char* response="{\"ResponseBody\":\"Laser On\"}";
  WebServer.send(200, "application/json", response);
}

//Turn off the Laser!
void laser_off() {
  laseroff();
  const char* response="{\"ResponseBody\":\"Laser Off\"}";
  WebServer.send(200, "application/json", response);
}


void handle_laser_flash() {
  int timespersecond = (WebServer.arg("timespersecond").toInt());
  int totalseconds = (WebServer.arg("totalseconds").toInt());
  const char* response="{\"ResponseBody\":\"Laser Flashing\"}";
  WebServer.send(200, "application/json", response);
  flashlaser(timespersecond,totalseconds);
}

void handle_laser_read() {
  laseron();
  int laservalue = analogRead(LASERDETECTOR);
  Serial.printf("Static Laser Value: %d ",laservalue); 
  delay(1000);
  laseroff();
  const char* response="{\"ResponseBody\":\"Laser Reading\"}";
  WebServer.send(200, "application/json", response);
}


//The Actual functions
void laseron() {
  mcp.digitalWrite(LASERPIN, HIGH);
  Debug.println("Laser ON");
}

void laseroff() {
  mcp.digitalWrite(LASERPIN, LOW);
  Debug.println("Laser OFF");
}

void flashlaser(int timespersecond, int totalseconds) {
  Debug.printf("Flashing Laser %d times per second for %d seconds\n",timespersecond,totalseconds);

  // Flash Laser for 'totalseconds'
  // timer subtraction works correctly even when micros() wraps (every 71 mins)
  unsigned long tstart, t1, tyield, t;
  tstart = tyield = t1 = micros();
  bool laserison=false;
  //Debug.printf("About to start loop %d\n",tstart);
  while (micros() - tstart < totalseconds * 1000000) {
    t = micros();
    if ( t - t1 >= (1000000 / timespersecond) / 2) {
      if (!laserison) {
        laseron();
        laserison = true;
      }
      else {
        laseroff();
        laserison = false;       
      }
      t1 = t;
    };

    // Run 'housekeeping' tasks (e.g., WiFi) every 0.5 sec
    if (t - tyield >= 500000) {
      Debug.println("yeilding");
      tyield = t;
      yield();
    };
    // Wait upto 0.0002 sec before next iteration (max speed=5000 steps/sec)
    //if (micros() - t < 200) delayMicroseconds(200 - (micros() - t));
    
  };
  //Debug.printf("out of loop after %d seconds \n", micros()*1000000);
  laseroff(); // Turn Laser Off
}
