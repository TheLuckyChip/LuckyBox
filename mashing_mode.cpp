// 
// 
// 

#include "mashing_mode.h"

void loadEepromMashing() {
	int i;
	// Считаем что раньше сохраняли
	EEPROM.begin(2048);
	uint16_t index = 1500;
	if (EEPROM.read(index) == 3) {
		index++;
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = EEPROM.read(index);  index++;
			tpl2web.dsPriority[i] = EEPROM.read(index);  index++;
			tpl2web.dsCutoff[i] = EEPROM.read(index);  index++;
			//if (processMode.allow == 3) {
				temperatureSensor[i].member = tpl2web.dsMember[i];
				temperatureSensor[i].priority = tpl2web.dsPriority[i];
				temperatureSensor[i].cutoff = tpl2web.dsCutoff[i];
			//}
		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = EEPROM.read(index);  index++;
			pwmOut[i].member = tpl2web.pwmMember[i]; //if (processMode.allow == 3) pwmOut[i].member = tpl2web.pwmMember[i];
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = EEPROM.read(index);  index++;
			adcIn[i].member = tpl2web.adcMember[i]; //if (processMode.allow == 3) adcIn[i].member = tpl2web.adcMember[i];
		}
	}
	else {
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = 0;
			tpl2web.dsPriority[i] = 0;
			tpl2web.dsCutoff[i] = 0;
			//if (processMode.allow == 3) {
				temperatureSensor[i].member = 0;
				temperatureSensor[i].priority = 0;
				temperatureSensor[i].cutoff = 0;
			//}
		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = 0;
			pwmOut[i].member = 0; //if (processMode.allow == 3) pwmOut[i].member = 0;
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = 0;
			adcIn[i].member = 0; //if (processMode.allow == 3) adcIn[i].member = 0;
		}
	}
	/*
		processMashing[0].time = 20; processMashing[0].temperature = 45;
		processMashing[1].time = 30; processMashing[1].temperature = 55;
		processMashing[2].time = 120; processMashing[2].temperature = 65;
		processMashing[3].time = 10; processMashing[3].temperature = 72;
	*/
	EEPROM.end();
}

void initMashing() {
	HTTP.on("/mashingSensorsGetTpl", handleMashingSensorTpl);				// Отправка топологии датчиков при входе на закладку
	HTTP.on("/mashingSensorsSetLoad", handleMashingSensorSetLoad);	// Отправка - Добавить датчики для процесса
	HTTP.on("/mashingSensorsSetSave", handleMashingSensorSetSave);	// Прием выбранных датчиков
}

// Отправка топологии датчиков при входе на закладку
void handleMashingSensorTpl() {
	int i, k;

	loadEepromMashing();

	String dataForWeb = "{";
	// датчики температуры
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				dataForWeb += "\"t" + String(i) + "\":{\"value\":" + String(temperatureSensor[k].data);
				dataForWeb += ",\"name\":\"" + String(temperatureSensor[k].name) + "\",\"color\":" + String(temperatureSensor[k].color);
				dataForWeb += ",\"member\":" + String(tpl2web.dsMember[k]);
				dataForWeb += ",\"priority\":" + String(tpl2web.dsPriority[k]);
				dataForWeb += ",\"allertValue\":" + String(temperatureSensor[k].allertValueIn) + "},";
				break;
			}
			if (k < 7) k++;
			else break;
		}
	}
	// температурные паузы
	dataForWeb += "\"pause1\":{\"name\":\"Внесение солода\",\"time\":" + String(processMashing[0].time);
	dataForWeb += ",\"temperature\":" + String(processMashing[0].temperature) + ",\"stop\":" + String(processMashing[0].stop) + "},";
	dataForWeb += "\"pause2\":{\"name\":\"Белковая пауза\",\"time\":" + String(processMashing[1].time);
	dataForWeb += ",\"temperature\":" + String(processMashing[1].temperature) + ",\"stop\":" + String(processMashing[1].stop) + "},";
	dataForWeb += "\"pause3\":{\"name\":\"Осахаривание\",\"time\":" + String(processMashing[2].time);
	dataForWeb += ",\"temperature\":" + String(processMashing[2].temperature) + ",\"stop\":" + String(processMashing[2].stop) + "},";
	dataForWeb += "\"pause4\":{\"name\":\"Мэш аут\",\"time\":" + String(processMashing[3].time);
	dataForWeb += ",\"temperature\":" + String(processMashing[3].temperature) + ",\"stop\":" + String(processMashing[3].stop) + "}}";
	HTTP.send(200, "text/json", dataForWeb);
}
// Отправка - Добавить датчики для процесса
void handleMashingSensorSetLoad() {
	int i, k;
	loadEepromMashing();
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
void handleMashingSensorSetSave() {
	String arg;
	uint16_t index = 1500;
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
	EEPROM.write(index, 0x03); index++; // 1-й процесс = дистилляция
										// Датчики температуры
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, temperatureSensor[i].member); index++;
		EEPROM.write(index, temperatureSensor[i].priority); index++;
		EEPROM.write(index, temperatureSensor[i].cutoff); index++;
	}
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, pwmOut[i].member); index++;
	}
	for (i = 0; i < 4; i++) {
		EEPROM.write(index, adcIn[i].member); index++;
	}
	
	EEPROM.end();
	delay(200);

	HTTP.send(200, "text/json", "{\"result\":\"ok\"}");
}

