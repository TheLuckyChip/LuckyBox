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
	if (RX_Pause <= millis() || powerSendOld != power.heaterPower) {
		//uint8_t crc_send = power.heaterPower + 0x6D;
		Serial.write(0x41);		// A
		Serial.write(0x54);		// T
		Serial.write(0x2B);		// +
		Serial.write(0x70);		// p
		Serial.write(0x3D);		// =
		Serial.write(power.heaterPower);
		Serial.write((uint8_t)(power.heaterPower + 0x6D));// crc_send);
		powerSendOld = power.heaterPower;
		RX_Pause = millis() + 500;
	}
}
