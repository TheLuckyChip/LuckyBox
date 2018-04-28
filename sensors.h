#ifndef _DALL_READ_h
#define _DALL_READ_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>
#include "user_config.h"

extern OneWire ds;
typedef struct DS_Str
{
	byte  addr[8];
	float temperature;
};


extern void dallSearch();
extern void dallRead();

#endif

