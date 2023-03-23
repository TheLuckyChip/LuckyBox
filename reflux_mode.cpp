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
bool step_new_body;
#ifndef valve_4_mode
bool step_new_end;
#endif
unsigned long timeValveMs;
uint8_t bodyPrimaPercent;
uint16_t bodyPrimaPercentSet;
uint8_t powerLow;
// Для плавного повышения мощности при стабилизации колпачковой колонны
uint8_t stepPowerCount;
uint8_t stepPowerPercent;
uint8_t stepPowerStart;
unsigned long stepPowerTime;

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

		timeStabilizationReflux = EEPROM.read(index); index++;					// 1488
		if (timeStabilizationReflux > 120) timeStabilizationReflux = 20;
		timeBoilTubeSetReflux = EEPROM.read(index); index++;
		if (timeBoilTubeSetReflux > 60) timeBoilTubeSetReflux = 10;

		headSteamPercent = EEPROM.read(index); index++;
		if (headSteamPercent > 200) headSteamPercent = 60;
		bodyPrimaPercentStart = EEPROM.read(index); index++;
		if (bodyPrimaPercentStart > 200) bodyPrimaPercentStart = 200;
		bodyPrimaPercentStop = EEPROM.read(index); index++;
		if (bodyPrimaPercentStop > 200) bodyPrimaPercentStop = 80;
		bodyPrimaDecline = EEPROM.read(index); index++;
		if (bodyPrimaDecline > 30) bodyPrimaDecline = 5;

		RefluxTransitionTemperature = EEPROM.read(index); index++;
		if (RefluxTransitionTemperature > 100) RefluxTransitionTemperature = 55;
		TapCorrectionWeb = EEPROM.read(index);
		if (TapCorrectionWeb < 50 || TapCorrectionWeb > 250) TapCorrectionWeb = 120;
		TapCorrection = (float)TapCorrectionWeb / 100;

		power.inPowerHigh = EEPROM.read(1497);
		if (power.inPowerHigh > 100) power.inPowerHigh = 100;
		power.inPowerLow = EEPROM.read(1498);
		if (power.inPowerLow > 100) power.inPowerLow = 65;
		processMode.number = EEPROM.read(1499);
		if (processMode.number > 6) processMode.number = 0;

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
		bodyPrimaPercentStart = 200;
		bodyPrimaPercentStop = 80;
		bodyPrimaDecline = 5;
		timeStabilizationReflux = 20;
		timeBoilTubeSetReflux = 10;
		
		RefluxTransitionTemperature = 55;
		TapCorrectionWeb = 120;
		TapCorrection = 1.20;
		
		power.inPowerHigh = 100;
		power.inPowerLow = 65;
		processMode.number = 0;
	}
	EEPROM.end();
	for (int i = 0; i < DS_Cnt; i++) {
		//allertSetTemperatureEn[i] = false;
		if (temperatureSensor[i].cutoff == true) temperatureSensor[i].allertValue = temperatureSensor[i].allertValueIn;
	}

	//if (processMode.step == 0) processMode.timeStart = time(nullptr);
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
	if (RU) Serial.println(""); Serial.println("Топология отправка:");
	else Serial.println(""); Serial.println("Topology sends:");
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
		if (RU) {
			Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
			Serial.print("Уставка: ");  Serial.println(temperatureSensor[k].allertValueIn);
		}
		else {
			Serial.print("Number: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Member: ");  Serial.println(temperatureSensor[k].member);
			Serial.print("Allert value: ");  Serial.println(temperatureSensor[k].allertValueIn);
		}