void mashingLoop() {
	// поиск выбранного датчика и начальная инициализация
	if (processMode.step == 0) {
		// если при выборе нет приоритета берем первый из выбранных
		if (temperatureSensor[0].member == 1) numSenseMashBrew = 0;
		else if (temperatureSensor[1].member == 1) numSenseMashBrew = 1;
		else if (temperatureSensor[2].member == 1) numSenseMashBrew = 2;
		else if (temperatureSensor[3].member == 1) numSenseMashBrew = 3;
		else if (temperatureSensor[4].member == 1) numSenseMashBrew = 4;
		else if (temperatureSensor[5].member == 1) numSenseMashBrew = 5;
		else if (temperatureSensor[6].member == 1) numSenseMashBrew = 6;
		else if (temperatureSensor[7].member == 1) numSenseMashBrew = 7;
		// если есть приоритет
		if (temperatureSensor[0].priority == 1) numSenseMashBrew = 0;
		else if (temperatureSensor[1].priority == 1) numSenseMashBrew = 1;
		else if (temperatureSensor[2].priority == 1) numSenseMashBrew = 2;
		else if (temperatureSensor[3].priority == 1) numSenseMashBrew = 3;
		else if (temperatureSensor[4].priority == 1) numSenseMashBrew = 4;
		else if (temperatureSensor[5].priority == 1) numSenseMashBrew = 5;
		else if (temperatureSensor[6].priority == 1) numSenseMashBrew = 6;
		else if (temperatureSensor[7].priority == 1) numSenseMashBrew = 7;
	}
	// запомним текущую температуру для PID регулировки
	Input = temperatureSensor[numSenseMashBrew].data;

	switch (processMode.step) {
		// пришли при старте затирания
		case 0: {
			// подготовка данных для вывода на TFT
#if defined TFT_Display
			csOn(TFT_CS);
			graphOutInterval = Display_out_temp;
			//tft.fillScreen(ILI9341_BLACK);
			tftStartForGraph();
			displayTimeInterval = millis() + 1000;
			DefCubOut = Display_out_temp;
			csOff(TFT_CS);
#endif
			tempBigOut = 1;
			myPID.SetOutputLimits(0, WindowSize);		// временной интервал реагирования для PID
			myPID.SetMode(AUTOMATIC);					// режим PID
			Setpoint = processMashing[0].temperature;	// температура которую надо поддерживать PID алгоритму (1-й шаг)
			processMashing[0].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
			processMode.timeStep = 0;					// для расчета и вывода времени прошедшего с начала текущего шага
			windowStartTime = millis();					// для расчетов в алгоритме PID
			processMode.timeStart = time(nullptr);
			nameProcessStep = "1-я пауза нагрев";
			processMode.step = 1;			// перешли на следующий шаг алгоритма
			break;
		}
		// Нагрев до температуры 1-й паузы если она задана
		case 1: {
			Setpoint = processMashing[0].temperature;	// температура которую надо поддерживать PID алгоритму (1-й шаг)
			if ((processMashing[0].time == 0 || processMashing[0].temperature == 0) && processMashing[0].stop == 0) { // если 1-й шаг надо пропустить
				Setpoint = processMashing[1].temperature;	// температура которую надо поддерживать PID алгоритму (2-й шаг)
				processMashing[0].step = 0;
				processMashing[1].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				nameProcessStep = "2-я пауза нагрев";
				processMode.step = 3;			// пропускаем шаг
			}
			else if (Input >= Setpoint) {
				stepTime = millis();			// для расчетов в алгоритме PID
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "1-я пауза стаб.";
				processMode.step = 2;			// перешли на следующий шаг алгоритма
			}
			processMode.timeStep = 0;			// для расчета и вывода времени прошедшего с начала текущего шага
			break;
		}
		// Ждем окончание 1-й паузы
		case 2: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[0].temperature;	// температура которую надо поддерживать PID алгоритму (1-й шаг)
			if (millis() >= 60000 * processMashing[0].time + stepTime && processMashing[0].stop == 0) {
				Setpoint = processMashing[1].temperature;	// температура которую надо поддерживать PID алгоритму (2-й шаг)
				processMashing[0].step = 0;
				processMashing[1].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "2-я пауза нагрев";
				processMode.step = 3;			// перешли на следующий шаг алгоритма
			}
			break;
		}



		// Нагрев до температуры 2-й паузы если она задана
		case 3: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[1].temperature;	// температура которую надо поддерживать PID алгоритму (2-й шаг)
			if ((processMashing[1].time == 0 || processMashing[1].temperature == 0) && processMashing[1].stop == 0) { // если 2-й шаг надо пропустить
				Setpoint = processMashing[2].temperature;	// температура которую надо поддерживать PID алгоритму (3-й шаг)
				processMashing[1].step = 0;
				processMashing[2].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				nameProcessStep = "3-я пауза нагрев";
				processMode.step = 5;			// пропускаем шаг
			}
			else if (Input >= Setpoint) {
				stepTime = millis();			// для расчетов в алгоритме PID
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "2-я пауза стаб.";
				processMode.step = 4;			// перешли на следующий шаг алгоритма
			}
			processMode.timeStep = 0;			// для расчета и вывода времени прошедшего с начала текущего шага
			break;
		}
		// Ждем окончание 2-й паузы
		case 4: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[1].temperature;	// температура которую надо поддерживать PID алгоритму (2-й шаг)
			if (millis() >= 60000 * processMashing[1].time + stepTime && processMashing[1].stop == 0) {
				Setpoint = processMashing[2].temperature;	// температура которую надо поддерживать PID алгоритму (3-й шаг)
				processMashing[1].step = 0;
				processMashing[2].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				settingAlarm = true;						// подаем звуковой сигнал
				timePauseOff = millis() + 6000;				// счетчик времени для зв.сигнала
				nameProcessStep = "3-я пауза нагрев";
				processMode.step = 5;			// перешли на следующий шаг алгоритма
			}
			break;
		}



		// Нагрев до температуры 3-й паузы если она задана
		case 5: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[2].temperature;	// температура которую надо поддерживать PID алгоритму (3-й шаг)
			if ((processMashing[2].time == 0 || processMashing[2].temperature == 0) && processMashing[2].stop == 0) { // если 3-й шаг надо пропустить
				Setpoint = processMashing[3].temperature;	// температура которую надо поддерживать PID алгоритму (4-й шаг)
				processMashing[2].step = 0;
				processMashing[3].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				nameProcessStep = "4-я пауза нагрев";
				processMode.step = 7;			// пропускаем шаг
			}
			else if (Input >= Setpoint) {
				stepTime = millis();			// для расчетов в алгоритме PID
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "3-я пауза стаб.";
				processMode.step = 6;			// перешли на следующий шаг алгоритма
			}
			processMode.timeStep = 0;			// для расчета и вывода времени прошедшего с начала текущего шага
			break;
		}
		// Ждем окончание 3-й паузы
		case 6: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[2].temperature;	// температура которую надо поддерживать PID алгоритму (3-й шаг)
			if (millis() >= 60000 * processMashing[2].time + stepTime && processMashing[2].stop == 0) {
				Setpoint = processMashing[3].temperature;	// температура которую надо поддерживать PID алгоритму (4-й шаг)
				processMashing[2].step = 0;
				processMashing[3].step = 1;					// для индикации обрабатываемой температурной паузы в WEB
				windowStartTime = millis();					// для расчетов в алгоритме PID
				settingAlarm = true;				// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "4-я пауза нагрев";
				processMode.step = 7;			// перешли на следующий шаг алгоритма
			}
			break;
		}



		// Нагрев до температуры 4-й паузы если она задана
		case 7: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[3].temperature;	// температура которую надо поддерживать PID алгоритму (4-й шаг)
			if ((processMashing[3].time == 0 || processMashing[3].temperature == 0) && processMashing[3].stop == 0) { // если 4-й шаг надо пропустить
				// нет последней паузы, окончание процесса
				timePauseOff = millis() + 10000;		// счетчик времени для зв.сигнала
				settingAlarm = true;			// включили звуковой сигнал
				processMashing[3].step = 0;
				processMode.timeStep = 0;
				stepTime = millis();			// для расчетов в алгоритме PID
				processMode.step = 9;			// перешли на следующий шаг алгоритма
			}
			else if (Input >= Setpoint) {
				stepTime = millis();			// для расчетов в алгоритме PID
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 6000;		// счетчик времени для зв.сигнала
				nameProcessStep = "4-я пауза стаб.";
				processMode.step = 8;			// перешли на следующий шаг алгоритма
			}
			processMode.timeStep = 0;			// для расчета и вывода времени прошедшего с начала текущего шага
			break;
		}
		// Ждем окончание 4-й паузы
		case 8: {
			if (millis() >= timePauseOff) settingAlarm = false;	// выключили звуковой сигнал
			Setpoint = processMashing[3].temperature;	// температура которую надо поддерживать PID алгоритму (4-й шаг)
			if (millis() >= 60000 * processMashing[3].time + stepTime && processMashing[3].stop == 0) {
				settingAlarm = true;			// подаем звуковой сигнал
				timePauseOff = millis() + 10000;		// счетчик времени для зв.сигнала
				processMashing[3].step = 0;
				Setpoint = 0;
				processMode.step = 9;			// перешли на следующий шаг алгоритма
			}
			break;
		}



		case 9: {
			if (millis() >= timePauseOff) {
				settingAlarm = false;		// выключили звуковой сигнал
				processMode.step = 0;
				processMode.allow = 0;
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
			}
			break;
		}
	}

	myPID.Compute();											// расчет времени для PID регулировки

	if (millis() > WindowSize + windowStartTime) {
		windowStartTime += WindowSize;
		if (windowStartTime > millis()) windowStartTime = 0;    // защита от переполнения
	}

	// Если идет предварительный нагрев (до температуры поддержания Т стабилизации минус 4 градуса)
	if (Input < Setpoint - 4) {
		digitalWrite(heater, HIGH);
	}
	// включить или выключить ТЭН в зависимости от расчетов временного PID регулирования
	else {
		if (Output < millis() - windowStartTime) {
			digitalWrite(heater, LOW);
			power.heaterPower = 0;
		}
		else {
			digitalWrite(heater, HIGH);
			power.heaterPower = 100;
		}
	}

	delay(100);

	if (processMode.allow == 0) {
		power.heaterPower = 0;
		power.heaterStatus = 0;
		settingAlarm = false;			// выключили звуковой сигнал
		digitalWrite(heater, LOW);		// Выключим ТЭН по окончанию процесса
		processMode.timeStep = 0;		// для расчета и вывода времени прошедшего с начала текущего шага
	}
}
