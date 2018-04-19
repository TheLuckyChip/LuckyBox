#include "timer_config.h"
#include "pressure.h"
#include "sensors.h"
#include "oled.h"
#include "setting.h"

void myTimer()
{
	if (pressureStatus) pressure = readPressureSensor(); // чтение датчика давления
	dallRead(); // чтение датчиков температуры
	#if defined Debug_en
	Serial.print("Temperature 1: ");
	Serial.println(temperature1);

	Serial.print("Temperature 2: ");
	Serial.println(temperature2);

	Serial.print("Temperature 3: ");
	Serial.println(temperature3);

	Serial.print("Temperature 4: ");
	Serial.println(temperature4);

	Serial.print("Temperature 5: ");
	Serial.println(temperature5);

	Serial.print("Temperature 6: ");
	Serial.println(temperature6);

	Serial.print("Temperature 7: ");
	Serial.println(temperature7);

	Serial.print("Temperature 8: ");
	Serial.println(temperature8);

	Serial.print("TPressure: ");
	Serial.println(pressure);
	Serial.println("......");
	#endif
	#if defined OLED_Display
	oledOutDisplay(); // вывод на дисплей, если он есть
	#endif
}



