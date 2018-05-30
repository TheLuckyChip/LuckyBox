// time_config.h

#ifndef _TIME_CONFIG_h
#define _TIME_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initTime();
extern void timeSynch(int zone);
extern void handleTimeZone();
extern void handleTime();
extern String GetTime();
extern String GetDate();

#endif

