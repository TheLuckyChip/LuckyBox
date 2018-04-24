#include "touch_interrupt.h"
#include "setup.h"
#include "user_config.h"
#include "wifi_config.h"
#include "timer_config.h"
#include "time_config.h"
#include "ssdp.h"
#include "reflux_mode.h"
#include "pressure.h"
#include "oled.h"
#include "tft.h"
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

	Wire.setClock(100000);
	Wire.begin(pSDA, pSCL);
	delay(100);

#if defined TFT_Display
	initTFT();	    // HSPI & TFT
#elif defined OLED_Display
	initOLED();	// I2C & OLED
#endif

	// инициализация Touch csreen
	pinMode(12, INPUT_PULLUP);
	attachInterrupt(12, touchInt, RISING);


	// инициализация I2C
	Serial.println("Step 1 - I2C Init");
#if defined TFT_Display
	tft.println("Step 1 - I2C Init");
#endif

	// Инициализация датчиков температуры
	Serial.println("Step 2 - DS Init");
#if defined TFT_Display
	tft.println("Step 2 - DS Init");
#endif
	DS_Count = DS_Cnt;
	if (DS_Count > 8) DS_Count = 8;
	dallSearch();
	dallRead();

	//Запускаем файловую систему
	Serial.println("Step 3 - FS Init");
#if defined TFT_Display
	tft.println("Step 3 - FS Init");
#endif
	initFS();
	Serial.println("Step 4 - File Config");
#if defined TFT_Display
	tft.println("Step 4 - File Config");
#endif
	loadConfig();
	Serial.println("Step 5 - WIFI Init");
#if defined TFT_Display
	tft.println("Step 5 - WIFI Init");
	tft.println("");
#endif
	//Запускаем WIFI
	initWifi();

	// просто отчет о подключении
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Not'Connected STA! WiFi up AP.");
		Serial.println("");
		// если есть индикатор выводим локальный IP
#if defined TFT_Display
		tft.setTextSize(2);
		tft.setTextColor(ILI9341_YELLOW);
		tft.println("AP mode. IP adress:");
		tft.setTextSize(1);
		tft.println("");
		tft.setTextSize(2);
		tft.println("192.168.4.1");
		tft.setTextSize(1);
		tft.setTextColor(ILI9341_GREEN);
		delay(5000);
#elif defined OLED_Display
		display.clearDisplay();
		display.setTextColor(WHITE, BLACK);
		display.invertDisplay(0);
		display.setTextSize(1);
		display.setCursor(20, 20);
		display.print("AP IP adress:");
		display.setCursor(32, 40);
		display.print("192.168.4.1");
		display.display();
		delay(5000);
#endif
	}
	else {
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		IPAddress Local_IP = WiFi.localIP();
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(Local_IP);

		// если есть индикатор выводим локальный IP
#if defined TFT_Display
		tft.setTextSize(2);
		tft.setTextColor(ILI9341_YELLOW);
		tft.println("STA mode. IP adress:");
		tft.setTextSize(1);
		tft.println("");
		tft.setTextSize(2);
		tft.println(Local_IP);
		tft.setTextSize(1);
		tft.setTextColor(ILI9341_GREEN);
		delay(5000);
#elif defined OLED_Display
		display.clearDisplay();
		display.setTextColor(WHITE, BLACK);
		display.invertDisplay(0);
		display.setTextSize(1);
		display.setCursor(20, 20);
		display.print("STA IP adress:");
		display.setCursor(32, 40);
		display.print(Local_IP);
		display.display();
		delay(5000);
#endif
	}

	Serial.println("Step 6 - Time, NTP Init");
#if defined TFT_Display
	tft.println("");
	tft.println("Step 6 - Time, Init");
#endif
	// Получаем время из сети
	initTime();
	//Настраиваем и запускаем SSDP интерфейс
	if (WiFi.status() == WL_CONNECTED)
	{
		//Удалось подключиться - запускаем SSDP
		Serial.println("Step 7  - SSDP Init");
#if defined TFT_Display
		tft.println("Step 7 - SSDP Init");
#endif
		initSSDP();
	}

	//Настраиваем и запускаем HTTP интерфейс
	Serial.println("Step 8  - WebServer Start");
#if defined TFT_Display
	tft.println("Step 8 - WebServer Sart");
#endif
	initHTTP();
	Serial.println("Step 9  - Reflux Init");
#if defined TFT_Display
	tft.println("Step 9 - User functions Init");
#endif
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
	while (1) {
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

#if defined TFT_Display
	tftStartForGraph();
#elif defined OLED_Display
	oledStartForGraph();
#endif

  Serial.println("Setup Done!");

  flipper.attach(1.0, myTimer);
}



