// brewing.h

#ifndef _BREWING_h
#define _BREWING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initBrewing();
extern void handleSettingBrewing();
extern void handleBrewingJSON();
extern void brewingLoop();

#endif

