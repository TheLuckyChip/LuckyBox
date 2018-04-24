// setting.h

#ifndef _SETTING_h
#define _SETTING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <FS.h>
#include <Ticker.h>
#include <ESP8266HTTPUpdateServer.h> 
#include <ESP8266WebServer.h>

extern Ticker flipper;

// Объект для обновления с web страницы 
extern ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
extern ESP8266WebServer HTTP;

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
extern float temperature1;
extern float temperature2;
extern float temperature3;
extern float temperature4;
extern float temperature5;
extern float temperature6;
extern float temperature7;
extern float temperature8;
extern float pressure;
extern bool  pressureStatus;
extern int readTempInterval;
extern bool settingAlarm;
extern int temp_in[120];

#endif

