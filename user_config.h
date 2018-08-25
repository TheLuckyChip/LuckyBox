#ifndef _user_config_h_
#define _user_config_h_

// ТЭН
#define		heater			16
// GPIO для I2C
#define		pSDA			4
#define		pSCL			5
// GPIO для DS18B20
#define		DS_Pin			0
// Количество используемых датчиков температуры DS18B20 (максимум 8)
// Не трогать, программа сама определит количество подключенных датчиков
#define		DS_Cnt			8
// Количество выходных линий ШИМ
#define		PWM_Cnt			9
// Количество входных линий АЦП
#define		ADC_Cnt			4
// Используем TFT дисплей или нет
#define		TFT_Display
// ТачСкрин
#define		intTouch		2
// GPIO для TFT
#define		TFT_DC_Lib		15
#define		TFT_CS_Lib		-1
#define		TFT_RES_Lib		-1
// Выбор линий CS для TFT, Touch и SD Card; Buzzer & Out_pwm
#define		TFT_RES_PRG	0
#define		SD_CS		7
#define		TFT_CS		15
#define		TOUCH_CS	12
#define		BUZ_VOL		13
#define		BUZ_OUT		14
#define		SIREN_OUT	1
#define		PWM_CH1		9//11
#define		PWM_CH2		8//10
#define		PWM_CH3		11//9
#define		PWM_CH4		10//8
#define		PWM_CH5		2
#define		PWM_CH6		3
#define		PWM_CH7		4
#define		PWM_CH8		5
#define		PWM_CH9		6
// Если клапан нормально закрыты ставим false
// Если клапан нормально открытый ставим true
#define		PWM_CH1_Invert	false
#define		PWM_CH2_Invert	false
#define		PWM_CH3_Invert	false
#define		PWM_CH4_Invert	false
#define		PWM_CH5_Invert	false
#define		PWM_CH6_Invert	false
#define		PWM_CH7_Invert	false
#define		PWM_CH8_Invert	true
#define		PWM_CH9_Invert	true
// переменные временных интервалов на отбор голов
#define		headValveOpen	800
#define		headValveClose	5000
// Периодичность вывода температурного графика в секундах на экран
#define		Display_out_temp	15
// Периодичность записи лог.файла на SD
#define		SD_out_temp	5
// Периодичность переподключения к потерянной точке доступа (роутеру) в секундах
#define		setRestartWiFi		60
// Какой используем датчик давления
//#define		Pressure_BMP085
//#define		Pressure_BMP180
#define		Pressure_BMP280
//#define		Pressure_BME280
// Вывод отладочной информации в UART
//#define		Debug_en

#endif
