#include <Arduino.h>
#include <WiFi.h>


//!!!!!!!!!!!!!!!
//    change   in HTTPConnection.hpp
//    #include <hwcrypto/sha.h>   -->    #include <esp32/sha.h>  
//!!!!!!!!!!!!!!!


//https://github.com/bblanchon/ArduinoJson
#include <ArduinoJson.h>

// TLS: Header file for the server. Note that we use the ...Secure.hpp version
//https://github.com/fhessel/esp32_https_server.git
// TLS: We need the private key and certificate from the external files
// WARNING: NEVER USE THE FILES FROM THE EXAMPLE FOR MORE THAN TESTING.
//          AS THE PRIVATE KEY IS PART OF THE REPOSITORY, THIS MEANS YOU
//          HAVE BASICALLY THE SAME LEVEL OF SECURITY AS WITH PLAIN
//          HTTP.
//          See the repository of the underlying esp32_https_server for
//          details on how to generate your own:
//          https://github.com/fhessel/esp32_https_server/tree/master/extras#create_certsh

// Includes for the server
// Note: We include HTTPServer and HTTPSServer
#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

#include "cert.h"
#include "private_key.h"

// The HTTPS Server comes in a separate namespace. For easier use, include it here.
using namespace httpsserver;


#include "WebApi.h"
#include "WilhelmiCar.h"


// JSON data buffer
static WilhelmiJson jsonDocument;
static char buffer[1024];

// Create an SSL certificate object from the files included above
static SSLCert cert = SSLCert(
  example_der, example_der_len,
  example_key_der, example_key_der_len
);

// First, we create the HTTPSServer with the certificate created above
HTTPSServer secureServer = HTTPSServer(&cert);
// Additionally, we create an HTTPServer for unencrypted traffic
HTTPServer insecureServer = HTTPServer();


WebApi *WebApi::_gInst(NULL);


void WebApi::AddCORS(HTTPResponse * res) {
  res->setHeader("Access-Control-Allow-Origin", "*");
  res->setHeader("Access-Control-Allow-Methods", "PUT, GET, POST, DELETE, PATCH, OPTIONS");
  res->setHeader("Access-Control-Allow-Headers", "*");
}


void WebApi::getIndexHtml(HTTPRequest * req, HTTPResponse * res) {
  Serial.println("Get Index Html");
  AddCORS(res);
  res->setHeader("Content-Type", "text/html");


  res->println("<!DOCTYPE html>");
  res->println("<html>");
  res->println("<head><title>Hello Wilhelmi!</title></head>");
  res->println("<body>");
  res->println("<h1>Hello Wilhelmi!</h1>");
  
  res->print("<p>Server is running for ");
  res->print((int)(millis()/1000), DEC);
  res->println(" seconds.</p>");
  
  if (req->isSecure()) {
    res->println("<p>You are connected via <strong>HTTPS</strong>.</p>");
  } else {
    res->println("<p>You are connected via <strong>HTTP</strong>.</p>");
  }
  
  res->println("</body>");
  res->println("</html>");
}


void WebApi::apiGetEnv(HTTPRequest * req, HTTPResponse * res) {
  jsonDocument.clear();

  jsonDocument["AutoPilotState"] = _car->autoPilot.GetState();  
  jsonDocument["PwmSpeed"] =  _car->carState.PwmSpeed;
  jsonDocument["PwmSteering"] = _car->carState.PwmSteering;
  jsonDocument["AutoStopByDistance"] = _car->carState.AutoStopByDistance;
  jsonDocument["PwmFrequency"] = _car->carState.PwmFrequency;  
  jsonDocument["Movement"] = _car->carState.Movement;
  jsonDocument["Speed"] = _car->carState.Speed;
  jsonDocument["Steering"] = _car->carState.Steering;
  jsonDocument["Distance"] = _car->carState.Distance;
  jsonDocument["DistanceLeft"] = _car->carState.DistanceLeft;
  jsonDocument["DistanceRight"] = _car->carState.DistanceRight;
  serializeJson(jsonDocument, buffer);

  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print(buffer);
}




void WebApi::handleGetAutoModeSettigs(HTTPRequest * req, HTTPResponse * res) {
  _car->autoPilot.AddAutoModeSettingsToJson(jsonDocument);
  serializeJson(jsonDocument, buffer);

  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print(buffer);
}


void WebApi::handlePostMove(HTTPRequest * req, HTTPResponse * res) {
  char body[1024];
  // Try to read request into buffer
  size_t idx = 0;
  // while "not everything read" or "buffer is full"
  while (!req->requestComplete() && idx < 500) {
    idx += req->readChars(body + idx, 500-idx);
  }
  deserializeJson(jsonDocument, body);
    
  
  // Get data
  MoveCommand newCmd;
  newCmd.Speed = jsonDocument["Speed"];
  newCmd.Steering = jsonDocument["Steering"];
  newCmd.Movement = jsonDocument["Movement"];

  _car->SendMoveCommand(newCmd);

  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print("{}");
}



