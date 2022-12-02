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
  randomSeed(analogRead(0));

  wilhelmiCar.begin();

  Serial.println("Connecting to wifi");
  if (digitalRead(ACCESS_POINT_SELECT) == HIGH)
    webApi.setupAP(SSID_AP, PWD_AP);               // wifi server
  else
    webApi.connectToWiFiClient(Client_SSID.c_str(), Client_PWD.c_str());   // Wifi client

  // start web api routing
  webApi.setup_routing();
}

// arduino loop function
void loop() {
  webApi.handleClient();
  wilhelmiCar.loop();
}
