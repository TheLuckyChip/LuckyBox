#include "pwm_out.h"
// выключение канала
void csOff(byte ch) {
	if (ch == PWM_CH1) {
		pwmOut[0].allert = 0;
		if (pwmOut[0].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH2) {
		pwmOut[1].allert = 0;
		if (pwmOut[1].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH3) {
		pwmOut[2].allert = 0;
		if (pwmOut[2].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH4) {
		pwmOut[3].allert = 0;
		if (pwmOut[3].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH5) {
		pwmOut[4].allert = 0;
		if (pwmOut[4].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH6) {
		pwmOut[5].allert = 0;
		if (pwmOut[5].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH7) {
		pwmOut[6].allert = 0;
		if (pwmOut[6].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH8) {
		pwmOut[7].allert = 0;
		if (pwmOut[7].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH9) {
		pwmOut[8].allert = 0;
		if (pwmOut[8].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else pwm.setPWM(ch, 4096, 0);
}
// включение канала
void csOn(byte ch) {
	if (ch == PWM_CH1) {
		pwmOut[0].allert = 1;
		if (pwmOut[0].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH2) {
		pwmOut[1].allert = 1;
		if (pwmOut[1].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH3) {
		pwmOut[2].allert = 1;
		if (pwmOut[2].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH4) {
		pwmOut[3].allert = 1;
		if (pwmOut[3].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH5) {
		pwmOut[4].allert = 1;
		if (pwmOut[4].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH6) {
		pwmOut[5].allert = 1;
		if (pwmOut[5].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH7) {
		pwmOut[6].allert = 1;
		if (pwmOut[6].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH8) {
		pwmOut[7].allert = 1;
		if (pwmOut[7].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else if (ch == PWM_CH9) {
		pwmOut[8].allert = 1;
		if (pwmOut[8].invert == false) pwm.setPWM(ch, 0, 4096);
		else pwm.setPWM(ch, 4096, 0);
	}
	else pwm.setPWM(ch, 0, 4096);
	delay(1);
}
// задать скважность ШИМ
void setPWM(byte ch, uint16_t Pon, uint16_t Poff) {
	pwm.setPWM(ch, Pon, Poff);
}
// управление сервоприводом
void servoAngleSet(byte ch, byte angle) {
	uint16_t servoSet;
	if (angle > 180) return;
	servoSet = (25 * angle) / 10 + 150;
	pwm.setPWMFreq(60);
	delay(100);
	if (servoOld > servoSet) {
		for (uint16_t pulselen = servoOld; pulselen > servoSet; pulselen--) {
			setPWM(ch, 0, pulselen);
			delay(1);
		}
	}
	else {
		for (uint16_t pulselen = servoOld; pulselen < servoSet; pulselen++) {
			setPWM(ch, 0, pulselen);
			delay(1);
		}
	}
	delay(100);
	setPWM(ch, 0, 4096);
	pwm.setPWMFreq(1000);
	servoOld = servoSet;
}
