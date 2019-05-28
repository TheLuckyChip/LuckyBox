//  Дистилляция

#include "distillation_mode.h"

bool valveOnOff;

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
		DistillationTransitionTemperature = EEPROM.read(index);
		if (DistillationTransitionTemperature > 100) DistillationTransitionTemperature = 80;
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
		DistillationTransitionTemperature = 80;
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
	dataForWeb += "\"in4\":{\"value\":" + String(adcIn[3].data) + ",\"name\":\"" + String(adcIn[3].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"transitionTemperature\":" + String(DistillationTransitionTemperature) + "}";

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
	DistillationTransitionTemperature = HTTP.arg("transitionTemperature").toInt();

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
	EEPROM.write(index, DistillationTransitionTemperature);

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

	// Пищалка для WEB и самой автоматики
	if (timeAllertInterval > millis()) settingAlarm = true;
	else if (errA == false && errT == false && adcIn[0].allert == false) settingAlarm = false;

	// Проверка датчиков безопасности
	if (processMode.step != 4 && !errA && !errT) check_Err();
	if (timePauseErrA <= millis()) {
		errA = false; check_Err();
		if (errA) {
			stop_Err();
			nameProcessStep = "Стоп по аварии ADC > " + String(adcIn[numCrashStop].name);
		}
	}
	if (timePauseErrT <= millis()) {
		errT = false; check_Err();
		if (errT) {
			stop_Err();
			nameProcessStep = "Стоп по аварии T > " + String(temperatureSensor[numCrashStop].name);
		}
	}

	switch (processMode.step) {
		// пришли при старте дистилляции
		case 0: {
			alertEnable = true;
			alertLevelEnable = true;
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
			//if (pwmOut[0].member == 1) csOn(PWM_CH1);		// открыть клапан отбора
			//if (pwmOut[1].member == 1) csOn(PWM_CH2);		// открыть клапан отбора
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
			if (temperatureSensor[DS_Cube].data >= DistillationTransitionTemperature) {
				csOn(PWM_CH3);			// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = power.inPowerLow;	// установили мощность на ТЭН
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
				processMode.timeStep = 0;
				nameProcessStep = "Отбор СС";
				processMode.step = 2;	// перешли на следующий шаг алгоритма
			}
			break;
		}
		case 2: {
			valveOnOff = true;
			processMode.step = 3;	// перешли на следующий шаг алгоритма
			break;
		}
		case 3: {
			// ждем достижения заданных температур
			if (temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue && temperatureSensor[DS_Cube].allertValue > 0) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность 0%
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала						// подали звуковой сигнал
				processMode.timeStep = 0;
				nameProcessStep = "Процесс закончен";
				settingAlarm = true;
				processMode.step = 4;						// перешли на следующий шаг алгоритма
			}
			// если выбраны для процесса клапана отбора, закроем их при срабатывании датчика уровня
			if (adcIn[0].member == 1 && adcIn[0].allert == true) {
				valveOnOff = true;
				if (pwmOut[0].member == 1) csOff(PWM_CH1);		// закрыть клапан отбора
				if (pwmOut[1].member == 1) csOff(PWM_CH2);		// закрыть клапан отбора
				//setPWM(PWM_CH5, 0, 10);							// закрыть шаровый кран
			}
			else if (valveOnOff == true) {
				valveOnOff = false;
				if (pwmOut[0].member == 1) csOn(PWM_CH1);		// открыть клапан отбора
				if (pwmOut[1].member == 1) csOn(PWM_CH2);		// открыть клапан отбора
				//setPWM(PWM_CH5, 0, 2000);						// открыть шаровый кран
			}
			break;
		}
		case 4: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%

			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[2].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				//setPWM(PWM_CH5, 0, 10);		// закрыть шаровый кран
				settingAlarm = false;		// выключили звуковой сигнал
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
			}

			break;
		}
	}
}
