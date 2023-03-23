//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _REFLUX_h
#define _REFLUX_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "heater.h"
#include "setting.h"
#include <time.h>
#include "tft.h"
#include "pwm_out.h"
#include <EEPROM.h>
#include "misc.h"
#include "touch_interrupt.h"

extern void loadEepromReflux();
extern void initReflux();
extern void handleRefluxSensorTpl();
extern void handleRefluxSensorSetLoad();
extern void handleRefluxSensorSetSave();
extern void refluxLoop();

#endif
