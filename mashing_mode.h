//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _MASHING_MODE_h
#define _MASHING_MODE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "heater.h"
#include "setting.h"
#include "pwm_out.h"
#include "tft.h"
#include <time.h>
#include <PID_v1.h>

//extern void loadEepromMashing();
extern void initMashing();
extern void loadEepromMashing();
extern void handleMashingSensorTpl();
extern void handleMashingSensorSetLoad();
extern void handleMashingSensorSetSave();
extern void mashingLoop();

#endif

