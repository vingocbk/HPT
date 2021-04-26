 #include "AppDebug.h"
#include <Arduino.h>
#include "WiFi.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"
#include <PubSubClient.h>

#include "soc/soc.h"  //Brownout detector was triggered
#include "soc/rtc_cntl_reg.h"


#define hallSensor1 14
#define hallSensor2 27  
#define hallSensor3 26
#define hallSensor4 4
#define hallSensor5 16
#define hallSensor6 17
#define ledTestWifi 32
#define inputFG 21
#define PWM 18
#define DIR 19
#define ANALOG_READ_BUTTON 34
#define PIN_CONFIG 25
#define PIN_TOUCH_SENSOR T5
#define PIN_LED_LIGHT_R 22
#define PIN_LED_LIGHT_G 5
#define PIN_LED_LIGHT_B 23
#define LED_CHANNEL_R 1
#define LED_CHANNEL_G 2
#define LED_CHANNEL_B 3
#define AlphaLed 1

// old motor
// #define QUAY_THUAN HIGH
// #define QUAY_NGHICH LOW

  // new motor
 #define QUAY_THUAN LOW
 #define QUAY_NGHICH HIGH

#define WL_MAC_ADDR_LENGTH 6
#define MINSPEED 5

#define RESPONSE_LENGTH 2048     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START 0
#define EEPROM_WIFI_SSID_END 32
#define EEPROM_WIFI_PASS_START 33
#define EEPROM_WIFI_PASS_END 64
#define EEPROM_WIFI_DEVICE_ID 65
#define EEPROM_WIFI_SERVER_START 66
#define EEPROM_WIFI_SERVER_END 128
#define EEPROM_SET_MODE_RUN_BEGIN 149
#define EEPROM_DISTANT 150
#define EEPROM_SET_PERCENT_OUT_LOW_SPEED 151
#define EEPROM_SET_PERCENT_IN_LOW_SPEED 152
#define EEPROM_SET_TIME_RETURN 153
#define EEPROM_WIFI_LED_RED 154
#define EEPROM_WIFI_LED_GREEN 155 
#define EEPROM_WIFI_LED_BLUE 156
#define EEPROM_WIFI_IS_TOUCH_SENSOR 157
#define EEPROM_DELAY_MQTT_TO_LOW_SPEAKER 158
#define EEPROM_COUNT_TIME_ANALOG 159
#define EEPROM_VALUE_ERROR_ANALOG 160
#define EEPROM_TIME_AUTO_CLOSE 161
#define EEPROM_DISTANT_OFF_LED_QUEEN 162
#define EEPROM_WIFI_MAX_CLEAR 256


#define m_Getstatus "/getstatus"
#define m_Control "/control"
#define m_Controlhand "/controlhand"
#define m_Resetdistant "/resetdistant"
#define m_Ledrgb "/ledrgb"
#define m_Setmoderun "/setmoderun"
#define m_Settimereturn "/settimereturn"
#define m_Setlowspeed "/setlowspeed"
#define m_Typedevice  "motor"



#define SSID_PRE_AP_MODE "AvyInterior-"
#define PASSWORD_AP_MODE "123456789"

 
#define HTTP_PORT 80
#define MQTT_PORT 1883
// #define MQTT_PORT 13347

#define CONFIG_HOLD_TIME 5000
#define TIME_CHECK_ANALOG 500       //500ms check 1 lan
// #define VALUE_ERROR_ANALOG 300

#define TIME_CHECK_TOUCH_SENSOR 100
#define VALUE_ERROR_TOUCH_SENSOR 10

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


