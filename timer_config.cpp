#include "timer_config.h"
#include "pressure.h"
#include "sensors.h"
#include "oled.h"
#include "tft.h"
#include "wifi_config.h"
#include "setting.h"
#include "user_config.h"

void myTimer()
{
	reconnectWiFi(timeWiFiReconnect);
	if (timeWiFiReconnect < setRestartWiFi) timeWiFiReconnect++;
	else timeWiFiReconnect = 0;

	if (pressureStatus) pressure = readPressureSensor(); // чтение датчика давления
	dallRead(); // чтение датчиков температуры

#if defined TFT_Display
	tftOutGraphDisplay(); // вывод на дисплей, если он есть
#elif defined OLED_Display
	oledOutGraphDisplay(); // вывод на дисплей, если он есть
#endif

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

	Serial.print("Client's AP: ");
	Serial.println(WiFi.softAPgetStationNum());

	Serial.print("Status STA: ");
	Serial.println(WiFi.status());

	Serial.println("......");

#endif
}