//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _PWM_OUT_h
#define _PWM_OUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "user_config.h"
#include "touch_interrupt.h"
#include "buzzer.h"

extern void csOff(byte ch);
extern void csOn(byte ch);
extern void setPWM(byte ch, uint16_t Pon, uint16_t Poff);
extern void servoAngleSet(byte ch, byte angle);

#endif
