// wifi_config.h

#ifndef _WIFI_CONFIG_h
#define _WIFI_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initWifi();
extern bool StartAPMode();

#endif

