//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _DISPLAY_h
#define _DISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "sensors.h"
#include "tft.h"
#include "wifi_config.h"
#include "setting.h"
#include "user_config.h"
#include "buzzer.h"
#include "touch_interrupt.h"

extern void displayLoop();
extern void touchLoop();

#endif
