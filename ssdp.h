//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _SSDP_h
#define _SSDP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include <ESP8266SSDP.h> 

extern void initSSDP(void);

#endif
