//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _FILE_CONFIG_h
#define _FILE_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "user_config.h"
#include "setting.h"
#include <EEPROM.h>

extern bool loadConfig();

#endif
