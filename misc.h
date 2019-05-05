//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _MISC_h
#define _MISC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"

extern float EEPROM_float_read(int addr);
extern void EEPROM_float_write(int addr, float val);
extern void stop_Err();
extern void check_Err();
extern void serialLoop();
extern void stepApLoop();

#endif
