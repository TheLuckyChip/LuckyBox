#include "buzzer.h"
#include "setting.h"
#include "user_config.h"

const uint8_t tone_timers[] = { 1 };
static uint8_t tone_pins[buzzer] = { 255, };
static long toggle_counts[buzzer] = { 0, };
#define T1INDEX 0

void t1IntHandler();

static int8_t toneBegin(uint8_t _pin) {
	int8_t _index = -1;

	// if we're already using the pin, reuse it.
	for (int i = 0; i < buzzer; i++) {
		if (tone_pins[i] == _pin) {
			return i;
		}
	}

	// search for an unused timer.
	for (int i = 0; i < buzzer; i++) {
		if (tone_pins[i] == 255) {
			tone_pins[i] = _pin;
			_index = i;
			break;
		}
	}

	return _index;
}

// frequency (in hertz) and duration (in milliseconds).
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration) {
	int8_t _index;

	_index = toneBegin(_pin);

	if (_index >= 0) {
		// Set the pinMode as OUTPUT
		pinMode(_pin, OUTPUT);

		// Alternate handling of zero freqency to avoid divide by zero errors
		if (frequency == 0)
		{
			noTone(_pin);
			return;
		}

		// Calculate the toggle count
		if (duration > 0) {
			toggle_counts[_index] = 2 * frequency * duration / 1000;
		}
		else {
			toggle_counts[_index] = -1;
		}

		// set up the interrupt frequency
		switch (tone_timers[_index]) {
		case 0:
			// Not currently supported
			digitalWrite(buzzer, LOW);
			break;

		case 1:
			timer1_disable();
			timer1_isr_init();
			timer1_attachInterrupt(t1IntHandler);
			timer1_enable(TIM_DIV1, TIM_EDGE, TIM_LOOP);
			timer1_write((clockCyclesPerMicrosecond() * 500000) / frequency);
			break;
		}
	}
}

void disableTimer(uint8_t _index) {
	tone_pins[_index] = 255;
	digitalWrite(buzzer, LOW);
	switch (tone_timers[_index]) {
	case 0:
		// Not currently supported
		break;

	case 1:
		timer1_disable();
		break;
	}
}

void noTone(uint8_t _pin) {
	for (int i = 0; i < buzzer; i++) {
		if (tone_pins[i] == _pin) {
			tone_pins[i] = 255;
			disableTimer(i);
			break;
		}
	}
	digitalWrite(_pin, LOW);
}

ICACHE_RAM_ATTR void t1IntHandler() {
	if (toggle_counts[T1INDEX] != 0) {
		// toggle the pin
		digitalWrite(tone_pins[T1INDEX], toggle_counts[T1INDEX] % 2);
		toggle_counts[T1INDEX]--;
		// handle the case of indefinite duration
		if (toggle_counts[T1INDEX] < -2) {
			toggle_counts[T1INDEX] = -1;
		}
	}
	else {
		disableTimer(T1INDEX);
		digitalWrite(tone_pins[T1INDEX], LOW);
	}
}

void initBuzzer(unsigned long duration)
{
	tone(buzzer, 1000, duration);
}

void deinitBuzzer()
{
	noTone(buzzer);
	digitalWrite(buzzer, LOW);
}
