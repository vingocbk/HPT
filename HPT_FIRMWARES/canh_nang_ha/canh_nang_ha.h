#include <Arduino.h>
#include "AppDebug.h"
#include "Ticker.h"
#include "config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include "ArduinoJson.h"
#include "webserver.h"
#include <PubSubClient.h>
#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"


WiFiClient espClient;
PubSubClient client(espClient);

#define WL_MAC_ADDR_LENGTH      6
#define SSID_PRE_AP_MODE        "AvyInterior-"
#define PASSWORD_AP_MODE        "123456789"
enum {
    MOTOR_UP,
    MOTOR_DOWN,
    MOTOR_STOP
};

String m_Pretopic = "CabinetAvy/HPT";


const int freq = 5000;
const int L1UpChannel = 0;
const int L1DownChannel = 1;
const int L2UpChannel = 2;
const int L2DownChannel = 3;
const int LED_CHANNEL_R = 4;
const int LED_CHANNEL_G = 5;
const int LED_CHANNEL_B = 6;
const int resolution = 8;

int Max_L1_UP = 255;
int Max_L1_DOWN = 255;
int Max_L2_UP = 255;
int Max_L2_DOWN = 255;

int Status_Motor;

int countChangeLed = 0;
uint32_t time_click_button_config;
uint32_t timeToSaveData;
bool is_wifi_mode = false;
bool statusLed;
bool flagIsHaveChangeColorHand = false;
//int errorCrrent = 0;
//void Current();
//Ticker tickerCurrent(Current, 50);   //every 50ms
String ssid, pass, sever;
int device_id;

uint8_t countLedLightRed_After, countLedLightGreen_After, countLedLightBlue_After;
uint8_t countLedLightRed_Before, countLedLightGreen_Before, countLedLightBlue_Before;

void checkButtonUpDownClick();
void checkButtonConfigClick();
void SetupConfigMode();
void StartWebServer();
void ReadEeprom();
void ReadEeprom();
bool testWifi(String esid, String epass);
void ConnecttoMqttServer();
void callbackMqttBroker(char* topic, byte* payload, unsigned int length);
void getStatus();
void setLedApMode();
void setPwmLedLighton();
void setPwmLedLightoff();
void setPwmLedLightChange();

Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
Ticker tickerSetPwmLedLightOn(setPwmLedLighton, 20, 255);	//every 20ms
Ticker tickerSetPwmLedLightOff(setPwmLedLightoff, 10, 255);
Ticker tickerSetPwmLedLightChange(setPwmLedLightChange, 10, 255);
