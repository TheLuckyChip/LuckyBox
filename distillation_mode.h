// distillation.h

#ifndef _DISTILLATION_h
#define _DISTILLATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

extern void initDistillation();
extern void handleSetTempTank();
extern void handleDistillationJSON();
extern void distillationLoop();
#endif