int countPulFG = 0;             //bien nay dung de dem van toc
int countPulFGDistant = 0;      //bien nay dem khoang cach tai thoi diem hien tai.
uint8_t setmoderunbegin = 1; // nhanh nhat
uint8_t setmoderunbeginchange = 1;      //bien nay co tac dung thay doi toc do cham dan
int countPulDistant;            //bien nay la tong do dai khoang cach                  
uint8_t countSetPwm = 0;        //bien nay co tac dung dieu chinh toc do dong co
uint32_t countTime = 0;          //tinh thoi gian, timer 2 la 10us, vi the 100ms bien nay se co gia tri la 10.000
uint8_t sau_1_s = 0;
uint8_t timecaculateSpeed = 0;  //sau 500ms moi bat dau tinh van toc
int pul, prepul = 0;
int percentLowSpeedIn = 20;          //gia tri set mac dinh ban dau la 15% moi dau.
int percentLowSpeedOut = 20;
float speed;
bool statusStop = true;     //true la he thong dang dung im, false la he thong dang chuyen dong
bool countHall1 = false, countHall2 = false, countHall3 = false, countHall4 = false, countHall5 = false, countHall6 = false;
bool modeFast = false;
bool Forward = true;    //chieu quay thuan
bool fristRun = true;   //kiem tra lan chay dau tinh khoang cach chieu dai tu
bool daytay = true;
bool Flag_Normal_Mode = true;
bool isSaveDistant = false;
bool isSavePercentLowSpeed = false;
bool flag_send_status_when_use_hand = false;
bool flag_reset_value_analog = false;
bool flag_is_touch_sensor = false;
int start_count_hall_sensor = 0;    //bat dau den 2 thi moi tinh day tay
int count_number_of_click = 0;      //click 5 lan thi chuyen mode
uint8_t countFrirstRun = 0;  //dem so lan va cham
uint32_t count_stop_motor = 0;
uint32_t time_click_button = 0;
uint32_t time_check_analog_pin = 0;   //500ms thi check analog 1 lan
int count_check_analog_pin = 0;   //dung de set thoi gian delay, tranh nhay qua, 1 tuong ung voi 100ms
uint32_t time_check_touch_sensor = 0;
uint32_t pre_time_click_button = 0;
int deviceId;
uint32_t countDisconnectToServer = 0;
unsigned long count_time_disconnect_to_sever = 0;
// unsigned long time_delay_reconnect_wifi = 0;
bool flag_disconnect_to_sever = false;
unsigned long sum_time_disconnect_to_sever = 0;
unsigned long lastReconnectAttempt = 0;
unsigned long lastClientMqttLoop = 0;
unsigned long timeToSaveData;
bool flagIsHaveChangeColorHand = false;
uint8_t delay_mqtt_loop = 0;
int distantOffLedQueen = 10;

String esid, epass, sever;
int stop_dau = 0;           //gia tri set cho khoang dung lai
int stop_cuoi = 0;
int time_return = 50;        //50 *6ms;
int loai_bien_giong_nhau_cua_cam_bien = 0;
int luu_trang_thai_cua_sensor_ngay_khi_dung_lai = 0;

int defineTimeAutoClose = 0;      //sau bao nhieu phut thi se tu dong dong lai  
unsigned long timeAutoClose = 0;
bool flagAutoClose = false;

int valueAnalogRead = 0;
int prevalueAnalogRead = 0;
int valueErrorAnalogRead = 0;
uint16_t valueTouchSensorFix = 0;			//gia tri load touch sensor sau moi lan reset
uint16_t valueTouchSensor = 0;				//gia tri load touch sensor lien tuc
int countLedLightRed_After = 0, countLedLightGreen_After = 0, countLedLightBlue_After = 0;						//bien cho den sang tu tu khi mo tu
int countLedLightRed_Before = 0, countLedLightGreen_Before = 0, countLedLightBlue_Before = 0;
int countChangeLed = 0;
bool statusLed = true;
//normal mode
void getStatus();
void SendStatusReconnect();
void IRAM_ATTR inputSpeed();
void IRAM_ATTR dirhallSensor1();
void IRAM_ATTR dirhallSensor2();
void IRAM_ATTR dirhallSensor3();
void IRAM_ATTR dirhallSensor4();
void IRAM_ATTR dirhallSensor5();
void IRAM_ATTR dirhallSensor6();
void loadDataBegin();
void setpwmMotor();
void tickerupdate();
void caculateSpeed();
bool testWifi(String esid, String epass);
void ConnecttoMqttServer();
void callbackMqttBroker(char* topic, byte* payload, unsigned int length);
bool reconnect();
void buttonClick();
void Open();
void Close();
void Stop();
void setpwmStopMotor();
void setPwmLedLighton();
void setPwmLedLightoff();
void setPwmLedLightChange();
void resetDistant();
void setTimeReturn();
void SetupNetwork();
void setSpeedControl();
void checkTouchSensor();
void checkAnalogReadButton();
void checkAutoClose();
//config mode
void setLedApMode();
void SetupConfigMode();             //phat wifi
void ConfigMode();                  //nhan data tu app
void StartConfigServer();           //thiet lap sever
String GetFullSSID();
void detachIP(String ip);
void setupIP();

///////////test xong se delete///////////////
unsigned long microsecondPwmmotor = 0;
bool printlnmicoros = true;
int co = 0;

Ticker tickerCaculateSpeed(caculateSpeed, 100);   //every 100ms
Ticker SetPWMspeed(setpwmMotor, 1, 0, MICROS_MICROS);  //MICROS_MICROS
Ticker SetPWMStopSpeed(setpwmStopMotor, 1, 0, MICROS_MICROS);
Ticker tickerSetApMode(setLedApMode, 200, 0);   //every 200ms
Ticker tickerSetPwmLedLightOn(setPwmLedLighton, 20, 255);	//every 20ms
Ticker tickerSetPwmLedLightOff(setPwmLedLightoff, 10, 255);
Ticker tickerSetPwmLedLightChange(setPwmLedLightChange, 10, 255);
