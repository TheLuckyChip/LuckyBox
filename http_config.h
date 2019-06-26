//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _HTTP_CONFIG_h
#define _HTTP_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "file_config.h"
#include "time_config.h"
#include "tft.h"

extern void initHTTP(void);
extern void handleSetSSDP();
extern void handleSetSSID();
extern void handleSetSSIDAP();
extern void handleRestart();
extern void handleSetRotate();
extern void handleSetVolume();
extern void handleConfigJSON();

#endif
