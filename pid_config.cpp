// 
// 
// 

#include "pid_config.h"

byte PIDsetSave;
unsigned long timePidPause;

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
		////EEPROM.commit();
		////delay(100);
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
	myPID.SetTunings(setKp, setKi, setKd);
	Setpoint = setTempForPID;
}

void pidSetLoop() {
	
	switch (processMode.step) {
		case 0: {
			loadEepromMashing();
			loadEepromPid();
			// если при выборе нет приоритета берем первый из выбранных
			if (temperatureSensor[DS_Cube].member == 1) numSenseMashBrew = DS_Cube;
			else if (temperatureSensor[DS_Tube].member == 1) numSenseMashBrew = DS_Tube;
			else if (temperatureSensor[DS_Out].member == 1) numSenseMashBrew = DS_Out;
			else if (temperatureSensor[DS_Def].member == 1) numSenseMashBrew = DS_Def;
			else if (temperatureSensor[DS_Res1].member == 1) numSenseMashBrew = DS_Res1;
			else if (temperatureSensor[DS_Res2].member == 1) numSenseMashBrew = DS_Res2;
			else if (temperatureSensor[DS_Res3].member == 1) numSenseMashBrew = DS_Res3;
			else if (temperatureSensor[DS_Res4].member == 1) numSenseMashBrew = DS_Res4;
			// если есть приоритет
			if (temperatureSensor[DS_Cube].priority == 1) numSenseMashBrew = DS_Cube;
			else if (temperatureSensor[DS_Tube].priority == 1) numSenseMashBrew = DS_Tube;
			else if (temperatureSensor[DS_Out].priority == 1) numSenseMashBrew = DS_Out;
			else if (temperatureSensor[DS_Def].priority == 1) numSenseMashBrew = DS_Def;
			else if (temperatureSensor[DS_Res1].priority == 1) numSenseMashBrew = DS_Res1;
			else if (temperatureSensor[DS_Res2].priority == 1) numSenseMashBrew = DS_Res2;
			else if (temperatureSensor[DS_Res3].priority == 1) numSenseMashBrew = DS_Res3;
			else if (temperatureSensor[DS_Res4].priority == 1) numSenseMashBrew = DS_Res4;

			processMode.timeStart = time(nullptr);
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
			myPID.SetTunings(setKp, setKi, setKd);
			if (powerType <= 1) myPID.SetOutputLimits(0, WindowSize);		// временной интервал реагирования для PID
			else {
				myPID.SetSampleTime(1000);
				myPID.SetOutputLimits(0, 100);
			}
			myPID.SetMode(AUTOMATIC);
			Setpoint = setTempForPID;
			timePauseOff = millis();
			processMode.step = 1;
			break;
		}
		case 1: {
			Input = temperatureSensor[numSenseMashBrew].data;

			if (powerType <= 1) {
				if (timePidPause < millis()) {
					myPID.Compute();											// расчет времени для PID регулировки
					if (millis() > WindowSize + windowStartTime) {
						windowStartTime += WindowSize;
						if (windowStartTime > millis()) windowStartTime = 0;    // защита от переполнения
					}
					// Если идет предварительный нагрев (до температуры поддержания Т стабилизации минус 4 градуса)
					if (Input < Setpoint - 4) {
						digitalWrite(heater, HIGH);
						power.heaterPower = 100;
					}
					else if (Input > Setpoint + 2) {
						digitalWrite(heater, LOW);
						power.heaterPower = 0;
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
					timePidPause = millis() + 100;
				}
			}
			else {
				myPID.Compute();
				power.heaterPower = (uint8_t)Output;
			}

			break;
		}
	}
}