#endif
	}
	// выходы ШИМ
	for (i = 0; i < 4; i++) {
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
	if (RU) Serial.println(""); Serial.println("Расстановка отправка:");
	else Serial.println(""); Serial.println("Placement (out):");
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
		if (RU) {
			Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
		}
		else {
			Serial.print("Number: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Member: ");  Serial.println(temperatureSensor[k].member);
		}
#endif
	}
	// выходы ШИМ
	for (i = 0; i < 4; i++) {
		dataForWeb += "\"out" + String(i + 1) + "\":{\"value\":" + String(pwmOut[i].data) + ",\"name\":\"" + String(pwmOut[i].name) + "\",\"member\":" + String(pwmOut[i].member) + "},";
	}
	// входы АЦП
	dataForWeb += "\"in1\":{\"value\":" + String(adcIn[0].data) + ",\"name\":\"" + String(adcIn[0].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in2\":{\"value\":" + String(adcIn[1].data) + ",\"name\":\"" + String(adcIn[1].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in3\":{\"value\":" + String(adcIn[2].data) + ",\"name\":\"" + String(adcIn[2].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"in4\":{\"value\":" + String(adcIn[3].data) + ",\"name\":\"" + String(adcIn[3].name) + "\",\"member\":" + String(adcIn[0].member) + "},";
	dataForWeb += "\"stab\":" + String(timeStabilizationReflux) + ",\"point\":" + String(timeBoilTubeSetReflux) + ",";
	dataForWeb += "\"transitionTemperature\":" + String(RefluxTransitionTemperature) + ",\"tapCorrection\":" + String(TapCorrectionWeb) + "}";
	HTTP.send(200, "text/json", dataForWeb);
}
// Прием выбранных датчиков
void handleRefluxSensorSetSave() {
	String arg;
	uint16_t index = 1400;
	int i, k;
	// парсим ответ от браузера в переменные
#if defined Debug_en
	if (RU) Serial.println(""); Serial.println("Расстановка прием:");
	else Serial.println(""); Serial.println("Placement (in):");
#endif
	for (i = 1; i <= DS_Cnt; i++) {
		k = 0;
		while (1) {
			if (temperatureSensor[k].num == i) {
				arg = "t" + String(i);
				temperatureSensor[k].member = HTTP.arg(arg + "[member]").toInt();
				temperatureSensor[k].priority = HTTP.arg(arg + "[priority]").toInt();
				//temperatureSensor[k].allertValue = HTTP.arg(arg + "[allertValue]").toFloat();
				temperatureSensor[k].delta = HTTP.arg(arg + "[delta]").toInt();
				temperatureSensor[k].cutoff = HTTP.arg(arg + "[cutoff]").toInt();
				break;
			}
			if (k < 7) k++;
			else break;
		}
#if defined Debug_en		
		Serial.print(i); Serial.print(" "); Serial.println(k);
		if (RU) {
			Serial.print("Номер: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Участвует: ");  Serial.println(temperatureSensor[k].member);
		}
		else {
			Serial.print("Number: ");  Serial.println(temperatureSensor[k].num);
			Serial.print("Member: ");  Serial.println(temperatureSensor[k].member);
		}
#endif		
	}
	for (i = 0; i < 4; i++) {
		arg = "out" + String(i + 1);
		pwmOut[i].member = HTTP.arg(arg + "[member]").toInt();
	}
	for (i = 0; i < 4; i++) {
		arg = "in" + String(i + 1);
		adcIn[i].member = HTTP.arg(arg + "[member]").toInt();
	}
	timeStabilizationReflux = HTTP.arg("stab").toInt();
	timeBoilTubeSetReflux = HTTP.arg("point").toInt();
	RefluxTransitionTemperature = HTTP.arg("transitionTemperature").toInt();
	TapCorrectionWeb = HTTP.arg("tapCorrection").toInt();
	TapCorrection = (float)TapCorrectionWeb / 100;
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
	for (i = 0; i < 4; i++) { // было 8 байт
		EEPROM.write(index, pwmOut[i].member);  index++;
	}
	index += 4;
	for (i = 0; i < 4; i++) { // 4 байта
		EEPROM.write(index, adcIn[i].member);  index++;
	}
	// для колонны
	index = 1488;
	EEPROM.write(index, timeStabilizationReflux); index++;
	EEPROM.write(index, timeBoilTubeSetReflux);
	index = 1494;
	EEPROM.write(index, RefluxTransitionTemperature); index++;
	EEPROM.write(index, TapCorrectionWeb);
	
	EEPROM.end();
}

void valveSet(uint8_t ch) {
	//if (typeRefOfValwe < 3 && adcIn[0].member == 1 && adcIn[0].allert == true) {
	if (adcIn[0].member == 1 && adcIn[0].allert == true && alertLevelEnable == true) { // прекращение отбора по датчику уровня
		csOff(ch);
	}
	else {
		if (processMode.step < 6) { // головы
			valveCiclePeriod = (unsigned long)headTimeCycle * 1000;
			timeOn = (valveCiclePeriod * headtimeOn) / 100;
		}
		else { // тело
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
			if (timeValveMs < millis() + 100) {
				yield();
				while (timeValveMs > millis());
				csOff(ch);
				timeValveMs = millis() + timeOff;
				OnOff = false;
				// если следующий период очень мал
				if (timeValveMs < millis() + 100) {
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
			if (timeValveMs < millis() + 100) {
				yield();
				while (timeValveMs > millis());
				csOn(ch);
				timeValveMs = millis() + timeOn;
				OnOff = true;
				if (timeValveMs < millis() + 100) {
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

// перерасчет процентов в значение ШИМ для шарового крана
// data = 200 это 100% т.к. шаг изменения 0.5
uint16_t percentCalc(uint8_t data) {
	uint8_t cnt = data / 10;
	uint16_t percent_1 = (uint16_t)((float)(TapCorrection * percentCorrectSquare[cnt]) / 10);
	uint16_t percent_2 = (uint16_t)((float)(TapCorrection * percentCorrectSquare[cnt + 1]) / 10);
	uint16_t percent = (uint16_t)((float)(TapCorrection * percentCorrectSquare[cnt])) + (percent_2 - percent_1) * (data - cnt * 10);
	return (percent / 5);
}

// ручной режим
void rfluxLoopMode_1() {
	if (processMode.step < 4) {
		// контроль датчиков безопасности
		if (processMode.step != 4 && !errA && !errT) check_Err();
		if (processMode.step != 4 && timePauseErrA <= millis()) {
			errA = false; check_Err();
			if (errA) {
				stop_Err();
				if (RU) name_Process_Step = "Стоп по аварии ADC > " + String(adcIn[numCrashStop].name);
				else name_Process_Step = "Accident stop from ADC > " + String(adcIn[numCrashStop].name);
			}
		}
		if (processMode.step != 4 && timePauseErrT <= millis()) {
			errT = false; check_Err();
			if (errT) {
				stop_Err();
				if (RU) name_Process_Step = "Стоп по аварии T > " + String(temperatureSensor[numCrashStop].name);
				else name_Process_Step = "Accident stop from T > " + String(temperatureSensor[numCrashStop].name);
			}
		}
	}
	// выключение звука
	if (alertEnable == false || (errA == false && errT == false && adcIn[0].allert == false
		&& temperatureSensor[DS_Tube].allert == false && timeAllertInterval <= millis())) settingAlarm = false;

	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			// контроль температуры в тубе для перехода на отбор
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек
				settingAlarm = true;
				processMode.timeStep = 0;
				processMode.step = 2;		// перешли на следующий шаг алгоритма
				stepNext = 0;
				if (RU) name_Process_Step = "Ручной режим, стабилизация колонны";
				else name_Process_Step = "Manual mode, column stabilization";
			}

			break;
		}
// пищалка на 10 сек. и стабилизация
		case 2: {
			if (processMode.timeStep >= (timeStabilizationReflux * 60) - 10) settingAlarm = true;
			if (processMode.timeStep >= timeStabilizationReflux * 60 || stepNext == 1) {
				settingAlarm = false;
				processMode.step = 3;	// перешли на следующий шаг алгоритма
				stepNext = 0;
				if (RU) name_Process_Step = "Ручной режим, отбор";
				else name_Process_Step = "Manual mode, picking alcohol";
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
			else {
				temperatureSensor[DS_Tube].allert = false;
			}

			// стоп по Т куба
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				//timePauseOff = millis() + 120000;			// время на отключение
				timeAllertInterval = millis() + 10000;		// время для зв. сигнала
				settingAlarm = true;
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				processMode.timeStep = 0;
				stepNext = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 4;
			}

			break;
		}
		case 4: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
	// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
			else { // просто вывод строки с датчиками, клапанами и мощностью

			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				//setPWM(PWM_CH5, 0, 10);		// закрыть шаровый кран
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
			}
			break;
		}
	}
}

// Прима - головы по жидкости, тело по пару
// PWM_CH1 или PWM_CH2 - клапан отбора голов
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - ПБ или УНО
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
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Стабилизация колонны";
				else name_Process_Step = "Column stabilization";
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
#ifdef columnSoftStart
			// плавный подъем температуры
			if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
				stepPowerStart = powerLow / 2;
				stepPowerPercent = powerLow / 20;
				if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
					stepPowerCount++;
					stepPowerTime += (timeStabilizationReflux * 3);
				}
				power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
				if (power.heaterPower > powerLow) power.heaterPower = powerLow;
			}
			else {
				power.heaterPower = powerLow;
			}
#else
			power.heaterPower = powerLow;
#endif
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Отбор голов";
				else name_Process_Step = "Picking heads";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				if (pwmOut[1].member == 1) csOn(PWM_CH2); // доп. клапан отбора голов, если есть
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
				csOff(PWM_CH1);	// закрыли клапан отбора голов
				csOff(PWM_CH2);	// закрыли доп. клапан отбора голов в раздельные емкости
//				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				// открыть шаровый кран отбора по пару
				bodyPrimaPercent = bodyPrimaPercentStart;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercentStart);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				///////////////////////////////////
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				if (RU) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Picking hearts";
			}
			// рулим клапаном отбора голов
			else if (pwmOut[0].member == 1) valveSet(PWM_CH1);
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
				if (stepNext == 1) numOkStop = 0;
				else numOkStop = 1;
				stepNext = 0;
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				break;
			}

			// контроль Т в царге
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				//csOff(PWM_CH1);
				//csOff(PWM_CH2);								// закрыли клапан отбора
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;
				
				// если есть польский буфер, работаем до первого стопа
#ifdef oneStop
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//				csOn(PWM_CH4);								// включаем клапан слива ПБ
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
				break;
#endif
			}

			// без ПБ рулим по уставке
			if (settingBoilTube == 0 || temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				temperatureSensor[DS_Tube].allert = false;
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
			}

			if (adcIn[0].member == 1 && adcIn[0].allert == true && alertLevelEnable == true) {
				setPWM(PWM_CH5, 0, 10); // емкость полная
				bodyValveSet = false;
			}
			else if (counterStartStop == 0) bodyValveSet = true;

			if (bodyValveSet == true && processMode.step != 7) {
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
				if (RU) {
					if (counterStartStop == 0) name_Process_Step = "Отбор тела";
					else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
				}
				else {
					if (counterStartStop == 0) name_Process_Step = "Picking hearts";
					else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
				}
				// расчет на сколько надо после старт/стопа открыть шаровый кран
				if (bodyPrimaPercentStart >= (uint16_t)bodyPrimaDecline * counterStartStop * 2) {
					bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop * 2);
					if (bodyPrimaPercentStop > bodyPrimaPercent) bodyPrimaPercent = bodyPrimaPercentStop;
				}
				else bodyPrimaPercent = bodyPrimaPercentStop;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);	// Открываем шаровый кран
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
			}
			else {
				if (bodyPrimaPercent == 0) {//if (bodyPrimaPercent <= bodyPrimaPercentStop) {
					setPWM(PWM_CH5, 0, 10);							// Закрыли отбор по пару
					power.heaterStatus = 0;						// выключили ТЭН
					power.heaterPower = 0;						// установили мощность на ТЭН 0 %
					timePauseOff = millis() + 120000;			// время на отключение
					timeAllertInterval = millis() + 10000;		// время для зв. сигнала
					processMode.timeStep = 0;
					stepNext = 0;
					if (RU) name_Process_Step = "Процесс закончен";
					else name_Process_Step = "Process is over";
					settingAlarm = true;
					processMode.step = 7;						// перешли на следующий шаг алгоритма
					numOkStop = 3;
				}
				else {
					if (counterStartStop != 0 && RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
					else if (counterStartStop != 0) name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
					setPWM(PWM_CH5, 0, 10);							// Закрыли отбор по пару
				}
			}
			break;
		}
// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				setPWM(PWM_CH5, 0, 10);		// закрыть шаровый кран
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				temperatureSensor[DS_Tube].allert = false;
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
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
//			if (pwmOut[3].member == 1) csOn(PWM_CH4);		// открыли клапан слива ПБ
			processMode.timeStep = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
		// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Стабилизация колонны";
				else name_Process_Step = "Column stabilization";
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
#ifdef columnSoftStart
			// плавный подъем температуры
			if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
				stepPowerStart = powerLow / 2;
				stepPowerPercent = powerLow / 20;
				if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
					stepPowerCount++;
					stepPowerTime += (timeStabilizationReflux * 3);
				}
				power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
				if (power.heaterPower > powerLow) power.heaterPower = powerLow;
			}
			else {
				power.heaterPower = powerLow;
			}
#else
			power.heaterPower = powerLow;
#endif
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Отбор голов";
				else name_Process_Step = "Picking heads";
				processMode.step = 4;	// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
				// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				if (pwmOut[1].member == 1) csOn(PWM_CH2); // доп. клапан отбора голов, если есть
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
				csOff(PWM_CH2);
//				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				// открыть шаровый кран отбора по пару на тело
				bodyPrimaPercent = bodyPrimaPercentStart;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercentStart);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				///////////////////////////////////
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				if (RU) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Picking hearts";
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
				if (stepNext == 1) numOkStop = 0;
				else numOkStop = 1;
				stepNext = 0;
				// Закрыли отбор по пару
				setPWM(PWM_CH5, 0, 10);
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				break;
			}

			// контроль Т в царге
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;

				// если есть польский буфер, работаем до первого стопа
#ifdef oneStop
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//				csOn(PWM_CH4);								// включаем клапан слива ПБ
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
				break;
#endif
			}

			// без ПБ рулим по уставке
			if (settingBoilTube == 0 || temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				temperatureSensor[DS_Tube].allert = false;
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
			}

			if (adcIn[0].member == 1 && adcIn[0].allert == true && alertLevelEnable == true) {
				setPWM(PWM_CH5, 0, 10); // емкость полная
				bodyValveSet = false;
			}
			else if (counterStartStop == 0) bodyValveSet = true;

			if (bodyValveSet == true && processMode.step != 7) {
				bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
				if (RU) {
					if (counterStartStop == 0) name_Process_Step = "Отбор тела";
					else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
				}
				else {
					if (counterStartStop == 0) name_Process_Step = "Picking hearts";
					else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
				}
				// расчет на сколько надо после старт/стопа открыть шаровый кран
				if (bodyPrimaPercentStart >= (uint16_t)bodyPrimaDecline * counterStartStop * 2) {
					bodyPrimaPercent = bodyPrimaPercentStart - (bodyPrimaDecline * counterStartStop * 2);
					if (bodyPrimaPercentStop > bodyPrimaPercent) bodyPrimaPercent = bodyPrimaPercentStop;
				}
				else bodyPrimaPercent = bodyPrimaPercentStop;
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercent);	// Открываем шаровый кран
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
			}
			else {
				if (bodyPrimaPercent == 0) {//if (bodyPrimaPercent <= bodyPrimaPercentStop) {
					setPWM(PWM_CH5, 0, 10);							// Закрыли отбор по пару
					power.heaterStatus = 0;						// выключили ТЭН
					power.heaterPower = 0;						// установили мощность на ТЭН 0 %
					timePauseOff = millis() + 120000;			// время на отключение
					timeAllertInterval = millis() + 10000;		// время для зв. сигнала
					processMode.timeStep = 0;
					stepNext = 0;
					if (RU) name_Process_Step = "Процесс закончен";
					else name_Process_Step = "Process is over";
					settingAlarm = true;
					processMode.step = 7;						// перешли на следующий шаг алгоритма
					numOkStop = 3;
				}
				else {
					if (counterStartStop != 0 && RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
					else if (counterStartStop != 0) name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
					setPWM(PWM_CH5, 0, 10);							// Закрыли отбор по пару
				}
			}
			break;
		}
				// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				setPWM(PWM_CH5, 0, 10);		// закрыть шаровый кран
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				temperatureSensor[DS_Tube].allert = false;
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
			}
			break;
		}
	}
}

// РК по жидкости, универсальный алгоритм 1 клапан на отбор, 1 на переключение емкостей
// PWM_CH1 - клапан переключения емкостей
// PWM_CH2 - клапан отбора
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - Польский буфер
void rfluxLoopMode_4() {
	switch (processMode.step) {
// пришли при старте ректификации
		case 0: {
			counterStartStop = 0;
			processMode.timeStep = 0;
			processMode.step = 1;							// перешли на следующий шаг алгоритма
			break;
		}
// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Стабилизация колонны";
				else name_Process_Step = "Column stabilization";
				processMode.step = 2;						// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// пищалка на 10 сек.
		case 2: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
				processMode.step = 3;						// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
			break;
		}
// ждем окончание стабилизации 20 минут
		case 3: {
#ifdef columnSoftStart
			// плавный подъем температуры
			if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
				stepPowerStart = powerLow / 2;
				stepPowerPercent = powerLow / 20;
				if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
					stepPowerCount++;
					stepPowerTime += (timeStabilizationReflux * 3);
				}
				power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
				if (power.heaterPower > powerLow) power.heaterPower = powerLow;
			}
			else {
				power.heaterPower = powerLow;
			}
#else
			power.heaterPower = powerLow;
#endif
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Отбор голов";
				else name_Process_Step = "Picking heads";
				processMode.step = 4;						// перешли на следующий шаг алгоритма
				stepNext = 0;
				if (typeRefOfValwe == 3) csOn(PWM_CH1);     // с 2-мя клапанами открыли CH1 на отбор голов
			}
			break;
		}
// пищалка на 10 сек.
		case 4: {
			if (timeAllertInterval <= millis() || stepNext == 1) {
				processMode.step = 5;						// перешли на следующий шаг алгоритма отбора голов
				stepNext = 0;
			}
			break;
		}
// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
		case 5: {
			if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
				if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
					timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
					processMode.step = 6;					// перешли на следующий шаг алгоритма
					stepNext = 0;
				}
			}
			else countHaedEnd = millis() + 10000;
			if (processMode.step == 6) {
				if (typeRefOfValwe == 3) csOff(PWM_CH1);	// закрыли клапан отбора голов т.к. 2 клапана на отбор
//				if (pwmOut[3].member == 1) csOff(PWM_CH4);	// закрыли клапан слива ПБ
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				if (RU) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Picking hearts";
			}
			// рулим клапаном отбора голов
			else if (typeRefOfValwe == 1) valveSet(PWM_CH1);
			else valveSet(PWM_CH2);
			break;
		}
// ждем окончание по достижению температуры в кубе и рулим клапаном отбора
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				csOff(PWM_CH1);
				csOff(PWM_CH2);								// закрыли клапана отбора, т.к. процесс закончен
//				if (pwmOut[3].member == 1) {
//					csOn(PWM_CH4);							// включаем клапан слива ПБ
//				}
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				if (stepNext == 1) numOkStop = 0;
				else numOkStop = 1;
				stepNext = 0;
				break;
			}
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
				if (typeRefOfValwe == 1) csOff(PWM_CH1);
				else csOff(PWM_CH2);								// закрыли клапан отбора
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
				if (bodyValveSet) counterStartStop++;
				bodyValveSet = false;
				// если есть польский буфер, работаем до первого стопа
#ifdef oneStop
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//				csOn(PWM_CH4);								// включаем клапан слива ПБ
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
				break;
