#ifndef _user_config_h_
#define _user_config_h_

// ТЭН
#define		heater			15

// buzzer
#define		buzzer			16

// Интерфейс I2C
#define		pSDA			4//5
#define		pSCL			5//4

// Датчики температуры DS18B20
#define		DS_Pin			13
// Количество используемых датчиков температуры DS18B20 (максимум 8)
// Можно не трогать, программа сама определит количество подключенных датчиков
#define		DS_Cnt			8

// Используем дисплей или нет, если нет закоментировать следующую строку
#define		OLED_Display
// Периодичность вывода температурного графика в секундах
#define		OLED_out_temp	10

// Какой датчик давления используем, ненужное закоментировать, нужное наоборот...
#define		Pressure_BMP085
//#define		Pressure_BMP180
//#define		Pressure_BMP280
//#define		Pressure_BME280
// Нужна отладка посредством UART или нет
//#define		Debug_en

#endif
