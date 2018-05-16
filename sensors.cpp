#include "sensors.h"
#include "setting.h"
#include "buzzer.h"
#include "tft.h"
#include "wifi_config.h"

#if defined Pressure_BMP085 || defined Pressure_BMP180
Adafruit_BMP085 bmp;
#elif defined Pressure_BMP280 || defined Pressure_BME280
Adafruit_BMP280 bmp;
#endif

OneWire ds(DS_Pin);

//struct DS_Str dallas_my_sensor[DS_Cnt];

void initPressureSensor()
{
	pressureStatus = 0;
#if defined Pressure_BMP085 || defined Pressure_BMP180
	if (!bmp.begin(BMP085_HIGHRES))
	{
		Serial.println("Ooops, no Pressure sensor BMP085 detected ... Check your wiring or I2C Addres!");
	}
	else pressureStatus = 1;
#elif defined Pressure_BMP280 || defined Pressure_BME280
	if (!bmp.begin(0x76, 0x58))
	{
		Serial.println("Ooops, no Pressure sensor BMP280 detected ... Check your wiring or I2C Addres!");
	}
	else pressureStatus = 1;
#endif
}

void pressureRead()
{
	pressure = bmp.readPressure() / 133.3;
}

void dallSearch()
{
	int i;
	// поиск датчиков, определение их количества и сохранение их адресов
	for (i = 0; i < DS_Cnt; i++) {
		if (!ds.search(dallas_my_sensor[i].addr)) break;
		if (ds.crc8(dallas_my_sensor[i].addr, 7) != dallas_my_sensor[i].addr[7]) break;
	}
	DS_Count = i;
	// установим разрядность, и пропустим симофоры
	for (i = 0; i < DS_Count; i++) {
		ds.reset(); // запрос шины 1-Wire
		ds.select(dallas_my_sensor[i - 1].addr); // выбор нашего DS18B20
		ds.write(0x4E); // запись в  scratchPad
		ds.write(0x00); // User byte 0 - Unused
		ds.write(0x00); // User byte 1 - Unused
		ds.write(0x7F); // выставляем 12 бит (0x7F)
	}
	ds.reset(); // сбрасываем 1-Wire

#if defined TFT_Display
	tft.setTextSize(1);
	tft.println("");
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("DS18B20 found: ");
	tft.println(DS_Count);
	tft.setTextSize(1);
	tft.println("");
	tft.setTextColor(ILI9341_GREEN);
#endif

#if defined Debug_en
	Serial.print("Temperature sensor found: ");
	Serial.println(DS_Count);
	Serial.println("......");
#endif
}

void dallRead()
{
	  int i = DS_Count;
	  while (i) {
		  ds.reset();
		  ds.select(dallas_my_sensor[i-1].addr);
		  ds.write(0xBE); //Считывание значения с датчика
		  dallas_my_sensor[i-1].temperature = (short)(ds.read() | ds.read() << 8); //Принимаем два байта температуры
		  dallas_my_sensor[i-1].temperature *= 0.0625;
		  dallas_my_sensor[i-1].temperature *= 100;
		  dallas_my_sensor[i-1].temperature = floor(dallas_my_sensor[i-1].temperature + 0.5);
		  dallas_my_sensor[i-1].temperature /= 100;
		  i--;
	  }

	 ds.reset();
	 ds.write(0xCC); //Обращение ко всем датчикам
	 ds.write(0x44); //Команда на конвертацию
}

void sensorLoop() {
	if ((millis() - sensorTimeRead)	>= 1000) {
		sensorTimeRead = millis();

		// опрос датчиков
		pressureRead();
		dallRead();

		// Пищалка для WEB
		if (settingAlarm == true) {
			initBuzzer(80);
			pwm.setPWM(0, 0, 4096); // =0 выключить клапан отбора
		}
		else pwm.setPWM(0, 4096, 0); // =1 включить клапан отбора

		// Отладочная информация
#if defined Debug_en
		Serial.print("Temperature 1: ");
		Serial.println(dallas_my_sensor[DS_Cube].temperature);

		Serial.print("Temperature 2: ");
		Serial.println(dallas_my_sensor[DS_Tube].temperature);

		Serial.print("Temperature 3: ");
		Serial.println(dallas_my_sensor[DS_Out].temperature);

		Serial.print("Temperature 4: ");
		Serial.println(dallas_my_sensor[DS_Def].temperature);

		Serial.print("Temperature 5: ");
		Serial.println(dallas_my_sensor[DS_Res1].temperature);

		Serial.print("Temperature 6: ");
		Serial.println(dallas_my_sensor[DS_Res2].temperature);

		Serial.print("Temperature 7: ");
		Serial.println(dallas_my_sensor[DS_Res3].temperature);

		Serial.print("Temperature 8: ");
		Serial.println(dallas_my_sensor[DS_Res4].temperature);

		Serial.print("TPressure: ");
		Serial.println(pressure);

		Serial.print("Client's AP: ");
		Serial.println(WiFi.softAPgetStationNum());

		Serial.print("Status STA: ");
		Serial.println(WiFi.status());

		Serial.println("......");

#endif
	}
}