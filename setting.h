// setting.h

#ifndef _SETTING_h
#define _SETTING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Ticker.h>

extern Ticker flipper;

#ifdef ESP8266
#include <FS.h>
#include <ESP8266HTTPUpdateServer.h> 
#include <ESP8266WebServer.h>

// Объект для обновления с web страницы
extern ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
extern ESP8266WebServer HTTP;

#else
#include <SPIFFS.h>
#include <ESP32HTTPUpdateServer.h>
#include <ESP32WebServer.h>

// Объект для обновления с web страницы
extern ESP32HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
extern ESP32WebServer HTTP;

#endif

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
extern int modeWiFi;
extern int timeWiFiReconnect;

#endif

