//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _DALL_READ_h
#define _DALL_READ_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "user_config.h"
#include "setting.h"
#include "buzzer.h"
#include "tft.h"
#include "wifi_config.h"
#include <EEPROM.h>
#include <OneWire.h>

#if defined Pressure_BMP085 || defined Pressure_BMP180
#include "Adafruit_BMP085.h"
extern Adafruit_BMP085 bmp;
#elif defined Pressure_BMP280
#include <Adafruit_BMP280.h>
extern Adafruit_BMP280 bmp;
#elif defined Pressure_BME280
#include <Adafruit_BME280.h>
extern Adafruit_BME280 bmp;
#endif

extern OneWire ds;
extern void initPressureSensor();
extern void pressureRead();
extern void senseWebInit();
extern void dallSearch();
extern void dallRead();
extern void sensorsUserSetInWeb();
extern void sensorsUserSetOutWeb();
extern void handleProcessSensorOut();
extern void handleProcessModeIn();
extern void handleResetDataEeprom();
extern void sensorLoop();

#endif
