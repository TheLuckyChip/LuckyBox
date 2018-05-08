// http_config.h

#ifndef _HTTP_CONFIG_h
#define _HTTP_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initHTTP(void);
extern void handleSetSSDP();
extern void handleSetSSID();
extern void handleSetSSIDAP();
extern void handleRestart();
extern void handleConfigJSON();
extern String getConfigJSON();

#endif

