//  Дистилляция

#include "distillation_mode.h"

float settingTank = 99.5;                   // температура отключени¤ нагрева куба при дистилл¤ции браги в спирт-сырец

void loadEepromDistillation() {
	int i;
	EEPROM.begin(2048);
	// Считаем что раньше сохраняли
	uint16_t index = 1300;
	if (EEPROM.read(index) == 1) {
		index++;
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = EEPROM.read(index);  index++;
			tpl2web.dsPriority[i] = EEPROM.read(index);  index++;
			tpl2web.dsAllertValue[i] = EEPROM_float_read(index); index += 4;
			tpl2web.dsCutoff[i] = EEPROM.read(index);  index++;
			//if (processMode.allow == 1) {
				temperatureSensor[i].member = tpl2web.dsMember[i];
				temperatureSensor[i].priority = tpl2web.dsPriority[i];
				temperatureSensor[i].allertValueIn = tpl2web.dsAllertValue[i];
				temperatureSensor[i].cutoff = tpl2web.dsCutoff[i];
			//}
		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = EEPROM.read(index);  index++;
			if (processMode.allow == 1) pwmOut[i].member = tpl2web.pwmMember[i];
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = EEPROM.read(index);  index++;
			if (processMode.allow == 1) adcIn[i].member = tpl2web.adcMember[i];
		}
	}
	else {
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = 0;
			tpl2web.dsPriority[i] = 0;
			tpl2web.dsAllertValue[i] = 0;
			tpl2web.dsCutoff[i] = 0;
			if (processMode.allow == 1) {
				temperatureSensor[i].member = 0;
				temperatureSensor[i].priority = 0;
				temperatureSensor[i].allertValue = 0;
				temperatureSensor[i].cutoff = 0;
			}
		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = 0;
			if (processMode.allow == 1) pwmOut[i].member = 0;
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = 0;
			if (processMode.allow == 1) adcIn[i].member = 0;
		}
	}
	EEPROM.end();
}

