// timer_config.h

#ifndef _TIMER_CONFIG_h
#define _TIMER_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void myTimer();
#endif

