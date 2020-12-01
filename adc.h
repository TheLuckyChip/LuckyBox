//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _ADC_h
#define _ADC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include <Adafruit_ADS1015.h>

extern void adcInit();
extern void adcLoop();

#endif
