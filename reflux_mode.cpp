//  Ректификация

#include "reflux_mode.h"
int	countHaedEnd;
unsigned long valveCiclePeriod;
unsigned long timeOn;
unsigned long timeOff;
byte typeRefOfValwe;
bool beepEnd;
bool OnOff = false;
bool bodyValveSet;
unsigned long timeValveMs;
uint8_t bodyPrimaPercent;
uint16_t bodyPrimaPercentSet;

void loadEepromReflux() {
	int i;
	// Считаем что раньше сохраняли
	EEPROM.begin(2048);
	uint16_t index = 1400;
	if (EEPROM.read(index) == 2) {
		index++;
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = EEPROM.read(index);  index++;
			tpl2web.dsPriority[i] = EEPROM.read(index);  index++;
			tpl2web.dsAllertValue[i] = EEPROM_float_read(index); index += 4;
			tpl2web.dsDelta[i] = EEPROM.read(index);  index++;
			tpl2web.dsCutoff[i] = EEPROM.read(index);  index++;

				temperatureSensor[i].member = tpl2web.dsMember[i];
				temperatureSensor[i].priority = tpl2web.dsPriority[i];
				temperatureSensor[i].allertValueIn = tpl2web.dsAllertValue[i];
				temperatureSensor[i].delta = tpl2web.dsDelta[i];
				temperatureSensor[i].cutoff = tpl2web.dsCutoff[i];

		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = EEPROM.read(index);  index++;
			if (processMode.allow == 2) pwmOut[i].member = tpl2web.pwmMember[i];
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = EEPROM.read(index);  index++;
			if (processMode.allow == 2) adcIn[i].member = tpl2web.adcMember[i];
		}
		headTimeCycle = EEPROM.read(index);  index++;                     // начало в EEPROM = 1477
		if (headTimeCycle < 5 || headTimeCycle > 30) headTimeCycle = 10;
		headtimeOn = EEPROM_float_read(index); index += 4;
		if (isnan(headtimeOn) || headtimeOn < 0 || headtimeOn > 100) headtimeOn = 3;
		bodyTimeCycle = EEPROM.read(index); index++;
		if (bodyTimeCycle < 5 || bodyTimeCycle > 30) bodyTimeCycle = 12;
		bodytimeOn = EEPROM_float_read(index); index += 4;
		if (isnan(bodytimeOn) || bodytimeOn < 0 || bodytimeOn > 100) bodytimeOn = 8;
		decline = EEPROM.read(index); index++;
		if (decline > 30) decline = 10;

		timeStabilizationReflux = EEPROM.read(index); index++;
		if (timeStabilizationReflux > 120) timeStabilizationReflux = 20;
		timeBoilTubeSetReflux = EEPROM.read(index); index++;
		if (timeBoilTubeSetReflux > 60) timeBoilTubeSetReflux = 10;

		headSteamPercent = EEPROM.read(index); index++;
		if (headSteamPercent > 100) headSteamPercent = 30;
		bodyPrimaPercentStart = EEPROM.read(index); index++;
		if (bodyPrimaPercentStart > 100) bodyPrimaPercentStart = 100;
		bodyPrimaPercentStop = EEPROM.read(index); index++;
		if (bodyPrimaPercentStop > 100) bodyPrimaPercentStop = 40;
		bodyPrimaDecline = EEPROM.read(index);
		if (bodyPrimaDecline > 30) bodyPrimaDecline = 15;

		power.inPowerHigh = EEPROM.read(1497);
		if (power.inPowerHigh > 100) power.inPowerHigh = 100;
		power.inPowerLow = EEPROM.read(1498);
		if (power.inPowerLow > 100) power.inPowerLow = 65;
		processMode.number = EEPROM.read(1499);
		if (processMode.number > 5) processMode.number = 0;

	}
	else {
		for (i = 0; i < 8; i++) {
			tpl2web.dsMember[i] = 0;
			tpl2web.dsPriority[i] = 0;
			tpl2web.dsAllertValue[i] = 0;
			tpl2web.dsDelta[i] = 0;
			tpl2web.dsCutoff[i] = 0;
			//if (processMode.allow == 2) {
				temperatureSensor[i].member = 0;
				temperatureSensor[i].priority = 0;
				temperatureSensor[i].allertValue = 0;
				temperatureSensor[i].delta = 0;
				temperatureSensor[i].cutoff = 0;
			//}
		}
		for (i = 0; i < 8; i++) {
			tpl2web.pwmMember[i] = 0;
			if (processMode.allow == 2) pwmOut[i].member = 0;
		}
		for (i = 0; i < 4; i++) {
			tpl2web.adcMember[i] = 0;
			if (processMode.allow == 2) adcIn[i].member = 0;
		}
		headTimeCycle = 10;
		headtimeOn = 3.5;
		bodyTimeCycle = 12;
		bodytimeOn = 8.5;
		decline = 10;
		headSteamPercent = 30;
		bodyPrimaPercentStart = 100;
		bodyPrimaPercentStop = 40;
		bodyPrimaDecline = 15;
		timeStabilizationReflux = 20;
		timeBoilTubeSetReflux = 10;
		power.inPowerHigh = 100;
		power.inPowerLow = 65;
		processMode.number = 0;
	}
	EEPROM.end();
	for (int i = 0; i < DS_Cnt; i++) {
		//allertSetTemperatureEn[i] = false;
		if (temperatureSensor[i].cutoff == true) temperatureSensor[i].allertValue = temperatureSensor[i].allertValueIn;
	}

	processMode.timeStart = time(nullptr);
	graphOutInterval = Display_out_temp;
	// определим тип процесса ректификации в зависимости от выбора клапанов
	typeRefOfValwe = 0;
	if (pwmOut[0].member == 1 && pwmOut[1].member == 0) typeRefOfValwe = 1;			// 1-й клапан на головы и тело
	else if (pwmOut[0].member == 0 && pwmOut[1].member == 1) typeRefOfValwe = 2;	// 2-й клапан на головы и тело
	else if (pwmOut[0].member == 1 && pwmOut[1].member == 1) typeRefOfValwe = 3;	// 1-й клапан на головы, 2-й на тело
}

