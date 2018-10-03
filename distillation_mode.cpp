//  Дистилляция

#include "distillation_mode.h"

float settingTank = 99.5;                   // температура отключени¤ нагрева куба при дистилл¤ции браги в спирт-сырец

void EEPROM_float_write_dist(int addr, float val) {
	byte *x = (byte *)&val;
	for (byte i = 0; i < 4; i++) EEPROM.write(i + addr, x[i]);
}
float EEPROM_float_read_dist(int addr) {
	byte x[4];
	for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
	float *y = (float *)&x;
	return y[0];
}

void loadEepromDistillation() {
	int i;
	// Считаем что раньше сохраняли
	uint16_t index = 1300;
	if (EEPROM.read(index) == 1) {
		index++;
		for (i = 0; i < 8; i++) {
			temperatureSensor[i].member = EEPROM.read(index);  index++;
			temperatureSensor[i].priority = EEPROM.read(index);  index++;
			temperatureSensor[i].allertValue = EEPROM_float_read_dist(index); index += 4;
			temperatureSensor[i].cutoff = EEPROM.read(index);  index++;
		}
		for (i = 0; i < 8; i++) {
			pwmOut[i].member = EEPROM.read(index);  index++;
		}
		for (i = 0; i < 4; i++) {
			adcIn[i].member = EEPROM.read(index);  index++;
		}
	}
	else {
		for (i = 0; i < 8; i++) {
			temperatureSensor[i].member = 0;
			temperatureSensor[i].priority = 0;
			temperatureSensor[i].allertValue = 0;
			temperatureSensor[i].cutoff = 0;
		}
		for (i = 0; i < 8; i++) {
			pwmOut[i].member = 0;
		}
		for (i = 0; i < 4; i++) {
			adcIn[i].member = 0;
		}
	}
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

	String dataForWeb = "{";
	// датчики температуры
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				dataForWeb += "\"t" + String(i) + "\":{\"value\":" + String(temperatureSensor[k].data);
				dataForWeb += ",\"name\":\"" + String(temperatureSensor[k].name) + "\",\"color\":" + String(temperatureSensor[k].color);
				dataForWeb += ",\"member\":" + String(temperatureSensor[k].member);
				dataForWeb += ",\"cutoff\":" + String(temperatureSensor[k].cutoff) + ",\"priority\":" + String(temperatureSensor[k].priority);
				dataForWeb += ",\"allertValue\":" + String(temperatureSensor[k].allertValueIn) + "},";
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
	HTTP.send(200, "text/json", "{\"result\":\"ok\"}");

	// сохраним в EEPROM
	EEPROM.write(index, 0x01); index++; // 1-й процесс = дистилляция
	// Датчики температуры
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, temperatureSensor[i].member);  index++;
		EEPROM.write(index, temperatureSensor[i].priority);  index++;
		EEPROM_float_write_dist(index, temperatureSensor[i].allertValue); index += 4;
		EEPROM.write(index, temperatureSensor[i].cutoff); index++;
	}
	for (i = 0; i < 8; i++) {
		EEPROM.write(index, pwmOut[i].member);  index++;
	}
	for (i = 0; i < 4; i++) {
		EEPROM.write(index, adcIn[i].member);  index++;
	}

	EEPROM.commit();
	delay(100);
}

void distillationLoop() {

	if (processMode.step < 2) {
		if (power.heaterPower != power.inPowerHigh) power.heaterPower = power.inPowerHigh;
	}
	else if (processMode.step < 4) {
		if (power.heaterPower != power.inPowerLow) power.heaterPower = power.inPowerLow;
	}
	else power.heaterPower = 0;

	//if (power.heaterPower != power.inPowerHigh && processMode.step < 4) power.heaterPower = power.inPowerHigh;
	//else if (processMode.step >= 4) power.heaterPower = 0;

	switch (processMode.step) {
		// пришли при старте дистилляции
		case 0: {
			loadEepromDistillation();
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
			csOn(PWM_CH1);				// открыть клапан отбора
			power.heaterStatus = 1;		// включили нагрев
			power.heaterPower = 100;	// установили мощность на ТЭН 100 %
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
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				settingAlarm = false;	// выключили звуковой сигнал
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
			}
			break;
		}
	}
}
