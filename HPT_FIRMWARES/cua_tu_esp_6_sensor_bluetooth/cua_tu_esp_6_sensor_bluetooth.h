#include "AppDebug.h"
#include <Arduino.h>
// #include "WiFi.h"
// #include "WebServer.h"
// #include "ESPmDNS.h"
#include "ArduinoJson.h"
#include "Ticker.h"
#include "EEPROM.h"
// #include <PubSubClient.h>
#include "BluetoothSerial.h"

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
//#define PIN_CONFIG 25
#define PIN_CONFIG 0
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
#define MINSPEED 5    //with small motor
//#define MINSPEED 15     //with big motor

#define RESPONSE_LENGTH 512     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_DEVICE_ID_START 1
#define EEPROM_WIFI_DEVICE_ID_END 32
#define EEPROM_IS_REGISTER 33
#define EEPROM_SET_MODE_RUN_BEGIN 34
#define EEPROM_DISTANT 35
#define EEPROM_SET_PERCENT_OUT_LOW_SPEED 36
#define EEPROM_SET_PERCENT_IN_LOW_SPEED 37
#define EEPROM_SET_TIME_RETURN 38
#define EEPROM_WIFI_LED_RED 39
#define EEPROM_WIFI_LED_GREEN 40 
#define EEPROM_WIFI_LED_BLUE 41
#define EEPROM_COUNT_TIME_ANALOG 42
#define EEPROM_VALUE_ERROR_ANALOG 43
#define EEPROM_TIME_AUTO_CLOSE 44
#define EEPROM_WIFI_MAX_CLEAR 256



#define SSID_PRE_AP_MODE "AvyConfig-"

 

#define CONFIG_HOLD_TIME 5000
#define TIME_CHECK_ANALOG 500       //500ms check 1 lan
// #define VALUE_ERROR_ANALOG 300

#define HTTP_PORT 80

// WebServer server(HTTP_PORT);
BluetoothSerial SerialBT;

int countPulFG = 0;             //bien nay dung de dem van toc
int countPulFGDistant = 0;      //bien nay dem khoang cach
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
uint8_t countFrirstRun = 0;  //dem so lan va cham
uint32_t count_stop_motor = 0;
uint32_t time_click_button = 0;
uint32_t time_check_analog_pin = 0;   //500ms thi check analog 1 lan
int count_check_analog_pin = 0;   //dung de set thoi gian delay, tranh nhay qua, 1 tuong ung voi 100ms
String esid;
bool isRegister;
unsigned long timeToSaveData;
bool flagIsHaveChangeColorHand = false;


int defineTimeAutoClose = 0;      //sau bao nhieu phut thi se tu dong dong lai  
unsigned long timeAutoClose = 0;
bool flagAutoClose = false;

int stop_dau = 0;           //gia tri set cho khoang dung lai
int stop_cuoi = 0;
int time_return = 50;        //50 *6ms;
int loai_bien_giong_nhau_cua_cam_bien = 0;
int luu_trang_thai_cua_sensor_ngay_khi_dung_lai = 0;

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
void IRAM_ATTR inputSpeed();
void IRAM_ATTR dirhallSensor1();
void IRAM_ATTR dirhallSensor2();
void IRAM_ATTR dirhallSensor3();
void IRAM_ATTR dirhallSensor4();
void IRAM_ATTR dirhallSensor5();
void IRAM_ATTR dirhallSensor6();
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void SetupNomalMode();
void loadDataBegin();
void setpwmMotor();
void tickerupdate();
void caculateSpeed();
void Open();
void Close();
void Stop();
void setpwmStopMotor();
void setPwmLedLighton();
void setPwmLedLightoff();
void setPwmLedLightChange();
void resetDistant();
void ClearData();
void setSpeedControl();
void checkAnalogReadButton();
void checkAutoClose();
//config mode
void setLedApMode();
void SetupConfigMode();             //phat wifi
void ConfigMode();                  //nhan data tu app
String GetFullSSID();

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