// Обмен с Web
void initReflux()
{
	HTTP.on("/refluxSensorsGetTpl", handleRefluxSensorTpl);			// Отправка топологии датчиков при входе на закладку
	HTTP.on("/refluxSensorsSetLoad", handleRefluxSensorSetLoad);    // Отправка - Добавить датчики для процесса
	HTTP.on("/refluxSensorsSetSave", handleRefluxSensorSetSave);	// Прием выбранных датчиков
}

// Отправка топологии датчиков при входе на закладку
void handleRefluxSensorTpl() {
	int i, k;

	loadEepromReflux();
	if (processMode.allow == 0) {
		for (int i = 0; i < DS_Cnt; i++) {
			//allertSetTemperatureEn[i] = false;
			if (temperatureSensor[i].cutoff == true) temperatureSensor[i].allertValue = temperatureSensor[i].allertValueIn;
		}
	}

	String dataForWeb = "{";
	// датчики температуры
#if defined Debug_en
	Serial.println(""); Serial.println("Топология отправка:");
#endif
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				dataForWeb += "\"t" + String(i) + "\":{\"value\":" + String(temperatureSensor[k].data);
				dataForWeb += ",\"name\":\"" + String(temperatureSensor[k].name) + "\",\"color\":" + String(temperatureSensor[k].color);
				dataForWeb += ",\"member\":" + String(tpl2web.dsMember[k]) + ",\"delta\":" + String(tpl2web.dsDelta[k]);
				dataForWeb += ",\"cutoff\":" + String(tpl2web.dsCutoff[k]) + ",\"priority\":" + String(tpl2web.dsPriority[k]);
				dataForWeb += ",\"allertValue\":" + String(temperatureSensor[k].allertValueIn) + "},";
				break;
			}
			if (k < 7) k++;
			else break;
		}
#if defined Debug_en
		Serial.print(i); Serial.print(" "); Serial.println(k);
		Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
		Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
		Serial.print("Уставка: ");  Serial.println(temperatureSensor[k].allertValueIn);//Serial.println(temperatureSensor[k].allertValue);
