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
		if (data > 50 && data < 65000) { adcIn[0].data = data; adcIn[0].allert = true; }
		else { adcIn[0].data = 0; adcIn[0].allert = false; }
		data = ads.readADC_SingleEnded(AN1);
		data = ads.readADC_SingleEnded(AN1);
		if (data > 50 && data < 65000) { adcIn[1].data = data; adcIn[1].allert = true; }
		else { adcIn[1].data = 0; adcIn[1].allert = false; }
		data = ads.readADC_SingleEnded(AN2);
		data = ads.readADC_SingleEnded(AN2);
		if (data > 50 && data < 65000) { adcIn[2].data = data; adcIn[2].allert = true; }
		else { adcIn[2].data = 0; adcIn[2].allert = false; }
		data = ads.readADC_SingleEnded(AN3);
		data = ads.readADC_SingleEnded(AN3);
		if (data > 50 && data < 65000) { adcIn[3].data = data; adcIn[3].allert = true; }
		else { adcIn[3].data = 0; adcIn[3].allert = false; }
		//Serial.print("AIN0: "); Serial.print(adcIn[0].data); Serial.print(" "); Serial.println(adcIn[0].allert);
		//Serial.print("AIN1: "); Serial.println(adcIn[1].data);
		//Serial.print("AIN2: "); Serial.println(adcIn[2].data);
		//Serial.print("AIN3: "); Serial.println(adcIn[3].data);
		//Serial.println(" ");
	}
}

