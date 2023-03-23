//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _BREWING_h
#define _BREWING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "heater.h"
#include "setting.h"
#include "pwm_out.h"
#include "tft.h"
#include <PID_v1.h>

extern void initBrewing();
extern void handleSettingBrewing();
extern void handleBrewingJSON();
extern void brewingLoop();

#endif