#endif
			}
			// рулим по уставке
			if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
				temperatureSensor[DS_Tube].allert = false;
				bodyValveSet = true;							// признак, что надо открыть клапан отбора
			}
			if (bodyValveSet == true && processMode.step != 7) {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (adcIn[0].allert == true && alertLevelEnable == true) { // если емкость полная - прекращаем отбор
					if (typeRefOfValwe == 1) csOff(PWM_CH1);
					else csOff(PWM_CH2);
				}
				else {
					if (typeRefOfValwe == 1) valveSet(PWM_CH1);
					else valveSet(PWM_CH2);
				}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bodyTimeOffCount = processMode.timeStep;		// сбрасываем таймер остановки процесса
                                if (RU) {
					if (counterStartStop == 0) name_Process_Step = "Отбор тела";
					else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
				}
                                else {
					if (counterStartStop == 0) name_Process_Step = "Picking hearts";
					else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
				}
			}
			else {
				if (counterStartStop != 0 && RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				else if (counterStartStop != 0) name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
				if (typeRefOfValwe == 1) csOff(PWM_CH1);
				else csOff(PWM_CH2);
			}
			// если задано понижение % отбора, проверим не пора ли вырубить
			if (decline > 0 && timeOn < ((valveCiclePeriod * bodytimeOn) / 1000)) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
			}
			break;
		}
// после завершения процесса ждем N мин. и выключаем клапана и пищалку
		case 7: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
#ifdef valve_4_mode
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
#endif
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				temperatureSensor[DS_Tube].allert = false;
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
			}
			break;
		}
	}
}

// РК по жидкости, универсальный алгоритм 2 клапана на отбор
// PWM_CH1 и PWM_CH2 - клапаны отбора
// PWM_CH3 - клапан подачи воды
// PWM_CH4 - Польский буфер
void rfluxLoopMode_5() {
	switch (processMode.step) {
		// пришли при старте ректификации
	case 0: {
		counterStartStop = 0;
		processMode.timeStep = 0;
		processMode.step = 1;							// перешли на следующий шаг алгоритма
		break;
	}
			// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
	case 1: {
		if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
			if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
			csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
			power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Стабилизация колонны";
			else name_Process_Step = "Column stabilization";
			processMode.step = 2;						// перешли на следующий шаг алгоритма
			stepNext = 0;
		}
		break;
	}
			// пищалка на 10 сек.
	case 2: {
		if (timeAllertInterval <= millis() || stepNext == 1) {
			timePauseOff = timeStabilizationReflux * 60000 + millis(); // время стабилизации колонны
			processMode.step = 3;						// перешли на следующий шаг алгоритма
			stepNext = 0;
		}
		break;
	}
			// ждем окончание стабилизации 20 минут
	case 3: {
#ifdef columnSoftStart
		// плавный подъем температуры
		if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
			stepPowerStart = powerLow / 2;
			stepPowerPercent = powerLow / 20;
			if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
				stepPowerCount++;
				stepPowerTime += (timeStabilizationReflux * 3);
			}
			power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
			if (power.heaterPower > powerLow) power.heaterPower = powerLow;
		}
		else {
			power.heaterPower = powerLow;
		}
#else
		power.heaterPower = powerLow;
#endif
		if (timePauseOff <= millis() || stepNext == 1) {
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Отбор голов";
			else name_Process_Step = "Picking heads";
			processMode.step = 4;						// перешли на следующий шаг алгоритма
			stepNext = 0;
		}
		break;
	}
			// пищалка на 10 сек.
	case 4: {
		if (timeAllertInterval <= millis() || stepNext == 1) {
			processMode.step = 5;						// перешли на следующий шаг алгоритма отбора голов
			stepNext = 0;
		}
		break;
	}
			// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
	case 5: {
		if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
			if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
				processMode.step = 6;					// перешли на следующий шаг алгоритма
				stepNext = 0;
			}
		}
		else countHaedEnd = millis() + 10000;

		if (processMode.step == 6) {
//			if (pwmOut[3].member == 1) csOff(PWM_CH4);	// закрыли клапан слива ПБ
			csOff(PWM_CH1);	// закрыли клапан отбора голов т.к. 2 клапана на отбор
			processMode.timeStep = 0;
			bodyTimeOffCount = 0;
			bodyValveSet = true;
			if (RU) name_Process_Step = "Отбор тела";
			else name_Process_Step = "Picking hearts";
		}
		// рулим клапаном отбора голов
		else if (typeRefOfValwe == 3 || typeRefOfValwe == 1) valveSet(PWM_CH1);
		else if (typeRefOfValwe == 2) valveSet(PWM_CH2);
		break;
	}
			// ждем окончание по достижению температуры в кубе и рулим клапаном отбора
	case 6: {
		if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность на ТЭН 0 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
			temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
			csOff(PWM_CH1);
			csOff(PWM_CH2);								// закрыли клапан отбора
//			if (pwmOut[3].member == 1) {
//				csOn(PWM_CH4);							// включаем клапан слива ПБ
//			}
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Процесс закончен";
			else name_Process_Step = "Process is over";
			settingAlarm = true;
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			if (stepNext == 1) numOkStop = 0;
			else numOkStop = 1;
			stepNext = 0;
			break;
		}
		else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
			temperatureSensor[DS_Tube].allert = true;	// сигнализация для WEB
			csOff(PWM_CH1);
			csOff(PWM_CH2);								// закрыли клапан отбора
			// если первый стоп пищим 10 сек.
			if (counterStartStop == 0) timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
			if (bodyValveSet) counterStartStop++;
			bodyValveSet = false;
			// если есть польский буфер, работаем до первого стопа
#ifdef oneStop
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность на ТЭН 0 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//			csOn(PWM_CH4);								// включаем клапан слива ПБ
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Процесс закончен";
			else name_Process_Step = "Process is over";
			settingAlarm = true;
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			numOkStop = 4;
			break;
#endif
		}
		// рулим по уставке
		if (temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) {
			temperatureSensor[DS_Tube].allert = false;
			bodyValveSet = true;							// признак, что надо открыть клапан отбора
		}
		if (bodyValveSet == true && processMode.step != 7) {
			if (typeRefOfValwe == 1) valveSet(PWM_CH1);
			else if (typeRefOfValwe == 2 || typeRefOfValwe == 3) valveSet(PWM_CH2);
			bodyTimeOffCount = processMode.timeStep;		// сбрасываем таймер остановки процесса
			if (RU) {
				if (counterStartStop == 0) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
			}
			else {
				if (counterStartStop == 0) name_Process_Step = "Picking hearts";
				else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
			}
		}
		else {
			if (counterStartStop != 0 && RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
			else if (counterStartStop != 0) name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
			csOff(PWM_CH1);
			csOff(PWM_CH2);
		}
			// если задано понижение % отбора, проверим не пора ли вырубить
			if (decline > 0 && timeOn < ((valveCiclePeriod * bodytimeOn) / 1000)) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
			}
		break;
	}
			// после завершения процесса ждем N мин. и выключаем клапана и пищалку
	case 7: {
		csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
		power.heaterStatus = 0;						// выключили ТЭН
		power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
		if (stopInfoOutScreen == true) {
			outStopInfo();
			stopInfoOutScreen = false;
		}
		else if (touch_in == true && stopInfoOutScreen == false) {
			processMode.allow = 0;  // вышли из режима дистилляции
			processMode.step = 0;	// обнулили шаг алгоритма
			if (RU) commandWriteSD = "Процесс завершен";
			else commandWriteSD = "Process is over";
			commandSD_en = true;
			stopInfoOutScreen = true;
			touchScreen = 0;
			touchArea = 0;
			touch_in = false;
			initBuzzer(50);
			delay(500);
			attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
		}
#endif
		// ждем 10 сек. до выключения сигнализации
		if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
			csOff(PWM_CH1);				// закрыли клапан отбора
			csOff(PWM_CH2);				// закрыли клапан отбора
#ifdef valve_4_mode
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
#endif
			settingAlarm = false;		// выключили звуковой сигнал
			alertEnable = false;
		}
		// ждем 5 минут. до выключения клапанов
		if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
			csOff(PWM_CH3);		// закрыли клапан подачи воды
			csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
			temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
			temperatureSensor[DS_Tube].allert = false;
			stepNext = 0;
