// 
// 
// 

#include "pid_config.h"

byte PIDsetSave;

void initPID()
{
	HTTP.on("/pidMashingSet", handlePidSet);				// Настройка коэффициентов Kp, Ki, Kd
}

void EEPROM_float_write_pid(int addr, float val) {
	byte *x = (byte *)&val;
	for (byte i = 0; i < 4; i++) EEPROM.write(i + addr, x[i]);
}
float EEPROM_float_read_pid(int addr) {
	byte x[4];
	for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
	float *y = (float *)&x;
	return y[0];
}

void loadEepromPid() {
	EEPROM.begin(2048);
	uint16_t index = 1600;
	if (EEPROM.read(index) == 4) {
		index++;
		Kp = EEPROM_float_read_pid(index); index += 4;
		Ki = EEPROM_float_read_pid(index); index += 4;
		Kd = EEPROM_float_read_pid(index); index += 4;
		setTempForPID = EEPROM_float_read_pid(index);
	}
	EEPROM.end();
}

void handlePidSet() {
	String arg;
	//arg = "process";
	arg = "save";
	PIDsetSave = HTTP.arg(arg).toInt();
	if (PIDsetSave == 0) {
		arg = "process";
		uint8_t processModeOld = processMode.allow;
		processMode.allow = HTTP.arg(arg + "[allow]").toInt();
		if (processMode.allow != processModeOld) processMode.step = 0;
	}
	arg = "Kp";
	setKp = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "Ki";
	setKi = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "Kd";
	setKd = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "t1";
	setTempForPID = HTTP.arg(arg + "[userSetValue]").toFloat();

	//Kp = setKp;
	//Ki = setKi;
	//Kd = setKd;

	if (PIDsetSave != 0) {
		Kp = setKp;
		Ki = setKi;
		Kd = setKd;
		// сохраним в EEPROM
		EEPROM.begin(2048);
		uint16_t index = 1600;
		EEPROM.write(index, 0x04); index++;
		EEPROM_float_write_pid(index, Kp); index += 4;
		EEPROM_float_write_pid(index, Ki); index += 4;
		EEPROM_float_write_pid(index, Kd); index += 4;
		EEPROM_float_write_pid(index, setTempForPID);
		EEPROM.commit();
		delay(100);
		EEPROM.end();
	}
	//else processMode.step = 0;

	String dataForWeb = "{";
	dataForWeb += "\"process\":{\"allow\":" + String(processMode.allow) + ",\"save\":" + String(PIDsetSave) + "},";
	dataForWeb += "\"settings\":[{\"Kp\":{\"userSetValue\":" + String(setKp) + ",\"deviceOutValue\":" + String(Kp) + "}},";
	dataForWeb += "{\"Ki\":{\"userSetValue\":" + String(setKi) + ",\"deviceOutValue\":" + String(Ki) + "}},";
	dataForWeb += "{\"Kd\":{\"userSetValue\":" + String(setKd) + ",\"deviceOutValue\":" + String(Kd) + "}},";
//	dataForWeb += "\"settings\":[{\"Kp\":{\"userSetValue\":" + String(setKp) + ",\"deviceOutValue\":" + String(setKp) + "}},";
//	dataForWeb += "{\"Ki\":{\"userSetValue\":" + String(setKi) + ",\"deviceOutValue\":" + String(setKi) + "}},";
//	dataForWeb += "{\"Kd\":{\"userSetValue\":" + String(setKd) + ",\"deviceOutValue\":" + String(setKd) + "}},";

	dataForWeb += "{\"t1\":{\"userSetValue\":" + String(setTempForPID) + ",\"deviceOutValue\":" + String(temperatureSensor[numSenseMashBrew].data) + "}}]}";

	HTTP.send(200, "text/json", dataForWeb);

	//Serial.println(PIDsetSave);
}

void pidSetLoop() {
	
	switch (processMode.step) {
		case 0: {
			//Serial.println("Start");
			loadEepromMashing();
			//loadEepromPid();
			processMode.timeStart = time(nullptr);
			if (temperatureSensor[0].member == 1) numSenseMashBrew = 0;
			else if (temperatureSensor[1].member == 1) numSenseMashBrew = 1;
			else if (temperatureSensor[2].member == 1) numSenseMashBrew = 2;
			else if (temperatureSensor[3].member == 1) numSenseMashBrew = 3;
			else if (temperatureSensor[4].member == 1) numSenseMashBrew = 4;
			else if (temperatureSensor[5].member == 1) numSenseMashBrew = 5;
			else if (temperatureSensor[6].member == 1) numSenseMashBrew = 6;
			else if (temperatureSensor[7].member == 1) numSenseMashBrew = 7;
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
			myPID.SetTunings(setKp, setKi, setKd);
			myPID.SetOutputLimits(0, WindowSize);		// временной интервал реагирования для PID
			myPID.SetMode(AUTOMATIC);
			Setpoint = processMashing[0].temperature;
			timePauseOff = millis();
			processMode.step = 1;
			break;
		}
		case 1: {
			if (timePauseOff <= millis()) {
				timePauseOff = millis() + 100;	// счетчик времени
				Input = setTempForPID;

				myPID.Compute();											// расчет времени для PID регулировки

				if (millis() > WindowSize + windowStartTime) {
					windowStartTime += WindowSize;
					if (windowStartTime > millis()) windowStartTime = 0;    // защита от переполнения
				}
				// включить или выключить ТЭН в зависимости от расчетов временного PID регулирования
				if (Output < millis() - windowStartTime) {
					digitalWrite(heater, LOW);
					power.heaterPower = 0;
				}
				else {
					digitalWrite(heater, HIGH);
					power.heaterPower = 100;
				}
			}
			break;
		}
	}
}
