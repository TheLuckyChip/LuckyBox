#include "pressure.h"
#include "setting.h"

#if defined Pressure_BMP085 || defined Pressure_BMP180
Adafruit_BMP085 bmp;
#elif defined Pressure_BMP280 || defined Pressure_BME280
Adafruit_BMP280 bmp;
#endif

void initPressureSensor()
{
	#if defined Pressure_BMP085 || defined Pressure_BMP180
	if (!bmp.begin())
	{
		Serial.println("Ooops, no Pressure sensor BMP085 detected ... Check your wiring or I2C Addres!");
	}
	else pressureStatus = 1;
	#elif defined Pressure_BMP280 || defined Pressure_BME280
	if (!bmp.begin(0x76, 0x58))
	{
		Serial.println("Ooops, no Pressure sensor BMP280 detected ... Check your wiring or I2C Addres!");
	}
	else pressure_status = 1;
	#endif
}

float readPressureSensor()
{
	float pressure = bmp.readPressure() / 133.3;
	return pressure;
}