#ifndef TFT_Display
			processMode.allow = 0;  // вышли из режима ректификации
			processMode.step = 0;	// обнулили шаг алгоритма
			if (RU) commandWriteSD = "Процесс завершен";
			else commandWriteSD = "Process is over";
			commandSD_en = true;
#endif
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
// *************************************************************************************************************
//                                                                                                             *
#if defined WColumn_water                                                // Подруливание отбора голов водой         *
//                                                                                                             *
// *************************************************************************************************************
void rfluxLoopMode_6() {
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
		if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
			csOn(PWM_CH3);				// общий на воду
			csOn(PWM_CH2);				// охлаждение на отборе тела
			csOn(PWM_CH1);				// охлаждение на отборе голов
			if (pwmOut[0].member == 1) csOn(PWM_CH1); // клапан отбора голов, если есть
#ifndef Sign_of_Work
			csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
			power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			name_Process_Step = "Стабилизация колонны";
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
#ifdef columnSoftStart
		// плавный подъем температуры
		if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
			stepPowerStart = powerLow / 2;
			stepPowerPercent = powerLow / 20;
			if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
				stepPowerCount++;
				stepPowerTime += (timeStabilizationReflux * 3);
			}
			power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
			if (power.heaterPower > powerLow) power.heaterPower = powerLow;
		}
		else {
			power.heaterPower = powerLow;
		}
#else
		power.heaterPower = powerLow;
#endif
		if (timePauseOff <= millis() || stepNext == 1) {
			timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			name_Process_Step = "Отбор голов";
			csOff(PWM_CH2);			// закрыли охлаждение на отборе тела
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
		if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
			if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
				processMode.step = 6;		// перешли на следующий шаг алгоритма
			}
		}
		else countHaedEnd = millis() + 10000;

		// доп.охлаждение по заданной температуре в отборе или дефе
		if ((temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data >= temperatureSensor[DS_Out].allertValue) ||
			(temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data >= temperatureSensor[DS_Def].allertValue)) {
				csOn(PWM_CH2);
		}
		else csOff(PWM_CH2);

		if (processMode.step == 6) {
//			if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
			csOn(PWM_CH2);				// открыли охлаждение на отборе тела
			csOff(PWM_CH1);				// закрыли охлаждение на отборе голов
			processMode.step = 6;		// перешли на следующий шаг алгоритма (если по stepNext)
			processMode.timeStep = 0;
			bodyTimeOffCount = 0;
			bodyValveSet = true;
			name_Process_Step = "Отбор тела";
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
			processMode.timeStep = 0;
			name_Process_Step = "Процесс закончен";
//			if (pwmOut[3].member == 1) csOn(PWM_CH4);
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			if (stepNext == 1) numOkStop = 0;
			else numOkStop = 1;
			stepNext = 0;
			break;
		}
		else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
			temperatureSensor[DS_Tube].allert = true;
			// если первый стоп пищим 10 сек.
			if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
			if (bodyValveSet) {
				counterStartStop++;
				name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				bodyValveSet = false;
			}
			csOn(PWM_CH1);								// открыли клапан доп. подачи воды т.е. клапан охлаждения на головы
#ifdef oneStop
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность на ТЭН 0 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//			csOn(PWM_CH4);								// включаем клапан слива ПБ
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Процесс закончен";
			else name_Process_Step = "Process is over";
			settingAlarm = true;
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			numOkStop = 4;
			break;
#endif
		}

		if (adcIn[0].allert == true && alertLevelEnable == true) {
			csOn(PWM_CH1);  // если емкость полная - уменьшаем отбор
			//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
		}
		else if ((temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) || temperatureSensor[DS_Tube].allertValue == 0) {
			temperatureSensor[DS_Tube].allert = false;
			csOff(PWM_CH1);	// закрыли клапан доп. подачи воды
			if (counterStartStop == 0) name_Process_Step = "Отбор тела";
			else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
			bodyValveSet = true;
			//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
		}

		if (bodyValveSet == true) bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса

		break;
	}
			// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
	case 7: {
		csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
		power.heaterStatus = 0;						// выключили ТЭН
		power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
		if (stopInfoOutScreen == true) {
			outStopInfo();
			stopInfoOutScreen = false;
		}
		else if (touch_in == true && stopInfoOutScreen == false) {
			processMode.allow = 0;  // вышли из режима дистилляции
			processMode.step = 0;	// обнулили шаг алгоритма
			commandWriteSD = "Процесс завершен";
			commandSD_en = true;
			stopInfoOutScreen = true;
			touchScreen = 0;
			touchArea = 0;
			touch_in = false;
			initBuzzer(50);
			delay(500);
			attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
		}
#endif
		// ждем 10 сек. до выключения сигнализации
		if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
			csOff(PWM_CH1);				// закрыли клапан отбора
			csOff(PWM_CH2);				// закрыли клапан отбора
			settingAlarm = false;		// выключили звуковой сигнал
			alertEnable = false;
		}
		// ждем 5 минут. до выключения клапанов
		if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
			csOff(PWM_CH3);		// закрыли клапан подачи воды
			csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
			temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
			temperatureSensor[DS_Tube].allert = false;
			stepNext = 0;
#ifndef TFT_Display
			processMode.allow = 0;  // вышли из режима ректификации
			processMode.step = 0;	// обнулили шаг алгоритма
			if (RU) commandWriteSD = "Процесс завершен";
			else commandWriteSD = "Process is over";
			commandSD_en = true;
#endif
		}
		break;
	}
	}
}
// *************************************************************************************************************
//                                                                                                             *
#elif defined WColumn_power                                     // Подруливание отбора голов мощностью         *
//                                                                                                             *
// *************************************************************************************************************
void rfluxLoopMode_6() {
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
		if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
			csOn(PWM_CH2);				// охлаждение на отборе голов
			csOn(PWM_CH3);				// охлаждение на отборе тела
			//csOn(PWM_CH3);				// общий на воду
			//csOn(PWM_CH2);				// охлаждение на отборе тела
			//csOn(PWM_CH1);				// охлаждение на отборе голов
			if (pwmOut[0].member == 1) csOn(PWM_CH1); // клапан отбора голов, если есть
#ifndef Sign_of_Work
			csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
			power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			name_Process_Step = "Стабилизация колонны";
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
#ifdef columnSoftStart
		// плавный подъем температуры
		if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
			stepPowerStart = powerLow / 2;
			stepPowerPercent = powerLow / 20;
			if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
				stepPowerCount++;
				stepPowerTime += (timeStabilizationReflux * 3);
			}
			power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
			if (power.heaterPower > powerLow) power.heaterPower = powerLow;
		}
		else {
			power.heaterPower = powerLow;
		}
#else
		power.heaterPower = powerLow;