#endif
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
	// остальные параметры
	dataForWeb += "\"stab\":" + String(timeStabilizationReflux) + ",\"point\":" + String(timeBoilTubeSetReflux);
	dataForWeb += ",\"number\":" + String(processMode.number) + ",\"powerHigh\":" + String(power.inPowerHigh) + ",\"powerLower\":" + String(power.inPowerLow) + "}";
	HTTP.send(200, "text/json", dataForWeb);
}
// Отправка - Добавить датчики для процесса
void handleRefluxSensorSetLoad() {
	int i, k;
	loadEepromReflux();
	String dataForWeb = "{";
	// датчики температуры
#if defined Debug_en
	Serial.println(""); Serial.println("Расстановка отправка:");
#endif
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
#if defined Debug_en
		Serial.print(i); Serial.print(" "); Serial.println(k);
		Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
		Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
#endif
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
	dataForWeb += "\"stab\":" + String(timeStabilizationReflux) + ",\"point\":" + String(timeBoilTubeSetReflux) + "}";
	HTTP.send(200, "text/json", dataForWeb);
}
// Прием выбранных датчиков
void handleRefluxSensorSetSave() {
	String arg;
	uint16_t index = 1400;
	int i, k;
	// парсим ответ от браузера в переменные
#if defined Debug_en
	Serial.println(""); Serial.println("Расстановка прием:");
#endif
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				arg = "t" + String(i);
				temperatureSensor[k].member = HTTP.arg(arg + "[member]").toInt();
				temperatureSensor[k].priority = HTTP.arg(arg + "[priority]").toInt();
				temperatureSensor[k].allertValue = HTTP.arg(arg + "[allertValue]").toFloat();
				temperatureSensor[k].delta = HTTP.arg(arg + "[delta]").toInt();
				temperatureSensor[k].cutoff = HTTP.arg(arg + "[cutoff]").toInt();
				break;
			}
			if (k < 7) k++;
			else break;
		}
#if defined Debug_en		
		Serial.print(i); Serial.print(" "); Serial.println(k);
		Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
		Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
#endif		
	}
	for (i = 0; i < 8; i++) {
		arg = "out" + String(i + 1);
		pwmOut[i].member = HTTP.arg(arg + "[member]").toInt();
	}
	for (i = 0; i < 4; i++) {
		arg = "in" + String(i + 1);
		adcIn[i].member = HTTP.arg(arg + "[member]").toInt();
	}
	timeStabilizationReflux = HTTP.arg("stab").toInt();
	timeBoilTubeSetReflux = HTTP.arg("point").toInt();
	HTTP.send(200, "text/json", "{\"result\":\"ok\"}");

	// сохраним в EEPROM
	EEPROM.begin(2048);
	EEPROM.write(index, 0x02); index++; // 2-й процесс = ректификация 1 байт
	// Датчики температуры
	for (i = 0; i < 8; i++) { // 64 байта
		EEPROM.write(index, temperatureSensor[i].member);  index++;
		EEPROM.write(index, temperatureSensor[i].priority);  index++;
		EEPROM_float_write(index, temperatureSensor[i].allertValueIn); index += 4;
		EEPROM.write(index, temperatureSensor[i].delta); index++;
		EEPROM.write(index, temperatureSensor[i].cutoff); index++;
	}
	for (i = 0; i < 8; i++) { // 8 байт
		EEPROM.write(index, pwmOut[i].member);  index++;
	}
	for (i = 0; i < 4; i++) { // 4 байта
		EEPROM.write(index, adcIn[i].member);  index++;
	}
	// для колонны
	index = 1488;
	EEPROM.write(index, timeStabilizationReflux); index++;
	EEPROM.write(index, timeBoilTubeSetReflux);
	
	EEPROM.end();
	delay(200);
}

void valveSet(uint8_t ch) {

	if (typeRefOfValwe < 3 && adcIn[0].member == 1 && adcIn[0].allert == true) {
		csOff(ch);
	}
	else {
		if (processMode.step < 6) {
			valveCiclePeriod = (unsigned long)headTimeCycle * 1000;
			timeOn = (valveCiclePeriod * headtimeOn) / 100;
		}
		else {
			valveCiclePeriod = (unsigned long)bodyTimeCycle * 1000;
			timeOn = (valveCiclePeriod * bodytimeOn) / 100;
			// применим декремент на уменьшение скорости отбора после N-го кол-ва Старт/Стопа
			if (decline != 0) {
				unsigned long calc = counterStartStop * decline;
				if (calc > 100) calc = 100;
				timeOn = timeOn - ((timeOn * calc) / 100);
			}
		}

		timeOff = valveCiclePeriod - timeOn;

		if (OnOff) {
			// клапан On
			if (timeValveMs < millis() + 500) {
				yield();
				while (timeValveMs > millis());
				csOff(ch);
				timeValveMs = millis() + timeOff;
				OnOff = false;
				// если следующий период очень мал
				if (timeValveMs < millis() + 500) {
					yield();
					while (timeValveMs > millis());
					csOn(ch);
					timeValveMs = millis() + timeOn;
					OnOff = true;
				}
			}
		}
		else {
			// клапан Off
			if (timeValveMs < millis() + 500) {
				yield();
				while (timeValveMs > millis());
				csOn(ch);
				timeValveMs = millis() + timeOn;
				OnOff = true;
				if (timeValveMs < millis() + 500) {
					yield();
					while (timeValveMs > millis());
					csOff(ch);
					timeValveMs = millis() + timeOff;
					OnOff = false;
				}
			}
		}
	}
}

