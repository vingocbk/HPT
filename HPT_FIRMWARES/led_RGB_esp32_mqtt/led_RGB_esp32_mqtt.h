#include <Arduino.h>
#include "AppDebug.h"
#include <Arduino.h>
#include "WiFi.h"
#include <WiFiClient.h>
#include "WebServer.h"
#include "ESPmDNS.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"
#include <PubSubClient.h>

#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"

#define LED_TEST        32     
#define PIN_CONFIG      0       
#define PIN_LED_RED     23      
#define PIN_LED_GREEN   25   
#define PIN_LED_BLUE    22

#define PIN_LED_LIGHT_R 23
#define PIN_LED_LIGHT_G 25
#define PIN_LED_LIGHT_B 22
#define LED_CHANNEL_R 1
#define LED_CHANNEL_G 2
#define LED_CHANNEL_B 3

#define WL_MAC_ADDR_LENGTH 6
#define RESPONSE_LENGTH 2048     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 32
#define EEPROM_WIFI_PASS_START 33
#define EEPROM_WIFI_PASS_END 64
#define EEPROM_WIFI_DEVICE_ID 65
#define EEPROM_WIFI_SERVER_START 66
#define EEPROM_WIFI_SERVER_END 128
#define EEPROM_WIFI_LED_RED 150
#define EEPROM_WIFI_LED_GREEN 151
#define EEPROM_WIFI_LED_BLUE 152
#define EEPROM_WIFI_LED_INTENSITY 153
#define EEPROM_WIFI_LED_STATUS_ON_OFF 154
#define EEPROM_WIFI_MAX_CLEAR 512

#define m_Getstatus "/getstatus"
#define m_Controlvoice "/control"
#define m_Controlhand "/controlhand"
#define m_Resetdistant "/resetdistant"
#define m_Ledrgb "/ledrgb"
#define m_Setmoderun "/setmoderun"
#define m_Settimereturn "/settimereturn"
#define m_Setlowspeed "/setlowspeed"
#define m_Typedevice  "ledRgb"


#define SSID_PRE_AP_MODE "AvyInterior-"
#define PASSWORD_AP_MODE "123456789"

#define HTTP_PORT 80
#define MQTT_PORT 1883

#define CONFIG_HOLD_TIME 5000

// const char* mqtt_server = "test.mosquitto.org";
// const char* mqtt_server = "iot.eclipse.org";
// const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "3.3.0.120";

const char* topicsendStatus = "CabinetAvy/HPT/deviceStatus";
const char* m_userNameServer = "avyinterial";
const char* m_passSever = "avylady123";
String m_Pretopic = "CabinetAvy/HPT";
WiFiClient espClient;
PubSubClient client(espClient);

WebServer server(HTTP_PORT);



// Ticker tickerSetMotor;
// Ticker tickerSpeed;
// Ticker tickerSetApMode;


//normal mode
void getStatus();
void ControlLed();
void ControlLedColor();
void ControlIntensity();
void SetupNomalMode();
void SetupNetwork();
void tickerupdate();

//Config Mode
void checkButtonConfigClick();      //kiem tra button
void SetupConfigMode();             //phat wifi
void StartConfigServer();           //thiet lap sever
void ConfigMode();                  //nhan data tu app
void setLedApMode();                //hieu ung led
String GetFullSSID();
bool testWifi(String esid, String epass);
unsigned long ConfigTimeout;
unsigned long timeToSaveData;
uint8_t red_before, red_after;
uint8_t green_before, green_after;
uint8_t blue_before, blue_after;
bool flag_led = true;
uint8_t intensityLight;
int deviceId;
uint32_t countDisconnectToServer = 0;
unsigned long count_time_disconnect_to_sever = 0;
// unsigned long time_delay_reconnect_wifi = 0;
bool flag_disconnect_to_sever = false;
unsigned long sum_time_disconnect_to_sever = 0;
String esid, epass, serverMqtt;
bool Flag_Normal_Mode = true;
// unsigned long Pul_Motor;
// unsigned long test_time, time_start_speed;


Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
