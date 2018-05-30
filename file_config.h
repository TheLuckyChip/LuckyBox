// file_config.h

#ifndef _FILE_CONFIG_h
#define _FILE_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern bool loadConfig();
extern bool saveConfig();

#endif