uint16_t percentCalc(uint8_t data) {
	uint8_t cnt = data / 5;
	uint16_t percent = (percentCorrectSquare[cnt + 1] - percentCorrectSquare[cnt]) / 5;
	percent = percentCorrectSquare[cnt] + percent * (data - cnt * 5);
	return (percent / 5);
}

// ручной режим
void rfluxLoopMode_1() {
	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= 55.0 || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
				csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}


// просто контролируем температуры и датчики для индикации
		case 3: {

			// если прошло timeBoilTubeSetReflux минут, применим уставку
			if (processMode.timeStep >= (timeBoilTubeSetReflux * 60)) {
				settingBoilTube = temperatureSensor[DS_Tube].allertValueIn;
				if (settingBoilTube != 0) {
					if (reSetTemperatureStartPressure == true) {
						settingColumn = temperatureSensor[DS_Tube].data;
						pressureSensor.dataStart = pressureSensor.data;
						// температура кипения спирта при старте
						temperatureStartPressure = 78.14 - (760 - pressureSensor.dataStart)*0.037;
						reSetTemperatureStartPressure = false;
					}
					// температура кипения спирта текущее
					float temperatureCurrentPressure = 78.14 - (760 - pressureSensor.data)*0.037;
					// скорректированная температура отсечки
					temperatureSensor[DS_Tube].allertValue = settingColumn + settingBoilTube + temperatureCurrentPressure - temperatureStartPressure;
				}
				else temperatureSensor[DS_Tube].allertValue = 0;
			}
			else temperatureSensor[DS_Tube].allertValue = 0;

			// если сработала уставка - пищим
			if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				settingAlarm = true;
			}
			// контроль датчика уровня
			else if (adcIn[0].allert == true && settingAlarm == false) settingAlarm = true;
			else {
				temperatureSensor[DS_Tube].allert = false;
				settingAlarm = false;
			}


			// Пищалка для WEB и самой автоматики
			//if (timeAllertInterval > millis()) settingAlarm = true;
			//else settingAlarm = false;

			// Проверка датчиков безопасности
			if (!errA && !errT) check_Err();
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


			



			// стоп по Т куба
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timePauseOff = millis() + 120000;			// время на отключение
				timeAllertInterval = millis() + 10000;		// время для зв. сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				processMode.timeStep = 0;
				stepNext = 0;
				nameProcessStep = "Процесс закончен";
				processMode.step = 4;
			}
			break;
		}
		case 4: {
			if (millis() >= timePauseOff || stepNext == 1) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);								// закрыли клапан слива ПБ
				if (pwmOut[2].member == 1) csOff(PWM_CH3);								// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;						// вышли из режима ректификации
				processMode.step = 0;						// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
				stepNext = 0;
			}
			break;
		}
	}
}
// Прима - головы по жидкости, тело по пару
// PWM_CH1 или PWM_CH2 - клапан отбора голов
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - ПБ
void rfluxLoopMode_2() {
	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			// Закрыли отбор по пару
			setPWM(PWM_CH5, 0, 10);
			bodyPrimaPercent = bodyPrimaPercentStart;
			counterStartStop = 0;
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= 55.0 || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
				csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Стабилизация колонны";
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// ждем окончание стабилизации 20 минут
		case 3: {
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Отбор голов";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				processMode.step = 5;		// перешли на следующий шаг алгоритма отбора голов
				stepNext = 0;
			}
			break;
		}
// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
		case 5: {
			if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
				if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
					timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
					processMode.step = 6;		// перешли на следующий шаг алгоритма
					stepNext = 0;
				}
			}
			else countHaedEnd = millis() + 10000;

			if (processMode.step == 6) {
				if (typeRefOfValwe == 3 || typeRefOfValwe == 1) csOff(PWM_CH1);	// закрыли клапан отбора голов
				else if (typeRefOfValwe == 2) csOff(PWM_CH2);	// закрыли  клапан отбора голов
				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				// открыть шаровый кран отбора по пару
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercentStart);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				///////////////////////////////////
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				nameProcessStep = "Отбор тела";
			}
			// рулим клапаном отбора голов
			else if (typeRefOfValwe == 3 || typeRefOfValwe == 1) valveSet(PWM_CH1);
			else if (typeRefOfValwe == 2) valveSet(PWM_CH2);
			break;
		}
// ждем окончание по достижению температуры в кубе или пищим при достижении уставки
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timePauseOff = millis() + 120000;			// время на отключение
				timeAllertInterval = millis() + 10000;		// время для зв. сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				processMode.timeStep = 0;
				stepNext = 0;
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
				nameProcessStep = "Процесс закончен";
				processMode.step = 7;						// перешли на следующий шаг алгоритма
			}

			// контроль Т в царге
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				csOff(PWM_CH1);
				csOff(PWM_CH2);								// закрыли клапан отбора
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;

				// расчет на сколько надо после старт/стопа открыть шаровый кран
				bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop);
				if (bodyPrimaPercent < bodyPrimaPercentStop) bodyPrimaPercent = bodyPrimaPercentStop;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);
				
				// если есть польский буфер, работаем до первого стопа
				if (pwmOut[3].member == 1) {
					power.heaterStatus = 0;							// выключили ТЭН
					power.heaterPower = 0;							// установили мощность на ТЭН 0 %
					timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
					temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
					csOn(PWM_CH4);								// включаем клапан слива ПБ
					processMode.timeStep = 0;
					// Закрыли отбор по пару
					setPWM(PWM_CH5, 0, 10);
					nameProcessStep = "Процесс закончен";
					timePauseOff = 60000 * 20 + millis();
					processMode.step = 7;						// перешли на следующий шаг алгоритма
				}
			}

			// без ПБ рулим по уставке
			if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
				if (adcIn[0].member == 1 && adcIn[0].allert == true) setPWM(PWM_CH5, 0, 10); // емкость полная
				/*else {
					// расчет на сколько надо после старт/стопа открыть шаровый кран
					bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop);
					if (bodyPrimaPercent < bodyPrimaPercentStop) bodyPrimaPercent = bodyPrimaPercentStop;
					bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);
					setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				}*/
			}

			if (bodyValveSet == true && processMode.step != 7) {
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
				if (counterStartStop == 0) nameProcessStep = "Отбор тела";
				else nameProcessStep = "Отбор тела, старт/стопов - " + String(counterStartStop);


				// расчет на сколько надо после старт/стопа открыть шаровый кран
				bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop);
				if (bodyPrimaPercent < bodyPrimaPercentStop) bodyPrimaPercent = bodyPrimaPercentStop;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
			}
			else {
				nameProcessStep = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
			}
			break;
		}
// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			// Закрыли отбор по пару, если стоп руками
			setPWM(PWM_CH5, 0, 10);
			if (millis() >= timePauseOff || stepNext == 1) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);								// закрыли клапан слива ПБ
				if (pwmOut[2].member == 1) csOff(PWM_CH3);								// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;						// вышли из режима ректификации
				processMode.step = 0;						// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
				stepNext = 0;
			}
			break;
		}
	}
}