#endif
		if (timePauseOff <= millis() || stepNext == 1) {
			timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
			processMode.timeStep = 0;
			name_Process_Step = "Отбор голов";
			csOff(PWM_CH3);			// закрыли охлаждение на отборе тела
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
			timeSetWChead = millis() + 60000; // 1 минут не корректировать мощность отбора
			power.heaterPower = powerLow;
			temperatureOld_DS_Out = temperatureSensor[DS_Out].data;
			temperatureOld_DS_Def = temperatureSensor[DS_Def].data;
		}
		break;
	}
			// ждем срабатывание датчика уровня в приемной емкоси голов если он есть, включаем пищалку, поднимаем мощность ТЭН для отбора
	case 5: {
		if ((adcIn[0].member == 1 && adcIn[0].allert == true) || stepNext == 1) {
			if (countHaedEnd <= millis() || stepNext == 1) {	// антирдебезг 10 сек. :)
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала
				processMode.step = 6;		// перешли на следующий шаг алгоритма
			}
		}
		else countHaedEnd = millis() + 10000;

		// подруливаем мощностью по заданной температуре в отборе или дефе
		if (timeSetWChead < millis()) {
			// регулировка по Т отбора
			if (temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data >= temperatureSensor[DS_Out].allertValue) {
				if (power.heaterPower > powerLow - 2) {
					if (!(temperatureSensor[DS_Out].data + 0.125 <= temperatureOld_DS_Out)) {
						power.heaterPower -= 1;
					}
					temperatureOld_DS_Out = temperatureSensor[DS_Out].data;
				}
				timeSetWChead = millis() + 120000; // 2 минута
			}
			else if (temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data <= temperatureSensor[DS_Out].allertValue - 1) {
				if (power.heaterPower < powerLow) {
					if (!(temperatureSensor[DS_Out].data >= temperatureOld_DS_Out + 0.125)) {
						power.heaterPower += 1;
					}
					temperatureOld_DS_Out = temperatureSensor[DS_Out].data;
				}
				timeSetWChead = millis() + 120000; // 2 минута
			}
			// регулировка по Т воды
			if (temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data >= temperatureSensor[DS_Def].allertValue + 2) {
				if (power.heaterPower > powerLow - 3) {
					if (!(temperatureSensor[DS_Def].data + 0.25 <= temperatureOld_DS_Def)) {
						power.heaterPower -= 1;
					}
					temperatureOld_DS_Def = temperatureSensor[DS_Out].data;
				}
				timeSetWChead = millis() + 120000; // 2 минута
			}
			else if (temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data <= temperatureSensor[DS_Def].allertValue - 2) {
				if (power.heaterPower < powerLow) {
					if (!(temperatureSensor[DS_Def].data >= temperatureOld_DS_Def + 0.25)) {
						power.heaterPower += 1;
					}
					temperatureOld_DS_Def = temperatureSensor[DS_Out].data;
				}
				timeSetWChead = millis() + 120000; // 2 минута
			}
		}

		if (processMode.step == 6) {
//			if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
			csOn(PWM_CH3);				// открыли охлаждение на отборе тела
			csOff(PWM_CH2);				// закрыли охлаждение на отборе голов
			//csOff(PWM_CH1);				// закрыли охлаждение на отборе голов
			processMode.step = 6;		// перешли на следующий шаг алгоритма (если по stepNext)
			processMode.timeStep = 0;
			bodyTimeOffCount = 0;
			bodyValveSet = true;
			name_Process_Step = "Отбор тела";
			stepNext = 0;
			power.heaterPower = powerLow;
		}

		break;
	}
			// ждем окончание по достижению температуры в кубе и рулим клапаном доп. охлаждения
	case 6: {
		if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
			power.heaterStatus = 0;							// выключили ТЭН
			power.heaterPower = 0;							// установили мощность на ТЭН 0 %
			temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
			processMode.timeStep = 0;
			name_Process_Step = "Процесс закончен";
//			if (pwmOut[3].member == 1) csOn(PWM_CH4);
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			if (stepNext == 1) numOkStop = 0;
			else numOkStop = 1;
			stepNext = 0;
			break;
		}
		else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
			temperatureSensor[DS_Tube].allert = true;
			// если первый стоп пищим 10 сек.
			if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
			if (bodyValveSet) {
				counterStartStop++;
				name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
				bodyValveSet = false;
			}
			csOn(PWM_CH2);
			//csOn(PWM_CH1);								// открыли клапан доп. подачи воды т.е. клапан охлаждения на головы
#ifdef oneStop
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность на ТЭН 0 %
			timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//			csOn(PWM_CH4);								// включаем клапан слива ПБ
			processMode.timeStep = 0;
			if (RU) name_Process_Step = "Процесс закончен";
			else name_Process_Step = "Process is over";
			settingAlarm = true;
			processMode.step = 7;						// перешли на следующий шаг алгоритма
			numOkStop = 4;
			break;
#endif
		}

		if (adcIn[0].allert == true && alertLevelEnable == true) {
			csOn(PWM_CH2);
			//csOn(PWM_CH1);  // если емкость полная - уменьшаем отбор
		}
		else if ((temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) || temperatureSensor[DS_Tube].allertValue == 0) {
			temperatureSensor[DS_Tube].allert = false;
			csOff(PWM_CH2);
			//csOff(PWM_CH1);	// закрыли клапан доп. подачи воды
			if (counterStartStop == 0) name_Process_Step = "Отбор тела";
			else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
			bodyValveSet = true;
		}

		if (bodyValveSet == true) bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса

		break;
	}
			// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
	case 7: {
		csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
		power.heaterStatus = 0;						// выключили ТЭН
		power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
		if (stopInfoOutScreen == true) {
			outStopInfo();
			stopInfoOutScreen = false;
		}
		else if (touch_in == true && stopInfoOutScreen == false) {
			processMode.allow = 0;  // вышли из режима дистилляции
			processMode.step = 0;	// обнулили шаг алгоритма
			commandWriteSD = "Процесс завершен";
			commandSD_en = true;
			stopInfoOutScreen = true;
			touchScreen = 0;
			touchArea = 0;
			touch_in = false;
			initBuzzer(50);
			delay(500);
			attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
		}
#endif
		// ждем 10 сек. до выключения сигнализации
		if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
			csOff(PWM_CH1);				// закрыли клапан отбора
			csOff(PWM_CH2);				// закрыли клапан отбора
			settingAlarm = false;		// выключили звуковой сигнал
			alertEnable = false;
		}
		// ждем 5 минут. до выключения клапанов
		if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
			csOff(PWM_CH3);		// закрыли клапан подачи воды
			csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
			temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
			temperatureSensor[DS_Tube].allert = false;
			stepNext = 0;
#ifndef TFT_Display
			processMode.allow = 0;  // вышли из режима ректификации
			processMode.step = 0;	// обнулили шаг алгоритма
			if (RU) commandWriteSD = "Процесс завершен";
			else commandWriteSD = "Process is over";
			commandSD_en = true;
#endif
		}
		break;
	}
	}
}
// *************************************************************************************************************
//                                                                                                             *
//                                                                  Стандартный алгоритм                       *
//                                                                                                             *
// *************************************************************************************************************
#else
void rfluxLoopMode_6() {
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
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				csOn(PWM_CH3);
				csOn(PWM_CH2);				// включаем клапан доп. подачи воды
				if (pwmOut[0].member == 1) csOn(PWM_CH1); // клапан отбора голов, если есть
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Стабилизация колонны";
				else name_Process_Step = "Column stabilization";
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
#ifdef columnSoftStart
			// плавный подъем температуры
			if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
				stepPowerStart = powerLow / 2;
				stepPowerPercent = powerLow / 20;
				if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
					stepPowerCount++;
					stepPowerTime += (timeStabilizationReflux * 3);
				}
				power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
				if (power.heaterPower > powerLow) power.heaterPower = powerLow;
			}
			else {
				power.heaterPower = powerLow;
			}
#else
			power.heaterPower = powerLow;
#endif
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Отбор голов";
				else name_Process_Step = "Picking heads";
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
				//if (pwmOut[0].member == 1) csOn(PWM_CH1); // клапан отбора голов, если есть
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
				}
			}
			else countHaedEnd = millis() + 10000;

			if (processMode.step == 6) {
				if (pwmOut[0].member == 1) csOff(PWM_CH1);		// клапан отбора голов, если есть
//				if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				csOff(PWM_CH2);		// закрыли клапан доп. подачи воды
				processMode.step = 6;		// перешли на следующий шаг алгоритма (если по stepNext)
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				if (RU) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Picking hearts";
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
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
//				if (pwmOut[3].member == 1) csOn(PWM_CH4);
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				if (stepNext == 1) numOkStop = 0;
				else numOkStop = 1;
				stepNext = 0;
				break;
			}
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) {
					counterStartStop++;
					if (RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
					else name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
					bodyValveSet = false;
				}
				csOn(PWM_CH2);								// открыли клапан доп. подачи воды
#ifdef oneStop
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//				csOn(PWM_CH4);								// включаем клапан слива ПБ
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
				break;
#endif
			}

			if (adcIn[0].allert == true && alertLevelEnable == true) {
				csOn(PWM_CH2);  // если емкость полная - уменьшаем отбор
				//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
			}
			else if ((temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) || temperatureSensor[DS_Tube].allertValue == 0) {
				temperatureSensor[DS_Tube].allert = false;
				csOff(PWM_CH2);	// закрыли клапан доп. подачи воды
				if (RU) {
					if (counterStartStop == 0) name_Process_Step = "Отбор тела";
					else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
				}
				else {
					if (counterStartStop == 0) name_Process_Step = "Picking hearts";
					else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
				}
				bodyValveSet = true;
				//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
			}

			if (bodyValveSet == true) bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса

			break;
		}
// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				temperatureSensor[DS_Tube].allert = false;
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
			}
			break;
		}
	}
}
#endif

// БК с отбором голов по жидкости до дефа, а тело по пару после дефа
// CH1 - головы
// CH2 - доп. вода
// CH4 - головы/тело                       УБИРАЕМ
void rfluxLoopMode_7() {
	switch (processMode.step) {
		// пришли при старте ректификации
		case 0: {
			// Закрыли отбор по пару
			setPWM(PWM_CH5, 0, 10);
			bodyPrimaPercent = bodyPrimaPercentStart;
			counterStartStop = 0;
			processMode.timeStep = 0;
			stepPowerTime = 0;
			stepPowerCount = 0;
			processMode.step = 1;	// перешли на следующий шаг алгоритма
			break;
		}
			// ждем начала подъема температуры в царге и включаем воду на охлаждение и понижаем мощность на ТЭН
		case 1: {
			if (temperatureSensor[DS_Tube].data >= RefluxTransitionTemperature || stepNext == 1) {
				if (pwmOut[2].member == 1) csOn(PWM_CH3);	// включаем клапан подачи воды
#ifndef Sign_of_Work
				csOff(PWM_CH6);							// выключить дополнительный ТЭН на разгон
#endif
				csOn(PWM_CH2);				// охлаждение на отборе голов
//				csOn(PWM_CH4);				// отбор в емкость голов

				power.heaterPower = powerLow;		// установили мощность на ТЭН 65 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Стабилизация колонны";
				else name_Process_Step = "Column stabilization";
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
#ifdef columnSoftStart
			// плавный подъем температуры
		    if ((timeStabilizationReflux * 40) > processMode.timeStep) { // первую половину времени стакбилизации плавный рост мощности
			    stepPowerStart =  powerLow / 2;
			    stepPowerPercent = powerLow / 20;
			    if (stepPowerTime < processMode.timeStep) { // шаги повышения мощности
			        stepPowerCount++;
			        stepPowerTime += (timeStabilizationReflux * 3);
			    }
			    power.heaterPower = stepPowerStart + (stepPowerPercent * stepPowerCount);
			    if (power.heaterPower > powerLow) power.heaterPower = powerLow;
		    } else {
			    power.heaterPower = powerLow;
		    }
#else
		    power.heaterPower = powerLow;
#endif
			if (timePauseOff <= millis() || stepNext == 1) {
				timeAllertInterval = millis() + 10000;	// установим счетчик времени для зв.сигнала 10 сек.
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Отбор голов";
				else name_Process_Step = "Picking heads";
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
				csOff(PWM_CH1);	// закрыли клапан отбора голов
				csOff(PWM_CH2);				// охлаждение на отборе голов
//				csOff(PWM_CH4);       // закрыли клапан в емкость голов
				//if (pwmOut[3].member == 1) csOff(PWM_CH4);		// закрыли клапан слива ПБ
				// открыть шаровый кран отбора по пару
				bodyPrimaPercentSet = percentCalc(bodyPrimaPercentStart);
				setPWM(PWM_CH5, 0, bodyPrimaPercentSet);
				///////////////////////////////////
				processMode.timeStep = 0;
				bodyTimeOffCount = 0;
				bodyValveSet = true;
				if (RU) name_Process_Step = "Отбор тела";
				else name_Process_Step = "Picking hearts";
			}
			// рулим клапаном отбора голов
			else if (pwmOut[0].member == 1) valveSet(PWM_CH1);
			break;
		}
			// ждем окончание по достижению температуры в кубе и рулим клапаном доп. охлаждения
		case 6: {
			if ((temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) || stepNext == 1) {
				power.heaterStatus = 0;							// выключили ТЭН
				power.heaterPower = 0;							// установили мощность на ТЭН 0 %
				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				//if (pwmOut[3].member == 1) csOn(PWM_CH4);
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				if (stepNext == 1) numOkStop = 0;
				else numOkStop = 1;
				stepNext = 0;
				break;
			}
			else if (temperatureSensor[DS_Tube].allertValue > 0 && temperatureSensor[DS_Tube].data >= temperatureSensor[DS_Tube].allertValue) {
				temperatureSensor[DS_Tube].allert = true;
				// если первый стоп пищим 10 сек.
				if (counterStartStop == 0) timeAllertInterval = millis() + 10000;			// установим счетчик времени для зв.сигнала
				if (bodyValveSet) {
					counterStartStop++;
					if (RU) name_Process_Step = "Отбор тела, " + String(counterStartStop) + "-й стоп";
					else name_Process_Step = "Picking hearts, " + String(counterStartStop) + "st stop";
					bodyValveSet = false;
				}
				csOn(PWM_CH2);								// открыли клапан доп. подачи воды
#ifdef oneStop
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность на ТЭН 0 %
				timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
//				csOn(PWM_CH4);								// включаем клапан слива ПБ
				processMode.timeStep = 0;
				if (RU) name_Process_Step = "Процесс закончен";
				else name_Process_Step = "Process is over";
				settingAlarm = true;
				processMode.step = 7;						// перешли на следующий шаг алгоритма
				numOkStop = 4;
				break;
#endif
			}

			if (adcIn[0].allert == true && alertLevelEnable == true) {
				csOn(PWM_CH2);  // если емкость полная - уменьшаем отбор
				//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
			}
			else if ((temperatureSensor[DS_Tube].data <= temperatureSensor[DS_Tube].allertValue - settingBoilTube) || temperatureSensor[DS_Tube].allertValue == 0) {
				temperatureSensor[DS_Tube].allert = false;
				csOff(PWM_CH2);	// закрыли клапан доп. подачи воды
				if (RU) {
					if (counterStartStop == 0) name_Process_Step = "Отбор тела";
					else name_Process_Step = "Отбор тела, старт/стопов - " + String(counterStartStop);
				}
				else {
					if (counterStartStop == 0) name_Process_Step = "Picking hearts";
					else name_Process_Step = "Picking hearts, start/stop - " + String(counterStartStop);
				}
				bodyValveSet = true;
				//bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса
			}

			if (bodyValveSet == true) bodyTimeOffCount = processMode.timeStep;			// сбрасываем таймер остановки процесса

			break;
		}
			// после завершения процесса ждем 120 сек. и выключаем клапана и пищалку
		case 7: {
			csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
			power.heaterStatus = 0;						// выключили ТЭН
			power.heaterPower = 0;						// установили мощность 0%
#if defined TFT_Display
	// выводим информацию по окончанию процесса
			if (stopInfoOutScreen == true) {
				outStopInfo();
				stopInfoOutScreen = false;
			}
			else if (touch_in == true && stopInfoOutScreen == false) {
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
				stopInfoOutScreen = true;
				touchScreen = 0;
				touchArea = 0;
				touch_in = false;
				initBuzzer(50);
				delay(500);
				attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
			}
#endif
			// ждем 10 сек. до выключения сигнализации
			if (processMode.timeStep >= 10 || adcIn[1].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				setPWM(PWM_CH5, 0, 10);		// закрыть шаровый кран
				settingAlarm = false;		// выключили звуковой сигнал
				alertEnable = false;
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				csOff(PWM_CH4);		// закрыли клапан ПБ или УНО
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
				temperatureSensor[DS_Tube].allert = false;
				stepNext = 0;
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима ректификации
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else commandWriteSD = "Process is over";
				commandSD_en = true;
#endif
			}
			break;
		}
	}
}


// если запущена ректификация
void refluxLoop() {
	if (processMode.step == 0) {
		for (int i = 0; i < 8; i++) temperatureSensor[i].allertValue = 0;
		temperatureSensor[DS_Cube].allert = false;
		temperatureSensor[DS_Tube].allert = false;
		stopInfoOutScreen = true;
		alertEnable = true;
		alertLevelEnable = true;
		startWriteSD = true;
		endWriteSD = false;
		errA = false;
		errT = false;
		temperatureSensor[DS_Tube].allertValue = 0;
		timeAllertInterval = millis();
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
		csOff(PWM_CH1);
		csOff(PWM_CH2);
		csOff(PWM_CH3);
#ifndef valve_4_mode
		if (pwmOut[3].member == 1) csOn(PWM_CH4);	// клапан в буфер открыт
		step_new_body = true;
		step_new_end = true;
#else
		csOff(PWM_CH4);
		step_new_body = true;
#endif
		power.heaterStatus = 1;						// включили нагрев
		csOn(PWM_CH6);								// дополнительный разгон на ТЭНы
		power.heaterPower = power.inPowerHigh;		// установили мощность на ТЭН 100 %
		processMode.timeStep = 0;
		processMode.timeStart = time(nullptr);
		stopInfoOutScreen = true;
		numOkStop = 0;
		stepNext = 0;
		countHaedEnd = 0;
		counterStartStop = 0;
		settingBoilTube = 0;
		beepEnd = false;
		reSetTemperatureStartPressure = true;
		if (processMode.number > 0) {
			if (RU) name_Process_Step = "Нагрев куба";
			else name_Process_Step = "Pot heating";
		}
		else {
			if (pwmOut[0].member == 1) csOn(PWM_CH1);				// открыть клапан отбора голов
			if (pwmOut[1].member == 1) csOn(PWM_CH2);				// открыть клапан отбора тела
			if (pwmOut[2].member == 1) csOn(PWM_CH3);				// включаем клапан подачи воды
			if (RU) name_Process_Step = "Ручной режим, нагрев куба";
			else name_Process_Step = "Manual mode, pot heating";
		}
	}
	else if (stepNext == 1) {
		if (RU) commandWriteSD = "WebSend: Пропустить шаг";
		else commandWriteSD = "WebSend: skip step";
		commandSD_en = true;
	}

	// Мощности ТЭНа (разогрев / работа)
#ifdef reducedPower
	powerLow = (float)power.inPowerLow * 0.9;
#else
	powerLow = power.inPowerLow;
#endif

/*	if (processMode.number != 5) {
		if (processMode.step < 2) power.heaterPower = power.inPowerHigh;
		else if (processMode.step < 7) {
			if (processMode.number > 0 && processMode.step < 6) power.heaterPower = powerLow;
			else if (processMode.number > 0 && processMode.step == 6) power.heaterPower = power.inPowerLow;
		}
	}
*/
#ifndef WColumn_power
	if (processMode.step < 2) power.heaterPower = power.inPowerHigh;
	else if (processMode.step < 7) {
		if (processMode.number > 0 && processMode.step < 6) power.heaterPower = powerLow;
		else if (processMode.number > 0 && processMode.step == 6) power.heaterPower = power.inPowerLow;
	}
#endif

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






		// разрешение или запрет сигнала на датчик уровня
#ifdef LevelOnlyHead
		if (processMode.number > 0 && processMode.step > 5) alertLevelEnable = false;
#else
		if ((processMode.number == 1 && pwmOut[1].member == 0) || (processMode.number == 2 && pwmOut[1].member == 0) ||
			((processMode.number == 3 || processMode.number == 4) && typeRefOfValwe != 3) || ((processMode.number == 5 || processMode.number == 6) && pwmOut[0].member == 0)) alertLevelEnable = true;
		else if (adcIn[0].allert == true && processMode.step == 6 && processMode.timeStep > 20 && processMode.timeStep < 25) {
			alertLevelEnable = false; // не пищим от датчика уровня
			settingAlarm = false;
		}
		else if (processMode.step == 6 && adcIn[0].allert == false && processMode.timeStep > 25) alertLevelEnable = true;
#endif




		// Пищалка для WEB и самой автоматики
		if (timeAllertInterval > millis()) settingAlarm = true;
		//else if (errA == false && errT == false) settingAlarm = false;

		// Проверка датчиков безопасности
		if (processMode.step != 7 && !errA && !errT) check_Err();
		if (processMode.step != 7 && timePauseErrA <= millis()) {
			errA = false; check_Err();
			if (errA) {
				stop_Err();
				if (RU) name_Process_Step = "Стоп по аварии ADC > " + String(adcIn[numCrashStop].name);
				else name_Process_Step = "Emergency stop by ADC > " + String(adcIn[numCrashStop].name);
			}
		}
		if (processMode.step != 7 && timePauseErrT <= millis()) {
			errT = false; check_Err();
			if (errT) {
				stop_Err();
				if (RU) name_Process_Step = "Стоп по аварии T > " + String(temperatureSensor[numCrashStop].name);
				else name_Process_Step = "Emergency stop by T > " + String(temperatureSensor[numCrashStop].name);
			}
		}

		// выключение звука
		if (alertEnable == false || (errA == false && errT == false && (adcIn[0].allert == false || alertLevelEnable == false)
			//&& temperatureSensor[DS_Tube].allert == false 
			&& timeAllertInterval <= millis())) settingAlarm = false;

		nameProcessStep = name_Process_Step;
		// % шарового крана
		if (processMode.number == 1 || processMode.number == 2) {
			if (processMode.step == 6) {
				if (bodyValveSet == true) nameProcessStep += " (\&#216 " + String(bodyPrimaPercent / 2) + "%)";
				else nameProcessStep += " (\&#216 0%)";
			}
		}
		// Вывод веса и скорости отбора
		if (timeScaleResponse < 60 && processMode.step > 3 && processMode.step < 7) {
		  if (RU) {
			  processInfo = "Объем: " + String(scaleWiFiOunces) + " мл. ";
			  processInfo += "Скорость отбора: " + String(scaleWiFiSpeed) + " мл/ч.";
		  }
		  else {
			  processInfo = "Volume: " + String(scaleWiFiOunces) + " ml. ";
			  processInfo += "Sampling rate: " + String(scaleWiFiSpeed) + " ml/h.";
		  }
		}
		else processInfo = "";
	}

	if (processMode.number > 0 && processMode.step == 6) {
		// завершение отбора по времени закрытого состояния клапана на отборе тела
		if (counterStartStop > 0 && processMode.timeStep >= (timeStabilizationReflux * 60 + bodyTimeOffCount)) {
			stop_Err();
			if (RU) nameProcessStep = "Стоп по времени Старт/Стоп";
			else nameProcessStep = "Stop by time start/stop";
			numOkStop = 2;
		}
	}

	/*processInfo = "тест";
	processInfo += "<br>";
	processInfo += "тест";*/
	//processInfo = "Heap: " + String(ESP.getFreeHeap());

	// клапан в 4-ом канале
#ifdef valve_4_mode // УНО
	if (processMode.step == 6 && pwmOut[3].member == 1 && step_new_body == true) {
		csOn(PWM_CH4);
		step_new_body = false;
	}
#else				// ПБ
	if (processMode.step == 6 && pwmOut[3].member == 1 && step_new_body == true) {
		csOff(PWM_CH4);
		step_new_body = false;
	}
	else if (processMode.step == 7 && pwmOut[3].member == 1 && step_new_end == true) {
		csOn(PWM_CH4);
		step_new_end = false;
	}
#endif

	// Уходим на выбранный алгоритм
	switch (processMode.number) {
		case 0: rfluxLoopMode_1(); break; // ручной режим, только сигнализация
		case 1: rfluxLoopMode_2(); break; // Прима - головы по жидкости, тело по пару
		case 2: rfluxLoopMode_3(); break; // РК с отбором по пару
		case 3: rfluxLoopMode_4(); break; // РК с отбором по жидкости 1 клапан на отбор
		case 4: rfluxLoopMode_5(); break; // РК с отбором по жидкости 2 клапана на отбор
		case 5: rfluxLoopMode_6(); break; // БК регулировка охлаждением
		case 6: rfluxLoopMode_7(); break; // БК регулировка охлаждением, отбор голов по жидкости до дефлегматора
	}
}
