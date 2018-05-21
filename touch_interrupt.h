// touch_interrupt.h

#ifndef _TOUCH_INTERRUPT_h
#define _TOUCH_INTERRUPT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void touchInt();
extern void touchscreenUpdate();

#endif

