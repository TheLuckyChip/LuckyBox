#ifndef _DALL_READ_h
#define _DALL_READ_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>
#include "user_config.h"

#if defined Pressure_BMP085 || defined Pressure_BMP180
#include "Adafruit_BMP085.h"
extern Adafruit_BMP085 bmp;
#elif defined Pressure_BMP280 || defined Pressure_BME280
#include <Adafruit_BMP280.h>
extern Adafruit_BMP280 bmp;
#endif

extern OneWire ds;
//typedef struct DS_Str
//{
//	byte  addr[8];
//	float temperature;
//};

extern void initPressureSensor();
extern void pressureRead();
extern void dallSearch();
extern void dallRead();
extern void sensorLoop();

#endif

