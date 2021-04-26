#ifndef webserver_h
#define webserver_h

#include "EEPROM.h"
#include "AppDebug.h"
#include "config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>



struct configweb
{
	/* data */
	bool is_receive_data;
    bool is_reset_distant;
	bool is_receive_wifi;
	bool is_reset_wifi;
};

void handleRoot(AsyncWebServerRequest *request);
void handleMotor1Up(AsyncWebServerRequest *request);
void handleMotor2Up(AsyncWebServerRequest *request);
void handleMotor1Down(AsyncWebServerRequest *request);
void handleMotor2Down(AsyncWebServerRequest *request);
void handleConfigWifi(AsyncWebServerRequest *request);
void handleResetWifi(AsyncWebServerRequest *request);
void notFound(AsyncWebServerRequest *request);

#endif