void initDistillation()
{
	HTTP.on("/distillationSensorsGetTpl", handleDistillationTpl);				// Отправка топологии датчиков при входе на закладку
	HTTP.on("/distillationSensorsSetLoad", handleDistillationSensorSetLoad);    // Отправка - Добавить датчики для процесса
	HTTP.on("/distillationSensorsSetSave", handleDistillationSensorSetSave);	// Прием выбранных датчиков
}
// Отправка топологии датчиков при входе на закладку
void handleDistillationTpl() {
	int i, k;

	loadEepromDistillation();
	if (processMode.allow == 0) {
		EEPROM.begin(2048);
		power.inPowerHigh = EEPROM.read(1397);
		if (power.inPowerHigh > 100) power.inPowerHigh = 100;
		power.inPowerLow = EEPROM.read(1398);
		if (power.inPowerLow > 100) power.inPowerLow = 65;
		for (int i = 0; i < DS_Cnt; i++) {
			if (temperatureSensor[i].cutoff == true) temperatureSensor[i].allertValue = temperatureSensor[i].allertValueIn;
		}
		EEPROM.end();
	}

	String dataForWeb = "{";
	// датчики температуры
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				dataForWeb += "\"t" + String(i) + "\":{\"value\":" + String(temperatureSensor[k].data);
				dataForWeb += ",\"name\":\"" + String(temperatureSensor[k].name) + "\",\"color\":" + String(temperatureSensor[k].color);
				dataForWeb += ",\"member\":" + String(tpl2web.dsMember[k]);
				dataForWeb += ",\"cutoff\":" + String(tpl2web.dsCutoff[k]) + ",\"priority\":" + String(tpl2web.dsPriority[k]);
				dataForWeb += ",\"allertValue\":" + String(temperatureSensor[k].allertValueIn) + "},";
				break;
			}
			if (k < 7) k++;
			else break;
		}
	}
	// выходы ШИМ
	for (i = 0; i < 8; i++) {
		dataForWeb += "\"out" + String(i + 1) + "\":{\"value\":" + String(pwmOut[i].data) + ",\"name\":\"" + String(pwmOut[i].name) + "\",\"member\":" + String(tpl2web.pwmMember[i]) + "},";
	}
	// входы АЦП
	dataForWeb += "\"in1\":{\"value\":" + String(adcIn[0].data) + ",\"name\":\"" + String(adcIn[0].name) + "\",\"member\":" + String(tpl2web.adcMember[0]) + "},";
	dataForWeb += "\"in2\":{\"value\":" + String(adcIn[1].data) + ",\"name\":\"" + String(adcIn[1].name) + "\",\"member\":" + String(tpl2web.adcMember[1]) + "},";
	dataForWeb += "\"in3\":{\"value\":" + String(adcIn[2].data) + ",\"name\":\"" + String(adcIn[2].name) + "\",\"member\":" + String(tpl2web.adcMember[2]) + "},";
	dataForWeb += "\"in4\":{\"value\":" + String(adcIn[3].data) + ",\"name\":\"" + String(adcIn[3].name) + "\",\"member\":" + String(tpl2web.adcMember[3]) + "},";
	dataForWeb += "\"number\":" + String(processMode.number) + ",\"powerHigh\":" + String(power.inPowerHigh) + ",\"powerLower\":" + String(power.inPowerLow) + "}";
	HTTP.send(200, "text/json", dataForWeb);
}
// Отправка - Добавить датчики для процесса
void handleDistillationSensorSetLoad() {
	int i, k;
	loadEepromDistillation();
	String dataForWeb = "{";
	// датчики температуры
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				dataForWeb += "\"t" + String(i) + "\":{\"value\":" + String(temperatureSensor[k].data);
				dataForWeb += ",\"name\":\"" + String(temperatureSensor[k].name) + "\",\"color\":" + String(temperatureSensor[k].color);
				dataForWeb += ",\"member\":" + String(temperatureSensor[k].member) + ",\"priority\":" + String(temperatureSensor[k].priority);
				dataForWeb += ",\"allertValue\":" + String(temperatureSensor[k].allertValue) + "},";
				break;
			}
			if (k < 7) k++;
			else break;
		}
	}
	// выходы ШИМ
	for (i = 0; i < 8; i++) {
		dataForWeb += "\"out" + String(i + 1) + "\":{\"value\":" + String(pwmOut[i].data) + ",\"name\":\"" + String(pwmOut[i].name) + "\",\"member\":" + String(pwmOut[i].member) + "},";
	}
	// входы АЦП
	dataForWeb += "\"in1\":{\"value\":" + String(adcIn[0].data) + ",\"name\":\"" + String(adcIn[0].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in2\":{\"value\":" + String(adcIn[1].data) + ",\"name\":\"" + String(adcIn[1].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in3\":{\"value\":" + String(adcIn[2].data) + ",\"name\":\"" + String(adcIn[2].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in4\":{\"value\":" + String(adcIn[3].data) + ",\"name\":\"" + String(adcIn[3].name) + "\",\"member\":" + String(adcIn[0].member) + "}}";

	HTTP.send(200, "text/json", dataForWeb);
}
// Прием выбранных датчиков
void handleDistillationSensorSetSave() {
	String arg;
	uint16_t index = 1300;
	int i, k;
	// парсим ответ от браузера в переменные
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				arg = "t" + String(i);
				temperatureSensor[k].member = HTTP.arg(arg + "[member]").toInt();
				temperatureSensor[k].priority = HTTP.arg(arg + "[priority]").toInt();
				temperatureSensor[k].allertValue = HTTP.arg(arg + "[allertValue]").toFloat();
				temperatureSensor[k].cutoff = HTTP.arg(arg + "[cutoff]").toInt();
				break;
			}
			if (k < 7) k++;
			else break;
		}
	}
	for (i = 0; i < 8; i++) {
		arg = "out" + String(i + 1);
		pwmOut[i].member = HTTP.arg(arg + "[member]").toInt();
	}
	for (i = 0; i < 4; i++) {
		arg = "in" + String(i + 1);
		adcIn[i].member = HTTP.arg(arg + "[member]").toInt();
	}

	// сохраним в EEPROM
	EEPROM.begin(2048);
	EEPROM.write(index, 0x01); index++; // 1-й процесс = дистилляция
	// Датчики температуры
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, temperatureSensor[i].member);  index++;
		EEPROM.write(index, temperatureSensor[i].priority);  index++;
		EEPROM_float_write(index, temperatureSensor[i].allertValue); index += 4;
		EEPROM.write(index, temperatureSensor[i].cutoff); index++;
	}
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, pwmOut[i].member);  index++;
	}
	for (i = 0; i < 4; i++) {
		EEPROM.write(index, adcIn[i].member);  index++;
	}

	EEPROM.end();
	delay(200);
	HTTP.send(200, "text/json", "{\"result\":\"ok\"}");
}

