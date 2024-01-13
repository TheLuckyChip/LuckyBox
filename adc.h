//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _ADC_h
#define _ADC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include "setting.h"

#define ADS_I2CADDR     (0x48)
#define ADS1115_REG_CONFIG_MUX_SINGLE_0     (0x4000)
#define ADS1115_REG_CONFIG_MUX_SINGLE_1     (0x5000)
#define ADS1115_REG_CONFIG_MUX_SINGLE_2     (0x6000)
#define ADS1115_REG_CONFIG_MUX_SINGLE_3     (0x7000)
#define ADS1115_REG_POINTER_CONVERT         (0x00)
#define ADS1115_REG_POINTER_CONFIG          (0x01)

static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value);
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg);
extern void adcInit();
extern void adcLoop();

#endif
