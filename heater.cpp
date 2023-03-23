#include "heater.h"

#ifdef power_PWM
int	reg				=	1;		// переменная для расчетов
#else
int	reg = 0;		// переменная для расчетов
int	tenRealPower	=	100;	// мощности ТЭН от напряжения сети, может быть от 50% (при напряжении сети 160В) до 135%(при напряжении сети 260В)
int	errorBr			=	0;      // ошибка округления по алгоритму Брезенхема
unsigned long lastTime=0;       //время периода полуволны
uint8_t State = LOW;
void ResOut();
#endif
void heaterLoop() {
	if (power.heaterStatus) {
#ifdef power_PWM
		if (reg <= power.heaterPowerCorr) digitalWrite(heater, 1);
		else digitalWrite(heater, 0);
		if (reg < 100) reg++;
		else reg = 1;
#else
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
#endif
	}
	else { digitalWrite(heater, 0); }
}

#ifndef power_PWM
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
#endif

void initHeater() {
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
#ifdef power_PWM
	tickerSetPower.attach_ms(10, heaterLoop);
#endif
}

void comHeaterLoop() {
	// отправим мощность для ТЕНа на внешнее устройство
#ifndef power_PMBK
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
#else
	pvmk_on_off();
#endif
}
#ifdef power_PMBK
void pvmk_on_off() {
  static uint8_t pow_old = 0;
  /////////////////////////////////////////////////////////////////////////////////////
  // пересчитываем мощность из процентов в напряжение
  // 0% = 30 вольт --- 100% = 230 вольт
  uint8_t pow = power.heaterPower * 2 + 30;
  /////////////////////////////////////////////////////////////////////////////////////
  if (pow_old >= 40 && pow < 40) { // мощность стала меньше допустимой
    uint8_t TX_BUF[8] = {0x41, 0x54, 0x2B, 0x4F, 0x4E, 0x3D, 0x30, 0x0D};
    Serial.write(TX_BUF, 8);
  }
  else if (pow_old < 40 && pow >= 40) { // первое включение мощности
    // запишем мощность
    uint8_t TX_BUF[10] = {0x41, 0x54, 0x2B, 0x56, 0x53, 0x3D, 0x00, 0x00, 0x00, 0x0D};
    String t = String(pow, DEC);
    if (t.length() == 3) { TX_BUF[6] = t[0]; TX_BUF[7] = t[1]; TX_BUF[8] = t[2]; }
    else if (t.length() == 2) { TX_BUF[6] = 0x30; TX_BUF[7] = t[0]; TX_BUF[8] = t[1]; }
    Serial.write(TX_BUF, 10);
    delay(100);
    // "АТ+ON={1 or 0} - команда управляет напряжением на выходе: =0 выключает , =1 включает. Команда
    // выключения блокирует РМВ-К, на дисплее OFF. Блокировка сохраняется даже после отключения и
    // повторного включения РМВ-К.
    // p.s.Команда должна заканчиваться символом перевода строки <CR> = 0x0A
    TX_BUF[3] = 0x4F; TX_BUF[4] = 0x4E; TX_BUF[5] = 0x3D; TX_BUF[6] = 0x31; TX_BUF[7] = 0x0D;
    Serial.write(TX_BUF, 8);
  }
  else if (pow_old != pow) { // изменение мощности в пределах допустимых
    // запишем мощность
    uint8_t TX_BUF[10] = {0x41, 0x54, 0x2B, 0x56, 0x53, 0x3D, 0x00, 0x00, 0x00, 0x0D};
    String t = String(pow, DEC);
    if (t.length() == 3) { TX_BUF[6] = t[0]; TX_BUF[7] = t[1]; TX_BUF[8] = t[2]; }
    else if (t.length() == 2) { TX_BUF[6] = 0x30; TX_BUF[7] = t[0]; TX_BUF[8] = t[1]; }
    Serial.write(TX_BUF, 10);
  }
  pow_old = pow;
}
#endif