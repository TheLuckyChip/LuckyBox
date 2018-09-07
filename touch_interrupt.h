//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _TOUCH_INTERRUPT_h
#define _TOUCH_INTERRUPT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "user_config.h"
#include "setting.h"
#include "pwm_out.h"
#include "SPI.h"

#define SPI_SETTING_LOW     SPISettings(2000000, MSBFIRST, SPI_MODE0)

extern void touchscreenUpdate();
extern void touchscreenUpdateSet();

#endif