// РК по пару
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - ПБ
void rfluxLoopMode_3() {
	switch (processMode.step) {
		// пришли при старте ректификации
		case 0: {
			if (pwmOut[3].member == 1) csOn(PWM_CH4);		// открыли клапан слива ПБ
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
		// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= 55.0 || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
				csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Стабилизация колонны";
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
		// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
				// ждем окончание стабилизации 20 минут
		case 3: {
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Отбор голов";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
				// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				processMode.step = 5;		// перешли на следующий шаг алгоритма отбора голов
				stepNext = 0;
			}
			break;
		}
				// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
		case 5: {
			if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
				if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
					timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
					processMode.step = 6;		// перешли на следующий шаг алгоритма
					stepNext = 0;
				}
			}
			else countHaedEnd = millis() + 10000;

			if (processMode.step == 6) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				// открыть шаровый кран отбора по пару на тело
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercentStart);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				///////////////////////////////////
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				nameProcessStep = "Отбор тела";
			}
			// рулим краном отбора голов
			else {
				bodyPrimaPercentSet = percentCalc(headSteamPercent);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
			}
			break;
		}
		// ждем окончание по достижению температуры в кубе или пищим при достижении уставки
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timePauseOff = millis() + 120000;			// время на отключение
				timeAllertInterval = millis() + 10000;		// время для зв. сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				processMode.timeStep = 0;
				stepNext = 0;
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
				nameProcessStep = "Процесс закончен";
				processMode.step = 7;						// перешли на следующий шаг алгоритма
			}

			// контроль Т в царге

			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;

				// расчет на сколько надо после старт/стопа открыть шаровый кран
				bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop);
				if (bodyPrimaPercent < bodyPrimaPercentStop) bodyPrimaPercent = bodyPrimaPercentStop;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);

				// если есть польский буфер, работаем до первого стопа
				if (pwmOut[3].member == 1) {
					power.heaterStatus = 0;							// выключили ТЭН
					power.heaterPower = 0;							// установили мощность на ТЭН 0 %
					timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
					temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
					csOn(PWM_CH4);								// включаем клапан слива ПБ
					processMode.timeStep = 0;
					// Закрыли отбор по пару
					setPWM(PWM_CH5, 0, 10);
					nameProcessStep = "Процесс закончен";
					timePauseOff = 60000 * 20 + millis();
					processMode.step = 7;						// перешли на следующий шаг алгоритма
				}
			}

			// без ПБ рулим по уставке
			if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
				if (adcIn[0].member == 1 && adcIn[0].allert == true) setPWM(PWM_CH5, 0, 10); // если емкость полная
				else setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
			}

			if (bodyValveSet == true && processMode.step != 7) {
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
				if (counterStartStop == 0) nameProcessStep = "Отбор тела";
				else nameProcessStep = "Отбор тела, старт/стопов - " + String(counterStartStop);
			}
			else {
				nameProcessStep = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
			}

			///////////////////////////////////
			break;
		}
				// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			// Закрыли отбор по пару, если стоп руками
			setPWM(PWM_CH5, 0, 10);
			if (millis() >= timePauseOff || stepNext == 1) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);								// закрыли клапан слива ПБ
				if (pwmOut[2].member == 1) csOff(PWM_CH3);								// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;						// вышли из режима ректификации
				processMode.step = 0;						// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
				stepNext = 0;
			}
			break;
		}
	}
}




// РК по жидкости, универсальный алгоритм
// PWM_CH1 и PWM_CH2 - клапаны отбора
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - Польский буфер
void rfluxLoopMode_4() {
	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			counterStartStop = 0;
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= 55.0 || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);				// включаем клапан подачи воды
				csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Стабилизация колонны";
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// ждем окончание стабилизации 20 минут
		case 3: {
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Отбор голов";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				processMode.step = 5;		// перешли на следующий шаг алгоритма отбора голов
				stepNext = 0;
			}
			break;
		}
// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
		case 5: {
			if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
				if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
					timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
					processMode.step = 6;		// перешли на следующий шаг алгоритма
					stepNext = 0;
				}
			}
			else countHaedEnd = millis() + 10000;

			if (processMode.step == 6) {
				if (typeRefOfValwe == 3) csOff(PWM_CH1);	// закрыли клапан отбора голов т.к. 2 клапана на отбор
				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				nameProcessStep = "Отбор тела";
			}
			// рулим клапаном отбора голов
			else if (typeRefOfValwe == 3 || typeRefOfValwe == 1) valveSet(PWM_CH1);
			else if (typeRefOfValwe == 2) valveSet(PWM_CH2);
			break;
		}
