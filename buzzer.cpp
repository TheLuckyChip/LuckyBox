#include "buzzer.h"

void initBuzzer(uint16_t duration)
{
	tickerSet.attach_ms(duration, deinitBuzzer);
	setPWM(BUZ_OUT, 0, 2048);
	// включить канал для сирены
	csOn(SIREN_OUT);
}

void deinitBuzzer()
{
	tickerSet.detach();
	setPWM(BUZ_OUT, 0, 4096);
	// выключить канал для сирены
	csOff(SIREN_OUT);
}
