//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _TIME_CONFIG_h
#define _TIME_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "file_config.h"
#include "setting.h"
#include <ESP8266WiFi.h>
#include <time.h>

extern void initTime();
extern void timeSynch(int zone);
extern void handleTimeZone();
extern void handleTime();
extern String GetTime();
extern String GetDate();

#endif