void distillationLoop() {
	if (processMode.step < 2) {
		if (power.heaterPower != power.inPowerHigh) power.heaterPower = power.inPowerHigh;
	}
	else if (processMode.step < 4) {
		if (power.heaterPower != power.inPowerLow) power.heaterPower = power.inPowerLow;
	}
	else power.heaterPower = 0;

	if (processMode.number > 0) {
		// датчики безопасности в каналах АЦП
		errA = false;
		if (adcIn[0].member == 1 && adcIn[0].allert == true) { settingAlarm = true; errA = true; }
		else if (adcIn[1].member == 1 && adcIn[1].allert == true) { settingAlarm = true; errA = true; }
		else if (adcIn[2].member == 1 && adcIn[2].allert == true) settingAlarm = true;
		else if (adcIn[3].member == 1 && adcIn[3].allert == true) settingAlarm = true;
		else if (timeAllertInterval < millis()) settingAlarm = false;
		if (!errA) timePauseErrA = millis() + 10000; // 10 секунд
		else if (timePauseErrA <= millis()) { stopErr(); nameProcessStep = "Стоп по аварии ADC"; }

		// датчики безопасности по температурным датчикам кроме Т куба и Т царги
		errT = false;
		if (temperatureSensor[DS_Out].cutoff == 1 && temperatureSensor[DS_Out].member == 1 && temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data >= temperatureSensor[DS_Out].allertValue) {
			errT = true;
		}
		else if (temperatureSensor[DS_Def].cutoff == 1 && temperatureSensor[DS_Def].member == 1 && temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data >= temperatureSensor[DS_Def].allertValue) {
			errT = true;
		}
		else if (temperatureSensor[DS_Res1].cutoff == 1 && temperatureSensor[DS_Res1].member == 1 && temperatureSensor[DS_Res1].allertValue > 0 && temperatureSensor[DS_Res1].data >= temperatureSensor[DS_Res1].allertValue) {
			errT = true;
		}
		else if (temperatureSensor[DS_Res2].cutoff == 1 && temperatureSensor[DS_Res2].member == 1 && temperatureSensor[DS_Res2].allertValue > 0 && temperatureSensor[DS_Res2].data >= temperatureSensor[DS_Res2].allertValue) {
			errT = true;
		}
		else if (temperatureSensor[DS_Res3].cutoff == 1 && temperatureSensor[DS_Res3].member == 1 && temperatureSensor[DS_Res3].allertValue > 0 && temperatureSensor[DS_Res3].data >= temperatureSensor[DS_Res3].allertValue) {
			errT = true;
		}
		else if (temperatureSensor[DS_Res4].cutoff == 1 && temperatureSensor[DS_Res4].member == 1 && temperatureSensor[DS_Res4].allertValue > 0 && temperatureSensor[DS_Res4].data >= temperatureSensor[DS_Res4].allertValue) {
			errT = true;
		}
		if (!errT) timePauseErrT = millis() + 10000; // 10 секунд
		else if (timePauseErrT <= millis()) { stopErr(); nameProcessStep = "Стоп по аварии T"; }
	}

	switch (processMode.step) {
		// пришли при старте дистилляции
		case 0: {
			startWriteSD = true;
			loadEepromDistillation();
			EEPROM.begin(2048);
			power.inPowerHigh = EEPROM.read(1397);
			if (power.inPowerHigh > 100) power.inPowerHigh = 100;
			power.inPowerLow = EEPROM.read(1398);
			if (power.inPowerLow > 100) power.inPowerLow = 65;
			for (int i = 0; i < DS_Cnt; i++) {
				if (temperatureSensor[i].cutoff == true) temperatureSensor[i].allertValue = temperatureSensor[i].allertValueIn;
			}
			EEPROM.end();
			// подготовка данных для вывода на TFT
#if defined TFT_Display
			csOn(TFT_CS);
			graphOutInterval = Display_out_temp;
			tftStartForGraph();
			displayTimeInterval = millis() + 1000;
			DefCubOut = Display_out_temp;
			csOff(TFT_CS);
#endif
			tempBigOut = 1;
			if (pwmOut[0].member == 1) csOn(PWM_CH1);		// открыть клапан отбора
			else if (pwmOut[1].member == 1) csOn(PWM_CH2);	// открыть клапан отбора
			power.heaterStatus = 1;							// включили нагрев
			csOn(PWM_CH6);									// включить дополнительный ТЭН на разгон
			power.heaterPower = power.inPowerHigh;			// установили мощность на ТЭН
			processMode.timeStep = 0;
			nameProcessStep = "Нагрев куба";
			processMode.timeStart = time(nullptr);
			processMode.step = 1;		// перешли на следующий шаг алгоритма
			break;
		}
		case 1: {
			// ждем нагрев куба до 80 градусов
			if (temperatureSensor[DS_Cube].data >= 80.0) {
				csOn(PWM_CH3);			// включаем клапан подачи воды
				csOff(PWM_CH6);			// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;	// установили мощность на ТЭН
				settingAlarm = true;	// подаем звуковой сигнал
				timePauseOff = millis();// обнулим счетчик времени для зв.сигнала
				processMode.timeStep = 0;
				nameProcessStep = "Отбор СС";
				processMode.step = 2;	// перешли на следующий шаг алгоритма
			}
			break;
		}
		case 2: {
			// проверяем время (10 сек.) чтобы выключить пищалку
			if (adcIn[0].allert != true && (millis() - timePauseOff) >= 10000) {
				settingAlarm = false;	// выключили звуковой сигнал
				processMode.step = 3;	// перешли на следующий шаг алгоритма
			}
			break;
		}
		case 3: {
			// ждем достижения заданных температур
			if (temperatureSensor[DS_Cube].data >= settingTank || (temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue && temperatureSensor[DS_Cube].allertValue > 0)) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность 0%
				timePauseOff = millis();					// обнулим счетчик времени для зв.сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				settingAlarm = true;						// подали звуковой сигнал
				processMode.timeStep = 0;
				nameProcessStep = "Процесс закончен";
				processMode.step = 4;						// перешли на следующий шаг алгоритма
			}
			// если сработал датчик уровня жидкости подаем звуковой сигнал
			else if (adcIn[0].allert == true) settingAlarm = true;	// подали звуковой сигнал
			else settingAlarm = false;								// выключили звуковой сигнал
			break;
		}
		case 4: {
			// ждем 30 сек.
			if ((millis() - timePauseOff) >= 30000) {
				csOff(PWM_CH1);		// закрыли клапан отбора
				csOff(PWM_CH2);		// закрыли клапан отбора
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				settingAlarm = false;	// выключили звуковой сигнал
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
			}
			break;
		}
	}
}
