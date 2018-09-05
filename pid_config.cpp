// 
// 
// 

#include "pid_config.h"


float setTempForPID;
byte PIDsetSave;

void initPID()
{
	HTTP.on("/pidMashingSet", handlePidSet);				// Настройка коэффициентов Kp, Ki, Kd
}

void handlePidSet() {
	String arg;
	arg = "process";
	PIDsetSave = HTTP.arg(arg + "[save]").toInt();
	processMode.allow = HTTP.arg(arg + "[allow]").toInt();
	arg = "Kp";
	setKp = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "Ki";
	setKi = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "Kd";
	setKd = HTTP.arg(arg + "[userSetValue]").toFloat();
	arg = "t1";
	setTempForPID = HTTP.arg(arg + "[userSetValue]").toFloat();

	if (PIDsetSave != 0) {
		Kp = setKp;
		Ki = setKi;
		Kd = setKd;
	}

	String dataForWeb = "{";
	dataForWeb += "\"process\":{\"allow\":4,\"save\":0},";
	dataForWeb += "\"settings\":[{\"Kp\":{\"userSetValue\":" + String(setKp) + ",\"deviceOutValue\":" + String(Kp) + "}},";
	dataForWeb += "{\"Ki\":{\"userSetValue\":" + String(setKi) + ",\"deviceOutValue\":" + String(Ki) + "}},";
	dataForWeb += "{\"Kd\":{\"userSetValue\":" + String(setKd) + ",\"deviceOutValue\":" + String(Kd) + "}},";
	//temperatureSensor[DS_Cube].data = 25.6;
	dataForWeb += "{\"t1\":{\"userSetValue\":" + String(setTempForPID) + ",\"deviceOutValue\":" + String(temperatureSensor[numSenseMashBrew].data) + "}}]}";

	HTTP.send(200, "text/json", dataForWeb);
}

void pidSetLoop() {
	switch (processMode.step) {
		case 0: {
			loadEepromMashing();
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
