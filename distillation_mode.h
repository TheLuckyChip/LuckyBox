//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _DISTILLATION_h
#define _DISTILLATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "user_config.h"
#include "time_config.h"
#include "pwm_out.h"
#include "tft.h"
#include <EEPROM.h>

//extern void loadEepromDistillation();
extern void initDistillation();
extern void handleDistillationTpl();
extern void handleDistillationSensorSetLoad();
extern void handleDistillationSensorSetSave();
extern void distillationLoop();
#endif
