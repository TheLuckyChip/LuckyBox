#include "adc.h"

Adafruit_ADS1115 ads;

void adcInit() {
	ads.begin();
	ads.setGain(GAIN_ONE);
}
// чтение всех внешних каналов АЦП
void adcLoop() {
	uint16_t data;
	if (millis() - adcTimeRead >= 1000) {
		adcTimeRead = millis();
		//Serial.println("Start");
		data = ads.readADC_SingleEnded(AN0);
		data = ads.readADC_SingleEnded(AN0);
		if (data > thresholdAN0 && data < 65000) { adcIn[0].data = data; adcIn[0].allert = true; }
		else { adcIn[0].data = 0; adcIn[0].allert = false; }
		data = ads.readADC_SingleEnded(AN1);
		data = ads.readADC_SingleEnded(AN1);
		if (data > thresholdAN1 && data < 65000) { adcIn[1].data = data; adcIn[1].allert = true; }
		else { adcIn[1].data = 0; adcIn[1].allert = false; }
		data = ads.readADC_SingleEnded(AN2);
		data = ads.readADC_SingleEnded(AN2);
		if (data > thresholdAN2 && data < 65000) { adcIn[2].data = data; adcIn[2].allert = true; }
		else { adcIn[2].data = 0; adcIn[2].allert = false; }
		data = ads.readADC_SingleEnded(AN3);
		data = ads.readADC_SingleEnded(AN3);
		if (data > thresholdAN3 && data < 65000) { adcIn[3].data = data; adcIn[3].allert = true; }
		else { adcIn[3].data = 0; adcIn[3].allert = false; }
		
		//Serial.print("AIN0: "); Serial.println(adcIn[0].data);
		//Serial.print("AIN1: "); Serial.println(adcIn[1].data);
		//Serial.print("AIN2: "); Serial.println(adcIn[2].data);
		//Serial.print("AIN3: "); Serial.println(adcIn[3].data);
		//Serial.println(" ");

		// Замер напряжения сети
		uint16_t minVoltage = 1024;
		uint16_t maxVoltage = 0;
		uint16_t adcRez;
		unsigned long timeAdc = millis() + 40;
		while (timeAdc > millis()) {
			adcRez = analogRead(0);
			if (minVoltage > adcRez) minVoltage = adcRez;
			if (maxVoltage < adcRez) maxVoltage = adcRez;
		}
		//Voltage = (maxVoltage - minVoltage) >> 2; // для MCP602
		Voltage = maxVoltage - minVoltage; // для LM358
		if (Voltage < 50) Voltage = 230; // модуль zmpt101 отсутствует
		float deltaPercent = 100 - (Voltage / 2.3);

		// вычислим коррекцию для ТЭНа
		if (power.heaterPower == 100) power.heaterPowerCorr = 100;
		else if (power.heaterPower > 0) {
			deltaPercent /= (100 / power.heaterPower);
			if ((power.heaterPower + deltaPercent) <= 100 && (power.heaterPower + deltaPercent) >= 0) power.heaterPowerCorr = uint8_t(power.heaterPower + deltaPercent);
			else if ((power.heaterPower + deltaPercent) > 100) power.heaterPowerCorr = 100;
			else if ((power.heaterPower + deltaPercent) < 0) power.heaterPowerCorr = 0;
		}
		else power.heaterPowerCorr = 0;
	}
}

