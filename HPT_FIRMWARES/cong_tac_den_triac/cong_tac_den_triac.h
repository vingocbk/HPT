#include "Arduino.h"
#include "AppDebug.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"
//#include "DallasTemperature.h"
//#include "OneWire.h"
#include <PubSubClient.h>


#define LED_TEST_AP 2 // D4 onchip GPIO2
#define PIN_CONFIG D3       // D3 flash GPIO0
#define LIGHT_1 16     
#define LIGHT_2 14     
#define LIGHT_3 12     
#define BTN_INPUT_1 13
#define BTN_INPUT_2 5
#define BTN_INPUT_3 4

#define RESPONSE_LENGTH 2048     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 32
#define EEPROM_WIFI_PASS_START 33
#define EEPROM_WIFI_PASS_END 64
#define EEPROM_WIFI_DEVICE_ID_START 65
#define EEPROM_WIFI_DEVICE_ID_END 75
#define EEPROM_WIFI_ROOM_ID_START 76
#define EEPROM_WIFI_ROOM_ID_END 120
#define EEPROM_WIFI_SERVER_START 121
#define EEPROM_WIFI_SERVER_END 150

#define EEPROM_WIFI_MAX_CLEAR 512


#define SSID_PRE_AP_MODE "AvyInterior-"
#define PASSWORD_AP_MODE "123456789"

#define HTTP_PORT 80
#define MQTT_PORT 1883

#define CONFIG_HOLD_TIME 3000
#define TIME_RESEND 1000
#define TIME_KEEP_SENDING 5000

#define m_Getstatus "/getstatus"
#define m_Control "/control"
#define m_Typedevice  "lightonoff"

const char* topicsendStatus = "Light/HPT/deviceStatus";
const char* m_userNameServer = "avyinterial";
const char* m_passSever = "avylady123";
String m_Pretopic = "Light/HPT";
WiFiClient espClient;
PubSubClient client(espClient);


ESP8266WebServer server(HTTP_PORT);
 

unsigned long ConfigTimeout;
uint8_t first_octet;
uint8_t second_octet;
uint8_t third_octet;
uint8_t fourth_octet;
uint8_t red_before, red_after;
uint8_t green_before, green_after;
uint8_t blue_before, blue_after;
uint32_t Time = 0;

uint32_t countDisconnectToServer = 0;
unsigned long count_time_disconnect_to_sever = 0;
bool flag_disconnect_to_sever = false;
unsigned long sum_time_disconnect_to_sever = 0;
unsigned long lastReconnectAttempt = 0;
unsigned long lastClientMqttLoop = 0;
uint8_t delay_mqtt_loop = 0;

//int templateAfter, templateBefor;
uint32_t count_bt1 = 0;
uint32_t count_bt2 = 0;
uint32_t count_bt3 = 0;
uint8_t flag_input_btn1;
uint8_t flag_input_btn2;
uint8_t flag_input_btn3;
bool flag_send_status = false;
bool sub_device_id1 = false;
bool sub_device_id2 = false;
bool sub_device_id3 = false;


String deviceId, esid, epass, roomId, sever;
bool Flag_Normal_Mode = true;
// unsigned long Pul_Motor;
// unsigned long test_time, time_start_speed;

//normal mode
void handleOk();
void handleRoot();
void getStatus();
void SendStatusReconnect();
void SetupNomalMode();
void SetupNetwork();
void tickerupdate();


//Config Mode
void SetupConfigMode();             //phat wifi
void StartConfigServer();           //thiet lap sever
void ConfigMode();                  //nhan data tu app
void setLedApMode();                //hieu ung led
String GetFullSSID();
bool testWifi(String esid, String epass);
void ConnecttoMqttServer();
void callbackMqttBroker(char* topic, byte* payload, unsigned int length);
bool reconnect();


Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
