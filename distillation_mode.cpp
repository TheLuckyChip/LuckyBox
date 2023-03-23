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
	for (i = 0; i < 4; i++) {
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
	for (i = 0; i < 4; i++) {
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
	for (i = 0; i < 4; i++) {
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
	for (i = 0; i < 4; i++) {
		EEPROM.write(index, pwmOut[i].member);  index++;
	}
	index += 4;
	for (i = 0; i < 4; i++) {
		EEPROM.write(index, adcIn[i].member);  index++;
	}
	EEPROM.write(index, DistillationTransitionTemperature);

	EEPROM.end();

	HTTP.send(200, "text/json", "{\"result\":\"ok\"}");
}

void distillationLoop() {
	if (processMode.step < 2) {
		if (power.heaterPower != power.inPowerHigh) power.heaterPower = power.inPowerHigh;
	}
	else if (processMode.step < 4) {
		if (power.heaterPower != power.inPowerLow) power.heaterPower = power.inPowerLow;
	}
	//else power.heaterPower = 0;

	// Пищалка для WEB и самой автоматики
	if (timeAllertInterval > millis()) settingAlarm = true;
	else if (errA == false && errT == false && adcIn[0].allert == false) settingAlarm = false;

	// Проверка датчиков безопасности
	if (processMode.step != 4 && !errA && !errT) check_Err();
	if (processMode.step != 4 && timePauseErrA <= millis()) {
		errA = false; check_Err();
		if (errA) {
			stop_Err();
			if (RU) nameProcessStep = "Стоп по аварии ADC > " + String(adcIn[numCrashStop].name);
			else nameProcessStep = "Emergency stop by ADC > " + String(adcIn[numCrashStop].name);
		}
	}
	if (processMode.step != 4 && timePauseErrT <= millis()) {
		errT = false; check_Err();
		if (errT) {
			stop_Err();
			if (RU) nameProcessStep = "Стоп по аварии T > " + String(temperatureSensor[numCrashStop].name);
			else nameProcessStep = "Emergency stop by T > " + String(temperatureSensor[numCrashStop].name);
		}
	}

	switch (processMode.step) {
		// пришли при старте дистилляции
		case 0: {
			endWriteSD = false;
			stopInfoOutScreen = true;
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
			power.heaterStatus = 1;							// включили нагрев
			csOn(PWM_CH6);									// включить дополнительный ТЭН на разгон
			if (pwmOut[0].member == 1) csOn(PWM_CH1);		// открыть клапан отбора
			if (pwmOut[1].member == 1) csOn(PWM_CH2);		// открыть клапан отбора
			power.heaterPower = power.inPowerHigh;			// установили мощность на ТЭН
			processMode.timeStep = 0;
			if (RU) nameProcessStep = "Нагрев куба";
			else nameProcessStep = "Pot heating";
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
				if (RU) nameProcessStep = "Отбор СС";
				else nameProcessStep = "Picking raw alcohol";
				processMode.step = 2;	// перешли на следующий шаг алгоритма
			}
			timeStopDistLevelErr = millis(); // время сработавшего датчика уровня
			break;
		}
		case 2: {
			valveOnOff = true;
			processMode.step = 3;	// перешли на следующий шаг алгоритма
			timeStopDistLevelErr = millis(); // время сработавшего датчика уровня
			break;
		}
		case 3: {
			// ждем достижения заданных температур
      if (temperatureSensor[DS_Cube].allertValue > 5) {
  			if (temperatureSensor[DS_Cube].data >= temperatureSensor[DS_Cube].allertValue) { // остановка по t куба
  				power.heaterStatus = 0;						// выключили ТЭН
  				power.heaterPower = 0;						// установили мощность 0%
  				temperatureSensor[DS_Cube].allert = true;	// сигнализация для WEB
  				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала						// подали звуковой сигнал
  				processMode.timeStep = 0;
  				if (RU) nameProcessStep = "Процесс закончен";
				else nameProcessStep = "Process is over";
  				settingAlarm = true;
				numOkStop = 0;
  				processMode.step = 4;						// перешли на следующий шаг алгоритма
  			}
      }
      else if (temperatureSensor[DS_Cube].allertValue > 0 && temperatureSensor[DS_Cube].allertValue <= 5) {
        if (temperatureCubeAlcohol > 0 && temperatureCubeAlcohol <= temperatureSensor[DS_Cube].allertValue) { // остановка по остатку спирта в кубе
          power.heaterStatus = 0;           // выключили ТЭН
          power.heaterPower = 0;            // установили мощность 0%
          temperatureSensor[DS_Cube].allert = true; // сигнализация для WEB
          timeAllertInterval = millis() + 10000;  // счетчик времени для зв.сигнала           // подали звуковой сигнал
          processMode.timeStep = 0;
          if (RU) nameProcessStep = "Процесс закончен";
          else nameProcessStep = "Process is over";
          settingAlarm = true;
	        numOkStop = 1;
          processMode.step = 4;           // перешли на следующий шаг алгоритма
        }
      }



      
      else {
          if (RU) {
            nameProcessStep = "Отбор СС " + String(scaleWiFiOunces) + " мл (" + String(scaleWiFiSpeed) + " мл/ч)";
          }
          else {
            nameProcessStep = "Picking raw alcohol " + String(scaleWiFiOunces) + " ml (" + String(scaleWiFiSpeed) + " ml/h)";
          }
      }






      
			// если выбраны для процесса клапана отбора, закроем их при срабатывании датчика уровня
			if (adcIn[0].member == 1 && adcIn[0].allert == true) {
				valveOnOff = true;
				if (pwmOut[0].member == 1) csOff(PWM_CH1);		// закрыть клапан отбора
				if (pwmOut[1].member == 1) csOff(PWM_CH2);		// закрыть клапан отбора
			}
			else if (valveOnOff == true) {
				valveOnOff = false;
				if (pwmOut[0].member == 1) csOn(PWM_CH1);		// открыть клапан отбора
				if (pwmOut[1].member == 1) csOn(PWM_CH2);		// открыть клапан отбора
			}
			else if (valveOnOff == false) timeStopDistLevelErr = millis();

			if (millis() >= timeStopDistLevelErr + 120000) {
				power.heaterStatus = 0;						// выключили ТЭН
				power.heaterPower = 0;						// установили мощность 0%
				timeAllertInterval = millis() + 10000;	// счетчик времени для зв.сигнала						// подали звуковой сигнал
				processMode.timeStep = 0;
				if (RU) { 
					nameProcessStep = "Процесс закончен, емкость полная";
					commandWriteSD = "Переполнена емкость";
				}
				else {
					nameProcessStep = "Process is over, tank is full";
					commandWriteSD = "Tank is full";
				}
				commandSD_en = true;
				settingAlarm = true;
				processMode.step = 4;						// перешли на следующий шаг алгоритма
				timeStopDistLevelErr = 0; // время сработавшего датчика уровня
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
				else nameProcessStep = "Process is over";
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
			if (processMode.timeStep >= 10 || adcIn[2].allert == true) {
				csOff(PWM_CH1);				// закрыли клапан отбора
				csOff(PWM_CH2);				// закрыли клапан отбора
				settingAlarm = false;		// выключили звуковой сигнал
			}
			// ждем 5 минут. до выключения клапанов
			if (processMode.timeStep >= 300 || adcIn[2].allert == true) {
				csOff(PWM_CH3);		// закрыли клапан подачи воды
				temperatureSensor[DS_Cube].allert = false;	// сигнализация для WEB
#ifndef TFT_Display
				processMode.allow = 0;  // вышли из режима дистилляции
				processMode.step = 0;	// обнулили шаг алгоритма
				if (RU) commandWriteSD = "Процесс завершен";
				else nameProcessStep = "Process is over";
				commandSD_en = true;
#endif
			}

			break;
		}
	}
}
