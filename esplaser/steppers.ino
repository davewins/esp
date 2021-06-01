

//Handle HTTP Requests
//Home the Stepper Moters!
void homing() {
  //mcp.digitalWrite(LASERPIN, LOW);
  Debug.println("Homing the stepper motors");
  const char* response="{\"ResponseBody\":\"Stepper motors homed\"}";
  WebServer.send(200, "application/json", response);
}
