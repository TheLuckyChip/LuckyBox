#include "pwm_out.h"
// выключение канала
void csOff(byte ch) {
	if (ch == PWM_CH1) {
		CH_up1 = true;
		pwmOut[0].allert = 0;
		if (pwmOut[0].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH2) {
		CH_up2 = true;
		pwmOut[1].allert = 0;
		if (pwmOut[1].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH3) {
		CH_up3 = true;
		pwmOut[2].allert = 0;
		if (pwmOut[2].invert == false) pwm.setPWM(ch, 4096, 0);
		else pwm.setPWM(ch, 0, 4096);
	}
	else if (ch == PWM_CH4) {
		CH_up4 = true;
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
	else {
		pwm.setPWM(ch, 4096, 0);
#if defined TFT_Display
		if (touch_in == true && (ch == SD_CS || ch == TFT_CS) && touchRead == 1) {
			touchscreenUpdate();
			touchRead = 0;
			initBuzzer(50);
		}
#endif
	}
}
// включение канала
void csOn(byte ch) {
	if (ch == PWM_CH1) {
		if (CH_up1 == true) {
			// включаем доп. подачу напряжения
			CH_all = true;
			CH_up1 = false;
			if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 0, 4096);
			else pwm.setPWM(PWM_CH9, 4096, 0);
			timeSetHighVoltage = millis() + 250;
			// включаем сам клапан
			pwmOut[0].allert = 1;
			if (pwmOut[0].invert == false) pwm.setPWM(ch, 0, 4096);
			else pwm.setPWM(ch, 4096, 0);
		}
	}
	else if (ch == PWM_CH2) {
		if (CH_up2 == true) {
			// включаем доп. подачу напряжения
			CH_all = true;
			CH_up2 = false;
			if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 0, 4096);
			else pwm.setPWM(PWM_CH9, 4096, 0);
			timeSetHighVoltage = millis() + 250;
			// включаем сам клапан
			pwmOut[1].allert = 1;
			if (pwmOut[1].invert == false) pwm.setPWM(ch, 0, 4096);
			else pwm.setPWM(ch, 4096, 0);
		}
	}
	else if (ch == PWM_CH3) {
		if (CH_up3 == true) {
			// включаем доп. подачу напряжения
			CH_all = true;
			CH_up3 = false;
			if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 0, 4096);
			else pwm.setPWM(PWM_CH9, 4096, 0);
			timeSetHighVoltage = millis() + 250;
			// включаем сам клапан
			pwmOut[2].allert = 1;
			if (pwmOut[2].invert == false) pwm.setPWM(ch, 0, 4096);
			else pwm.setPWM(ch, 4096, 0);
		}
	}
	else if (ch == PWM_CH4) {
		if (CH_up4 == true) {
			// включаем доп. подачу напряжения
			CH_all = true;
			CH_up4 = false;
			if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 0, 4096);
			else pwm.setPWM(PWM_CH9, 4096, 0);
			timeSetHighVoltage = millis() + 250;
			// включаем сам клапан
			pwmOut[3].allert = 1;
			if (pwmOut[3].invert == false) pwm.setPWM(ch, 0, 4096);
			else pwm.setPWM(ch, 4096, 0);
		}
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
	else {
		pwm.setPWM(ch, 0, 4096);
		delay(3);
	}
}
// задать скважность ШИМ
void setPWM(byte ch, uint16_t Pon, uint16_t Poff) {
	pwm.setPWM(ch, Pon, Poff);
}
// управление сервоприводом
void servoAngleSet(byte ch, byte angle) {
	uint16_t servoSet;
	uint16_t pulselen;
	if (angle > 90) return;
	servoSet = 100 + angle * 2;//servoSet = (25 * angle) / 10 + 150;
	pwm.setPWMFreq(50);
	pulselen = servoOld;
	delay(50);
	if (abs(servoOld - servoSet) < 50) {
		if (servoOld > 190) {
			while (pulselen > servoOld - 50) {
				setPWM(ch, 0, pulselen);
				delay(2);
				pulselen--;
			}
			setPWM(ch, 0, pulselen);
		}
		else {
			while (pulselen < servoOld + 50) {
				setPWM(ch, 0, pulselen);
				delay(2);
				pulselen++;
			}
			setPWM(ch, 0, pulselen);
		}
		servoOld = pulselen;
		delay(20);
	}
	if (servoOld > servoSet) {
		while (pulselen > servoSet) {
			setPWM(ch, 0, pulselen);
			delay(2);
			pulselen--;
		}
		setPWM(ch, 0, pulselen);
	}
	else {
		while (pulselen < servoSet) {
			setPWM(ch, 0, pulselen);
			delay(2);
			pulselen++;
		}
		setPWM(ch, 0, pulselen);
	}
	delay(50);
	setPWM(ch, 0, 4096);
	pwm.setPWMFreq(1000);
	servoOld = servoSet;
}
