// reflux.h

#ifndef _REFLUX_h
#define _REFLUX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern float settingColumn;
extern float temperatureStartPressure;
extern float settingColumnShow;
extern float temperatureAlcoholBoil;

extern void initReflux();
extern void handleSetTemp();
extern void handleRefluxJSON();
extern void refluxLoop();

#endif

