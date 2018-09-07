//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _PID_CONFIG_h
#define _PID_CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "setting.h"
#include "user_config.h"
#include "mashing_mode.h"
#include <time.h>

extern void initPID();
extern void loadEepromPid();
extern void handlePidSet();
extern void pidSetLoop();

#endif
