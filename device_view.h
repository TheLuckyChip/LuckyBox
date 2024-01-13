//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _DEVICE_VIEW_h
#define _DEVICE_VIEW_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include <time.h>
#include "tft.h"
#include "pwm_out.h"
#include "misc.h"
//#include "my_eeprom.h"

extern void deviceViewLoop();

#endif

