//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _BUZZER_h
#define _BUZZER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "pwm_out.h"
#include "user_config.h"

extern void initBuzzer(uint16_t duration);
extern void deinitBuzzer();

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration);
void noTone(uint8_t _pin);

#endif
