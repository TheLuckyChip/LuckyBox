//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _WIFI_CONFIG_h
#define _WIFI_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "user_config.h"
#include "tft.h"
#include <ESP8266WiFi.h>

extern void initWifi();
extern void reconnectWiFi();

#endif
