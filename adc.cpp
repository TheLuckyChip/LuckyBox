#include "adc.h"

void adcInit() {
  Wire.begin();
}
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  Wire.beginTransmission(i2cAddress);
  Wire.write((uint8_t)reg);
  Wire.write((uint8_t)(value>>8));
  Wire.write((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  Wire.beginTransmission(i2cAddress);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom(i2cAddress, (uint8_t)2);
  return ((Wire.read() << 8) | Wire.read());  
}
// чтение всех внешних каналов АЦП
void adcLoop() {
  static uint32_t adsTimeRead = 0;
  static uint8_t chStep = 0;
  uint16_t config_a = 0b1000010111100011;
  switch(chStep) {
    case 0: {
      config_a |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
      writeRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONFIG, config_a);
      adsTimeRead = millis() + 10;
      chStep = 1;
    } break;
    case 1: {
      if (adsTimeRead < millis()) {
        adcIn[0].data = readRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONVERT) / 1.75;
        if (adcIn[0].data > (thresholdAN0 * 114)) adcIn[0].allert = true;
        else adcIn[0].allert = false;
        chStep = 2;
      }
    } break;
    case 2: {
      config_a |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
      writeRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONFIG, config_a);
      adsTimeRead = millis() + 10;
      chStep = 3;
    } break;
    case 3: {
      if (adsTimeRead < millis()) {
        adcIn[1].data = readRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONVERT) / 1.75;
        if (adcIn[1].data > (thresholdAN1 * 114)) adcIn[1].allert = true;
        else adcIn[1].allert = false;
        chStep = 4;
      }
    } break;

    case 4: {
      config_a |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
      writeRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONFIG, config_a);
      adsTimeRead = millis() + 10;
      chStep = 5;
    } break;
    case 5: {
      if (adsTimeRead < millis()) {
        adcIn[2].data = readRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONVERT) / 1.75;
        if (adcIn[2].data > (thresholdAN2 * 114)) adcIn[2].allert = true;
        else adcIn[2].allert = false;
        chStep = 6;
      }
    } break;
    case 6: {
      config_a |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
      writeRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONFIG, config_a);
      adsTimeRead = millis() + 10;
      chStep = 7;
    } break;
    case 7: {
      if (adsTimeRead < millis()) {
        adcIn[3].data = readRegister(ADS_I2CADDR, ADS1115_REG_POINTER_CONVERT) / 1.75;
        if (adcIn[3].data > (thresholdAN3 * 114)) adcIn[3].allert = true;
        else adcIn[3].allert = false;
        chStep = 0;
      }
    } break;
  }


  
	if (millis() - adcTimeRead >= 1000) {
		adcTimeRead = millis();
		
//		Serial.print("AIN0: "); Serial.println(adcIn[0].data);
//		Serial.print("AIN1: "); Serial.println(adcIn[1].data);
//		Serial.print("AIN2: "); Serial.println(adcIn[2].data);
//		Serial.print("AIN3: "); Serial.println(adcIn[3].data);
//		Serial.println(" ");

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

