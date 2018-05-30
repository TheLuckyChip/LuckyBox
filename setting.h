// setting.h

#ifndef _SETTING_h
#define _SETTING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ticker.h>
#include <Adafruit_PWMServoDriver.h>
#include "user_config.h"

typedef struct DS_Str
{
	byte  addr[8];
	float temperature;
};

extern Ticker tickerSet;

extern Adafruit_PWMServoDriver pwm;

#include <SPIFFS.h>
#include <ESP32HTTPUpdateServer.h>
#include <ESP32WebServer.h>

// Объект для обновления с web страницы 
extern ESP32HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
extern ESP32WebServer HTTP;

// Для файловой системы
extern File fsUploadFile;

// Определяем переменные wifi
extern String _ssid;
extern String _password;
extern String _ssidAP;
extern String _passwordAP;
extern String SSDP_Name;
extern String jsonConfig;
extern int port;
extern int timezone;
extern int DS_Count;
extern struct DS_Str dallas_my_sensor[DS_Cnt];
extern float pressure;
extern bool  pressureStatus;
extern unsigned long displayTimeInterval;
extern unsigned long sensorTimeRead;
extern unsigned long touchTimeRead;
extern bool settingAlarm;
extern int modeWiFi;
extern int timeWiFiReconnect;
extern uint8_t DefCubOut;
extern int16_t touch_x;
extern int16_t touch_y;
extern bool touch_in;

extern void csOff(uint8_t ch);
extern void csOn(uint8_t ch);

#endif
