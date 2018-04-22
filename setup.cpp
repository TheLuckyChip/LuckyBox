
#include "setup.h"
#include "wifi_config.h"
#include "timer_config.h"
#include "time_config.h"
#include "ssdp.h"
#include "reflux_mode.h"
#include "pressure.h"
#include "oled.h"
#include "http_config.h"
#include "fs_config.h"
#include "file_config.h"
#include "distillation_mode.h"
#include "sensors.h"
#include "setting.h"
#include "buzzer.h"
#include "heater.h"
#include "brewing_mode.h"

void setup()
{

	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	Serial.println("Start Setup");

	Serial.println("Step 1 - DS Init");
	DS_Count = DS_Cnt;
	if (DS_Count > 8) DS_Count = 8;

	// инициализация I2C
	Serial.println("Step 2 - I2C Init");
	Wire.setClock(1000000);
	Wire.begin(pSDA, pSCL);
	delay(100);

	// Инициализация датчиков температуры
	dallSearch();
	dallRead();

	#if defined OLED_Display
	// I2C & OLED
	initOLED();
	#endif

	//Запускаем файловую систему
	Serial.println("Step 3 - FS Init");
	initFS();
	Serial.println("Step 4 - File Config");
	loadConfig();
	Serial.println("Step 5 - WIFI Init");
	//Запускаем WIFI
	initWifi();
	Serial.println("Step 6 - Time, NTP Init");
	// Получаем время из сети
	initTime();
	//Настраиваем и запускаем SSDP интерфейс
	if (WiFi.status() != WL_CONNECTED)
	{
		// Если не удалось подключиться, и мы в режиме AP, то SSDP нам не нужен. Ничего не делаем
	}
	else
	{
		//Удалось подключиться - запускаем SSDP
		Serial.println("Step 7  - SSDP Init");
		initSSDP();
	}

	//Настраиваем и запускаем HTTP интерфейс
	Serial.println("Step 8  - WebServer Start");
	initHTTP();
	Serial.println("Step 9  - Reflux Init");
	initReflux();
	Serial.println("Step 10 - Distillation Init");
	initDistillation();
	Serial.println("Step 11 - Brewing Init");
	initBrewing();
	Serial.println("Step 12 - Heater Init");
	initHeater();
	Serial.println("Step 13 - Pressure sensor Init");
	initPressureSensor();
	Serial.println("Step 14 - Buzzer Init");
	initBuzzer();
	Serial.println("Step 15 - Variables Init");
	pinMode(heater, OUTPUT);
	pinMode(buzzer, OUTPUT);

	int count_w = 10;
	while (1)
	{
		dallRead();
		if (temperature1 != 5 || count_w == 0) break;
		count_w--;
		delay(200);
	}
	Serial.println("Temperature sensors:");
	if (DS_Count >= 1) Serial.println(temperature1);
	if (DS_Count >= 2) Serial.println(temperature2);
	if (DS_Count >= 3) Serial.println(temperature3);
	if (DS_Count >= 4) Serial.println(temperature4);
	if (DS_Count >= 5) Serial.println(temperature5);
	if (DS_Count >= 6) Serial.println(temperature6);
	if (DS_Count >= 7) Serial.println(temperature7);
	if (DS_Count >= 8) Serial.println(temperature8);
	Serial.println("Pressure sensor:");
	if (pressureStatus) pressure = readPressureSensor();
	Serial.println(pressure);

	for (int cnt=0; cnt<120; cnt++) temp_in[cnt] = temperature1 * 10;

	flipper.attach(1.0, myTimer);
	#if defined OLED_Display
	oledStartNormal();
	readTempInterval = 10;
	#endif
	Serial.println("Setup Done!");
}



