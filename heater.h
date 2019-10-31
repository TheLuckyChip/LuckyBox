//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _HEATER_h
#define _HEATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "user_config.h"
#include "setting.h"

extern int heaterPower;
extern bool heaterStatus;

extern void heaterLoop();
extern void initHeater();
extern void comHeaterLoop();
extern void wifiHeaterLoop();

#endif
