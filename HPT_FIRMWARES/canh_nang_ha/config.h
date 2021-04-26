/*
Design by Ngoc Vi
Ha Noi 04/2021
*/

#ifndef CONFIG_H
#define CONFIG_H

#define RF0                 5
#define RF1                 14
#define RF2                 4
#define PIN_LOA             13
#define PIN_MODE            16      //MODE HOLD BUTTON OR CLICK BUTTON
#define PIN_CONFIG          0
#define L1                  17
#define L2                  18
#define L3                  19
#define L4                  21
#define BUTTON_UP           26
#define BUTTON_DOWN         27
#define PIN_LED_LIGHT_B     22
#define PIN_LED_LIGHT_R     23
#define PIN_LED_LIGHT_G     25
#define I1IN                34
#define I2IN                35
#define ledTestWifi         32

#define RESPONSE_LENGTH             2048     //do dai data nhan ve tu tablet
#define EEPROM_WIFI_SSID_START      0
#define EEPROM_WIFI_SSID_END        32
#define EEPROM_WIFI_PASS_START      33
#define EEPROM_WIFI_PASS_END        64
#define EEPROM_WIFI_DEVICE_ID       65
#define EEPROM_WIFI_SERVER_START    66
#define EEPROM_WIFI_SERVER_END      128
#define EEPROM_MOTOR_1_UP           129
#define EEPROM_MOTOR_2_UP           130
#define EEPROM_MOTOR_1_DOWN         131
#define EEPROM_MOTOR_2_DOWN         132
#define EEPROM_WIFI_LED_RED         154
#define EEPROM_WIFI_LED_GREEN       155 
#define EEPROM_WIFI_LED_BLUE        156
#define EEPROM_WIFI_IS_REGISTER     157
#define EEPROM_WIFI_MAX_CLEAR       256

#define CONFIG_HOLD_TIME 5000
#define MQTT_PORT 1883
#define HTTP_PORT 80

#define m_Getstatus "/getstatus"
#define m_Control "/control"
#define m_Controlhand "/controlhand"
#define m_Resetdistant "/resetdistant"
#define m_Ledrgb "/ledrgb"
#define m_Setmoderun "/setmoderun"
#define m_Settimereturn "/settimereturn"
#define m_Setlowspeed "/setlowspeed"
#define m_Typedevice  "motor"




#endif
