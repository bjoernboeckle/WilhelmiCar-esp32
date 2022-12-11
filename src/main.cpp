#include <Arduino.h>
#include "defines.h"
#include "WilhelmiCar.h"
#include "WebApi.h"


// client SSID and password (configurable by connecting to AP and set using the app)
String Client_SSID = "NoSSID";
String Client_PWD  = "NoPassword";


WilhelmiCar wilhelmiCar;
WebApi webApi(&wilhelmiCar);


// arduino setup function
void setup() {
  Serial.begin(921600);
  Serial.print("Starting up ");
  Serial.println(VERSION);
  
  delay(200);
  wilhelmiCar.begin();
  delay(500);

  Serial.println("Connecting to wifi");
  if (digitalRead(ACCESS_POINT_SELECT) == HIGH)
    webApi.setupAP(SSID_AP, PWD_AP);               // wifi server
  else
    webApi.connectToWiFiClient(Client_SSID.c_str(), Client_PWD.c_str());   // Wifi client

  // start web api routing
  webApi.setup_routing();
}



//long timer = 0;
// arduino loop function
void loop() {
  webApi.handleClient();
  wilhelmiCar.loop();

  /*if (micros() - timer >1000000 )
  {
    Serial.print("Distance: ");
    Serial.print(wilhelmiCar.carState.Distance);
    Serial.print(" / ");
    Serial.print(wilhelmiCar.carState.DistanceLeft);
    Serial.print(" / ");
    Serial.println(wilhelmiCar.carState.DistanceRight);    
    timer = micros();
  }*/
}
