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
// Распределение датчиков (нумерация с 0)
#define		DS_Cube			2 // куб
#define		DS_Tube			1 // царга
#define		DS_Out			0 // узел отбора
#define		DS_Def			3 // дефлегматор
#define		DS_Res1			4 // резерв
#define		DS_Res2			5 // резерв
#define		DS_Res3			6 // резерв
#define		DS_Res4			7 // резерв

// Используем дисплей или нет
#define		TFT_Display

#define		TFT_DC   2
#define		TFT_CS  -1
#define		TFT_RES -1

// Периодичность вывода температурного графика в секундах на экран
#define		Display_out_temp	1

// Периодичность переподключения к потерянной точке доступа (роутеру) в секундах
#define		setRestartWiFi		60

// Какой используем датчик температуры
//#define		Pressure_BMP085
#define		Pressure_BMP180
//#define		Pressure_BMP280
//#define		Pressure_BME280

// Вывод отладочной информации в UART
//#define		Debug_en

#endif
