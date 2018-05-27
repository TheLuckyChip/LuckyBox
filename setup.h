// setup.h

#ifndef _SETUP_h
#define _SETUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void setup();

#endif

