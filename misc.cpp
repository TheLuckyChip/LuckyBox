#include "misc.h"

#if defined setHeater
	#define WAIT { while (ESP.getCycleCount()-start < wait) optimistic_yield(1); wait += F_CPU/19200; }
#endif

float EEPROM_float_read(int addr) {
	byte x[4];
	for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
	float *y = (float *)&x;
	return y[0];
}
void EEPROM_float_write(int addr, float val) {
	byte *x = (byte *)&val;
	for (byte i = 0; i < 4; i++) EEPROM.write(i + addr, x[i]);
}
void stop_Err() {
	csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
	power.heaterStatus = 0;						// выключили ТЭН
	power.heaterPower = 0;						// установили мощность на ТЭН 0 %
	timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
	processMode.timeStep = 0;
	timePauseOff = 60000 * 2 + millis();
	if (processMode.allow == 1) processMode.step = 4;			// дистилляция
	else if (processMode.allow == 2) {							// ректификация
		if (processMode.number == 0) processMode.step = 4;
		else processMode.step = 7;
		if (processMode.number == 1 || processMode.number == 2) setPWM(PWM_CH5, 0, 10); // Закрыли отбор по пару
	}
}
void check_Err() {
	// датчики безопасности в каналах АЦП
	if ((pwmOut[3].member == 0 || processMode.allow == 1) && adcIn[0].member == 1 && adcIn[0].allert == true && alertLevelEnable == true) settingAlarm = true;
	else if (adcIn[1].member == 1 && adcIn[1].allert == true) { settingAlarm = true; errA = true; numCrashStop = 1; }
	else if (adcIn[2].member == 1 && adcIn[2].allert == true) { settingAlarm = true; errA = true; numCrashStop = 2; }
	else if (adcIn[3].member == 1 && adcIn[3].allert == true) { settingAlarm = true; errA = true; numCrashStop = 3; }
	if (!errA) timePauseErrA = millis() + 10000;		// 10 секунд пауза до защиты
	// датчики безопасности по температурным датчикам кроме Т куба и Т царги кроме бражной колонны
	if (temperatureSensor[DS_Out].cutoff == 1 && temperatureSensor[DS_Out].member == 1 && temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data >= temperatureSensor[DS_Out].allertValue && processMode.number != 5) {
		settingAlarm = true; errT = true; numCrashStop = DS_Out;
	}
	else if (temperatureSensor[DS_Def].cutoff == 1 && temperatureSensor[DS_Def].member == 1 && temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data >= temperatureSensor[DS_Def].allertValue && processMode.number != 5) {
		settingAlarm = true; errT = true; numCrashStop = DS_Def;
	}
	else if (temperatureSensor[DS_Res1].cutoff == 1 && temperatureSensor[DS_Res1].member == 1 && temperatureSensor[DS_Res1].allertValue > 0 && temperatureSensor[DS_Res1].data >= temperatureSensor[DS_Res1].allertValue) {
		settingAlarm = true; errT = true; numCrashStop = DS_Res1;
	}
	else if (temperatureSensor[DS_Res2].cutoff == 1 && temperatureSensor[DS_Res2].member == 1 && temperatureSensor[DS_Res2].allertValue > 0 && temperatureSensor[DS_Res2].data >= temperatureSensor[DS_Res2].allertValue) {
		settingAlarm = true; errT = true; numCrashStop = DS_Res2;
	}
	else if (temperatureSensor[DS_Res3].cutoff == 1 && temperatureSensor[DS_Res3].member == 1 && temperatureSensor[DS_Res3].allertValue > 0 && temperatureSensor[DS_Res3].data >= temperatureSensor[DS_Res3].allertValue) {
		settingAlarm = true; errT = true; numCrashStop = DS_Res3;
	}
	else if (temperatureSensor[DS_Res4].cutoff == 1 && temperatureSensor[DS_Res4].member == 1 && temperatureSensor[DS_Res4].allertValue > 0 && temperatureSensor[DS_Res4].data >= temperatureSensor[DS_Res4].allertValue) {
		settingAlarm = true; errT = true; numCrashStop = DS_Res4;
	}
	if (!errT) timePauseErrT = millis() + 10000;		// 10 секунд пауза до защиты
}

// Выключение повышенного напряжения на клапана
void stepApLoop() {
	if (CH_all == true) {
		if (timeSetHighVoltage < millis()) {
			if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 4096, 0);
			else pwm.setPWM(PWM_CH9, 0, 4096);
			CH_all = false;
		}
	}
}