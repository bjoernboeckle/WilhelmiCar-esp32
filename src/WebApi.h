#ifndef WEB_API_H
#define WEB_API_H


#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

class WilhelmiCar;

class WebApi {
    public:
        WebApi(WilhelmiCar *car) : _car(car) { _gInst = this; }
        void setupAP(const char* ssid, const char* pwd);
        void connectToWiFiClient(const char* ssid, const char* pwd);
        void setup_routing();
        void handleClient();

        static WebApi *_gInst;
   
        void getIndexHtml(HTTPRequest * req, HTTPResponse * res);
        void apiGetEnv(HTTPRequest * req, HTTPResponse * res);
        void handleGetAutoModeSettigs(HTTPRequest * req, HTTPResponse * res);
        void handlePostMove(HTTPRequest * req, HTTPResponse * res);
        void handlePostSetSettings(HTTPRequest * req, HTTPResponse * res);
        void handlePostStartAutoMode(HTTPRequest * req, HTTPResponse * res);
        void handleActivateHorn(HTTPRequest * req, HTTPResponse * res);

private:
        WilhelmiCar *_car;
        void AddCORS(HTTPResponse * res);
};


#endif