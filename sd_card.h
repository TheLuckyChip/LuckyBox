//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _SD_CARD_h
#define _SD_CARD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "pwm_out.h"
#include "SD.h"
#include "tft.h"
#include "time.h"

extern void sdInit();
extern void logfileLoop();

#endif
