#include "heater.h"

int	reg				=	0;		// переменная для расчетов
int	tenRealPower	=	100;	// мощности ТЭН от напряжения сети, может быть от 50% (при напряжении сети 160В) до 135%(при напряжении сети 260В)
int	errorBr			=	0;      // ошибка округления по алгоритму Брезенхема
unsigned long lastTime=0;       //время периода полуволны
uint8_t State = LOW;

void ResOut();

void heaterLoop() {
	if (power.heaterStatus) {
		if (lastTime + 250 <= millis()) {
			if (State == LOW) {
				ResOut();
				State = HIGH;
			} else {
				ResOut();
				State = LOW;
			}
			lastTime = millis();
		}
	}
	else { digitalWrite(heater, 0); }
}

void ResOut() {      // вызываем функцию ResOut()при каждом переходе напряжения через ноль (каждые 10мс)
	   //delay(1);         // задержка которая устанавливает начало открывания семистора ровно при переходе напряжения через ноль 
	reg = power.heaterPowerCorr + errorBr;
	if (reg < 50) {
		outHeater = LOW;
		errorBr = reg;
	} else {
		outHeater = HIGH;
		// ставим флаг измерения амплитудs напряжения сетb через 5мс после переходе через ноль
		errorBr = reg - tenRealPower;
	}
	digitalWrite(heater, outHeater); //пин через который осуществляется дискретное управление реле
}

void initHeater() {
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
}

void comHeaterLoop() {
	// отправим мощность для ТЕНа на внешнее устройство
	if (RX_Pause <= millis()) {// || powerSendOld != power.heaterPower) {
		TX_BUF_IO_Power[5] = power.heaterPower;
		TX_BUF_IO_Power[6] = (uint8_t)(power.heaterPower + 0x6D);
		Serial.write(TX_BUF_IO_Power, 7);

		/*Serial.write(0x41);		// A
		Serial.write(0x54);		// T
		Serial.write(0x2B);		// +
		Serial.write(0x70);		// p
		Serial.write(0x3D);		// =
		Serial.write(power.heaterPower);
		Serial.write((uint8_t)(power.heaterPower + 0x6D)); // Serial.write(buf, len)*/

		powerSendOld = power.heaterPower;
		RX_Pause = millis() + 1000;
	}
}

void wifiHeaterLoop() {
	// отправим мощность для ТЕНа на внешнее устройство
	if ((Tx_WiFi_Pause <= millis() || powerSendOldWiFi != power.heaterPower) && powerWiFiPresent) {
	
		if (client.connected() == 0) {
			client.setTimeout(250);
			client.connect("192.168.1.250", 80);
		}

		TX_BUF_IO_Power[5] = power.heaterPower;
		TX_BUF_IO_Power[6] = (uint8_t)(power.heaterPower + 0x6D);

		String url = "/powerLB?";
		url += "cap1=";
		url += TX_BUF_IO_Power[0];
		url += "&";
		url += "cap2=";
		url += TX_BUF_IO_Power[1];
		url += "&";
		url += "cap3=";
		url += TX_BUF_IO_Power[2];
		url += "&";
		url += "cap4=";
		url += TX_BUF_IO_Power[3];
		url += "&";
		url += "cap5=";
		url += TX_BUF_IO_Power[4];
		url += "&";
		url += "dat=";
		url += TX_BUF_IO_Power[5];
		url += "&";
		url += "crc=";
		url += TX_BUF_IO_Power[6];

		client.print(String("POST ") + url + " HTTP/1.1\r\n" +
			"Host: " + "192.168.1.250" + "\r\n" +
			"Connection: close\r\n\r\n");

		unsigned long Tx_Pause = millis() + 250;
		while (client.available() == 0) {
			if (millis() >= Tx_Pause) {
				client.stop();
				break;
			}
		}

		powerSendOldWiFi = power.heaterPower;
		Tx_WiFi_Pause = millis() + 5000;
	}
}
