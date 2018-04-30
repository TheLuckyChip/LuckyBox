// heater.h

#ifndef _HEATER_h
#define _HEATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern int heaterPower;
extern byte heaterStatus;

extern void heaterLoop();
extern void initHeater();
extern void handleSetHeaterPower();
extern void handleHeaterJSON();

#endif

