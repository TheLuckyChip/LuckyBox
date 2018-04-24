#ifndef _user_config_h_
#define _user_config_h_

// ТЭН
#define		heater			15

// buzzer
#define		buzzer			16

// GPIO для I2C
#define		pSDA			4
#define		pSCL			5

// GPIO для DS18B20
#define		DS_Pin			0
// Количество используемых датчиков температуры DS18B20 (максимум 8)
// Можно не трогать, программа сама определит количество подключенных датчиков
#define		DS_Cnt			8

// Какой используем дисплей
//#define		OLED_Display
#define		TFT_Display

#define		TFT_DC   2
#define		TFT_CS  -1
#define		TFT_RES -1

// Периодичность вывода температурного графика в секундах
#define		Display_out_temp	1

// Периодичность переподключения к потерянной точке доступа (роутеру) в секундах
#define		setRestartWiFi		60

// Какой используем датчик температуры
//#define		Pressure_BMP085
//#define		Pressure_BMP180
//#define		Pressure_BMP280
#define		Pressure_BME280

// Вывод отладочной информации в UART
//#define		Debug_en

#endif