// ждем окончание по достижению температуры в кубе и рулим клапаном отбора
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;							// выключили ТЭН
				power.heaterPower = 0;							// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				csOff(PWM_CH1);
				csOff(PWM_CH2);								// закрыли клапан отбора
				if (pwmOut[3].member == 1) {
					timePauseOff = 60000 * 20 + millis();
					csOn(PWM_CH4);								// включаем клапан слива ПБ
				}
				else timePauseOff = 60000 * 2 + millis();
				processMode.timeStep = 0;
				nameProcessStep = "Процесс закончен";
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				csOff(PWM_CH1);
				csOff(PWM_CH2);								// закрыли клапан отбора
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;
				// если есть польский буфер, работаем до первого стопа
				if (pwmOut[3].member == 1) {
					power.heaterStatus = 0;							// выключили ТЭН
					power.heaterPower = 0;							// установили мощность на ТЭН 0 %
					timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
					temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
					csOn(PWM_CH4);								// включаем клапан слива ПБ
					processMode.timeStep = 0;
					nameProcessStep = "Процесс закончен";
					timePauseOff = 60000 * 20 + millis();
					processMode.step = 7;						// перешли на следующий шаг алгоритма
				}
			}

			// без ПБ рулим по уставке
			if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube && pwmOut[3].member == 0) {
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
			}

			if (bodyValveSet == true && processMode.step != 7) {
				if (typeRefOfValwe == 1) valveSet(PWM_CH1);
				else if (typeRefOfValwe == 2 || typeRefOfValwe == 3) valveSet(PWM_CH2);
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
				if (counterStartStop == 0) nameProcessStep = "Отбор тела";
				else nameProcessStep = "Отбор тела, старт/стопов - " + String(counterStartStop);
			}
			else {
				nameProcessStep = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				csOff(PWM_CH1);
				csOff(PWM_CH2);
			}

			break;
		}
// после завершения процесса ждем N мин. и выключаем клапана и пищалку
		case 7: {
			if (millis() >= timePauseOff || stepNext == 1) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);								// закрыли клапан слива ПБ
				if (pwmOut[2].member == 1) csOff(PWM_CH3);								// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;						// вышли из режима ректификации
				processMode.step = 0;						// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
				stepNext = 0;
			}
			break;
		}
	}
}

// БК регулировка охлаждением
// PWM_CH1
// PWM_CH2 - клапан доп.подачи воды
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - Польский буфер
void rfluxLoopMode_5() {
	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			counterStartStop = 0;
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= 55.0 || stepNext == 1) {
				csOn(PWM_CH3);
				csOn(PWM_CH2);				// включаем клапан доп. подачи воды
				csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
				power.heaterPower = power.inPowerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Стабилизация колонны";
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// ждем окончание стабилизации 20 минут
		case 3: {
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				nameProcessStep = "Отбор голов";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				processMode.step = 5;		// перешли на следующий шаг алгоритма отбора голов
				countHaedEnd = millis() + 10000;
				stepNext = 0;
			}
			break;
		}
// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
		case 5: {
			if (adcIn[0].allert == true) {
				if (countHaedEnd <= millis()) {	// антирдебезг 10 сек. :)
					timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
					processMode.step = 6;		// перешли на следующий шаг алгоритма
				}
			}
			else countHaedEnd = millis() + 10000;

			if (processMode.step == 6 || stepNext == 1) {
				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				csOff(PWM_CH2);		// закрыли клапан доп. подачи воды
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				nameProcessStep = "Отбор тела";
				stepNext = 0;
			}
			break;
		}
// ждем окончание по достижению температуры в кубе и рулим клапаном доп. охлаждения
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;							// выключили ТЭН
				power.heaterPower = 0;							// установили мощность на ТЭН 0 %
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.timeStep = 0;
				nameProcessStep = "Процесс закончен";
				if (pwmOut[3].member == 1) {
					timePauseOff = 60000 * 20 + millis();
					csOn(PWM_CH4);
				}
				else timePauseOff = 60000 * 2 + millis();
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				stepNext = 0;
				break;
			}
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) {
					counterStartStop++;
					nameProcessStep = "Отбор тела, " + String(counterStartStop) + "-й стоп";
					bodyValveSet = false;
				}
				csOn(PWM_CH2);								// открыли клапан доп. подачи воды
			}

			if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				csOff(PWM_CH2);	// закрыли клапан доп. подачи воды
				if (counterStartStop == 0) nameProcessStep = "Отбор тела";
				else nameProcessStep = "Отбор тела, старт/стопов - " + String(counterStartStop);
				bodyValveSet = true;
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
			}

			break;
		}
// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			if (millis() >= timePauseOff || stepNext == 1) {
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				processMode.allow = 0;						// вышли из режима ректификации
				processMode.step = 0;						// обнулили шаг алгоритма
				commandWriteSD = "Процесс завершен";
				commandSD_en = true;
				stepNext = 0;
			}
			break;
		}
	}
}

// БК регулировка мощностью
void rfluxLoopMode_6() {
	rfluxLoopMode_1();
}

