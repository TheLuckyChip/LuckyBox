#include "buzzer.h"
#include "setting.h"
#include "user_config.h"
#include <Ticker.h>

bool bounce = false;
Ticker tickerSet;

void setPin()
{
	if (settingAlarm)
	{
		digitalWrite(buzzer, bounce);
		bounce = !bounce;
	}
}

void initBuzzer()
{
	tickerSet.attach_ms(3, setPin);
}

