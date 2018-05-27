#include "touch_interrupt.h"
#include "setup.h"
#include "user_config.h"
#include "wifi_config.h"
#include "time_config.h"
#include "ssdp.h"
#include "reflux_mode.h"
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
	// Настройка вывода для ТЭНа
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
	// Настройка вывода для пищалки
	pinMode(buzzer, OUTPUT);
	digitalWrite(buzzer, LOW);

	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	Serial.println("Start Setup");

	Wire.setClock(400000);
	Wire.begin(pSDA, pSCL);
	delay(10);

	// Установим CS для всех SPI устройств
	pwm = Adafruit_PWMServoDriver();
	pwm.begin();
	pwm.setPWMFreq(1000);
	csOff(13);	// CS SD
	csOff(14);	// CS TFT
	csOff(15);	// CS Touch

#if defined TFT_Display
	csOn(14);
	initTFT();	    // HSPI & TFT
#endif

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
		delay(2000);
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
		delay(2000);
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

	Serial.println("Step 15 - Variables Init");

	dallas_my_sensor[DS_Cube].temperature = 5.0;
	dallas_my_sensor[DS_Tube].temperature = 5.0;
	dallas_my_sensor[DS_Out].temperature = 5.0;
	dallas_my_sensor[DS_Def].temperature = 5.0;
	dallas_my_sensor[DS_Res1].temperature = 5.0;
	dallas_my_sensor[DS_Res2].temperature = 5.0;
	dallas_my_sensor[DS_Res3].temperature = 5.0;
	dallas_my_sensor[DS_Res4].temperature = 5.0;

	int count_w = 10;
	while (1) {
		dallRead();
		if (dallas_my_sensor[DS_Cube].temperature != 5 || count_w == 0) break;
		count_w--;
		delay(200);
	}
	Serial.println("Temperature sensors:");
	if (DS_Count == 0) Serial.println("Not present...");
	if (DS_Count >= 1) Serial.println(dallas_my_sensor[DS_Cube].temperature);
	if (DS_Count >= 2) Serial.println(dallas_my_sensor[DS_Tube].temperature);
	if (DS_Count >= 3) Serial.println(dallas_my_sensor[DS_Out].temperature);
	if (DS_Count >= 4) Serial.println(dallas_my_sensor[DS_Def].temperature);
	if (DS_Count >= 5) Serial.println(dallas_my_sensor[DS_Res1].temperature);
	if (DS_Count >= 6) Serial.println(dallas_my_sensor[DS_Res2].temperature);
	if (DS_Count >= 7) Serial.println(dallas_my_sensor[DS_Res3].temperature);
	if (DS_Count >= 8) Serial.println(dallas_my_sensor[DS_Res4].temperature);
	Serial.println("Pressure sensor:");
	if (pressureStatus) pressureRead();
	Serial.println(pressure);

#if defined TFT_Display
	tftStartForGraph();
#endif

  Serial.println("Setup Done!");

#if defined TFT_Display
  tftStartForGraph();
  tft.setTextColor(ILI9341_GREEN);
  tft.println("");
  tft.println("Done!!!");
  delay(2000);
  tft.fillScreen(ILI9341_BLACK);
  csOff(14);
#endif
}
