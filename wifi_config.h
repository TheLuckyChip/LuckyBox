// wifi_config.h

#ifndef _WIFI_CONFIG_h
#define _WIFI_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initWifi();
extern void reconnectWiFi(int tCnt);

#endif