void WebApi::handlePostSetSettings(HTTPRequest * req, HTTPResponse * res) {
  char body[1024];
  // Try to read request into buffer
  size_t idx = 0;
  // while "not everything read" or "buffer is full"
  while (!req->requestComplete() && idx < 500) {
    idx += req->readChars(body + idx, 500-idx);
  }

  deserializeJson(jsonDocument, body);

  Settings newSettings;
  newSettings.PwmSpeed = jsonDocument["PwmSpeed"];
  newSettings.PwmSteering = jsonDocument["PwmSteering"];
  newSettings.Client_SSID = jsonDocument["Client_SSID"].as<String>();
  newSettings.Client_PWD = jsonDocument["Client_PWD"].as<String>();
  newSettings.AutoStopByDistance = jsonDocument["AutoStopByDistance"];
  newSettings.PwmFrequency = jsonDocument["PwmFrequency"];

  _car->SetSettings(newSettings);

  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print("{}");
}


void WebApi::handlePostStartAutoMode(HTTPRequest * req, HTTPResponse * res) {
 char body[1024];
  // Try to read request into buffer
  size_t idx = 0;
  // while "not everything read" or "buffer is full"
  while (!req->requestComplete() && idx < 500) {
    idx += req->readChars(body + idx, 500-idx);
  }

  deserializeJson(jsonDocument, body);

  _car->autoPilot.SetAutoPilotSettings(jsonDocument);
  _car->autoPilot.StartAutoMode();
  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print("{}");
}



void WebApi::handleActivateHorn(HTTPRequest * req, HTTPResponse * res) {
  char body[1024];
  // Try to read request into buffer
  size_t idx = 0;
  // while "not everything read" or "buffer is full"
  while (!req->requestComplete() && idx < 500) {
    idx += req->readChars(body + idx, 500-idx);
  }

  deserializeJson(jsonDocument, body);  
  
  int frequency = jsonDocument["Frequency"];
  int duration = jsonDocument["Duration"];
  frequency = frequency == 0 ? 300 : frequency;
  duration = duration == 0 ? 500 : duration;

  Serial.print("Horn (Frequency: ");
  Serial.print(frequency);
  Serial.print(" Hz / Duration: ");
  Serial.print(duration);
  Serial.println("ms)");

  _car->ActivateHorn(frequency, duration);

  AddCORS(res);
  res->setHeader("Content-Type", "application/json");
  res->print("{}");
}

void WebApi::setupAP(const char* ssid, const char* pwd) {
  WiFi.disconnect();
  WiFi.softAP(ssid, pwd);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}



void WebApi::connectToWiFiClient(const char* ssid, const char* pwd) {
  WiFi.disconnect();

  Serial.print("Connecting to ");
  Serial.println(ssid);
    
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}


void WebApi::handleClient()
{
  //server.handleClient(); 
  secureServer.loop();
  insecureServer.loop();
}


void WebApi::setup_routing() {	 	 
  // For every resource available on the server, we need to create a ResourceNode
  // The ResourceNode links URL and HTTP method to a handler function
  ResourceNode * nodeRoot = new ResourceNode("/", "GET", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->getIndexHtml(req, res); }); // &WebApi::getIndexHtml);
  ResourceNode * nodeEnv  = new ResourceNode("/api/env", "GET", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->apiGetEnv(req, res); });
  ResourceNode * nodeAutoModeSettings  = new ResourceNode("/api/getAutoModeSettings", "GET", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->handleGetAutoModeSettigs(req, res); });
  ResourceNode * nodePostMove  = new ResourceNode("/api/move", "POST", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->handlePostMove(req, res); });
  ResourceNode * nodePostSetSettings  = new ResourceNode("/api/setSettings", "POST", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->handlePostSetSettings(req, res); });
  ResourceNode * nodePostStartAutoMode  = new ResourceNode("/api/startAutoMode", "POST", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->handlePostStartAutoMode(req, res); });
  ResourceNode * nodePostActivateHorn  = new ResourceNode("/api/activateHorn", "POST", [](HTTPRequest *req, HTTPResponse *res) { WebApi::_gInst->handleActivateHorn(req, res); });


  //secureServer.registerNode(nodeRoot);
  secureServer.registerNode(nodeEnv);
  secureServer.registerNode(nodeAutoModeSettings);
  secureServer.registerNode(nodePostMove);
  secureServer.registerNode(nodePostSetSettings);
  secureServer.registerNode(nodePostStartAutoMode);
  secureServer.registerNode(nodePostActivateHorn);
  

  //insecureServer.registerNode(nodeRoot);
  insecureServer.registerNode(nodeEnv);
  insecureServer.registerNode(nodeAutoModeSettings);
  insecureServer.registerNode(nodePostMove);
  insecureServer.registerNode(nodePostSetSettings);
  insecureServer.registerNode(nodePostStartAutoMode);
  insecureServer.registerNode(nodePostActivateHorn);

  secureServer.setDefaultNode(nodeRoot);
  insecureServer.setDefaultNode(nodeRoot);
  secureServer.start();
  insecureServer.start();

  if (secureServer.isRunning() && insecureServer.isRunning()) {
    Serial.println("Servers ready.");
  }
  else
    Serial.println("Servers failed???");

  //server.enableCORS();
}