// если запущена ректификация
void refluxLoop() {

	if (processMode.step == 0) {
		startWriteSD = true;
		errA = false;
		errT = false;
		temperatureSensor[DS_Tube].allertValue = 0;
		loadEepromReflux();
#if defined TFT_Display
		// подготовка данных для вывода на TFT
		csOn(TFT_CS);
		tftStartForGraph();
		displayTimeInterval = millis() + 1000;
		DefCubOut = Display_out_temp;
		csOff(TFT_CS);
#endif
		tempBigOut = 2;
		if (pwmOut[3].member == 1) csOn(PWM_CH4);	// клапан в буфер открыт
		power.heaterStatus = 1;						// включили нагрев
		csOn(PWM_CH6);								// дополнительный разгон на ТЭНы
		power.heaterPower = power.inPowerHigh;		// установили мощность на ТЭН 100 %
		processMode.timeStep = 0;
		stepNext = 0;
		countHaedEnd = 0;
		beepEnd = false;
		reSetTemperatureStartPressure = true;
		if (processMode.number != 0) nameProcessStep = "Нагрев куба";
		else {
			settingBoilTube = 0;
			if (pwmOut[0].member == 1) csOn(PWM_CH1);				// открыть клапан отбора голов
			if (pwmOut[1].member == 1) csOn(PWM_CH2);				// открыть клапан отбора тела
			if (pwmOut[2].member == 1) csOn(PWM_CH3);				// включаем клапан подачи воды
			nameProcessStep = "Ручной режим";
		}
	}
	else if (stepNext == 1) {
		commandWriteSD = "WebSend: Пропустить шаг";
		commandSD_en = true;
	}

	// Мощности ТЭНа (разогрев / работа)
	if (processMode.step == 1) {
		if (power.heaterPower != power.inPowerHigh) power.heaterPower = power.inPowerHigh;
	}
	else if (processMode.step < 7) {
		if (power.heaterPower != power.inPowerLow) power.heaterPower = power.inPowerLow;
	}
	else power.heaterPower = 0;

	if (processMode.number > 0) {

		// Коррекция уставки от давления
		if (processMode.step == 6) {
			// если прошло timeBoilTubeSetReflux минут, применим уставку
			if (processMode.timeStep >= (timeBoilTubeSetReflux * 60)) {
				settingBoilTube = temperatureSensor[DS_Tube].allertValueIn;
				if (settingBoilTube != 0) {
					if (reSetTemperatureStartPressure == true) {
						settingColumn = temperatureSensor[DS_Tube].data;
						pressureSensor.dataStart = pressureSensor.data;
						// температура кипения спирта при старте
						temperatureStartPressure = 78.14 - (760 - pressureSensor.dataStart)*0.037;
						reSetTemperatureStartPressure = false;
					}
					// температура кипения спирта текущее
					float temperatureCurrentPressure = 78.14 - (760 - pressureSensor.data)*0.037;
					// скорректированная температура отсечки
					temperatureSensor[DS_Tube].allertValue = settingColumn + settingBoilTube + temperatureCurrentPressure - temperatureStartPressure;
				}
				else temperatureSensor[DS_Tube].allertValue = 0;
			}
			else temperatureSensor[DS_Tube].allertValue = 0;
		}

		// Пищалка для WEB и самой автоматики
		if (timeAllertInterval > millis()) settingAlarm = true;
		else settingAlarm = false;

		// Проверка датчиков безопасности
		if (processMode.step != 7 && !errA && !errT) check_Err();
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
	}

	if (processMode.number > 0 && processMode.number < 4 && processMode.step == 6) {
		// завершение отбора по времени закрытого состояния клапана на отборе тела
		if (counterStartStop > 0 && processMode.timeStep >= (timeStabilizationReflux * 60 + bodyTimeOffCount)) {
			stop_Err();
			nameProcessStep = "Стоп по времени Старт/Стоп";
		}
	}

	// Выключение повышенного напряжения на клапана
	/*if (timeSetHighVoltage < millis()) {
		if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 4096, 0);
		else pwm.setPWM(PWM_CH9, 0, 4096);
	}*/

	// Уходим на выбранный алгоритм
	switch (processMode.number) {
		case 0: rfluxLoopMode_1(); break; // ручной режим, только сигнализация
		case 1: rfluxLoopMode_2(); break; // Прима - головы по жидкости, тело по пару
		case 2: rfluxLoopMode_3(); break; // РК с отбором по пару
		case 3: rfluxLoopMode_4(); break; // РК с отбором по жидкости
		case 4: rfluxLoopMode_5(); break; // БК регулировка охлаждением
		case 5: rfluxLoopMode_6(); break; // БК регулировка мощностью
	}
}
