//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _SETTING_h
#define _SETTING_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "user_config.h"

#include <Ticker.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP8266WebServer.h>
#include <PID_v1.h>

struct TPL_Str
{
	bool		dsMember[8];
	byte		dsPriority[8];
	float		dsAllertValue[8];
	bool		dsDelta[8];
	bool		dsCutoff[8];
	bool		pwmMember[8];
	bool		adcMember[4];
};
struct DS_Str
{
	byte		addrSearch[8];	// серийный номер датчика для поиска
	byte		addr[8];		// серийный номер датчика
	byte		num;			// порядковый номер датчика
	bool		member;			// участвует в выводе или нет
	float		data;			// значение температуры
	float		dataT[4];		// значение температуры для усреднения и исключения глюков опроса
	uint16_t	color;			// цвет для графика
	bool		delta;			// признак что выставляем дельту для уставки
	bool		cutoff;			// признак что выставляем значение температуры для уставки
	float		allertValue;	// значение требуемой температуры (расчитанное)
	float		allertValueIn;	// значение введенное пользователем
	bool		allert;			// сигнализация достижения требуемой температуры
	byte		priority;		// приоритет
	char		name[60];		// название назначенное пользователем
};
struct BMP_Str
{
	float		data;			// значение атмосферного давления
	float		dataStart;		// значение атмосферного давления для коррекции дельты
	bool		status;			// наличие датчика
	uint16_t	color;			// цвет для графика
	bool		member;			// участвует в выводе или нет
};
struct OUT_Pwm
{
	uint16_t	data;			// значение PWM или ON/OFF
	bool		member;			// используется или нет
	bool		allert;			// для индикации
	bool		invert;			// для клапанов - НО или НЗ
	char		name[60];		// название назначенное пользователем
};
struct IN_Adc
{
	uint16_t	data;			// данные АЦП
	bool		member;			// используется или нет
	bool		allert;			// сигнализация аварии по датчику
	char		name[60];		// название назначенное пользователем
};
struct PR_Type
{
	uint8_t			allow;			// какой процесс (дистилляция, ректификация или затирание) включен
	uint8_t			step;			// номер шага алгоритма
	unsigned long	timeStep;		// время шага алгоритма в секундах
	unsigned long	timeStart;		// время старта процесса
	uint8_t			number;			// тип (номер) алгоритма для ректификации или затирания
};
struct PR_Mashing
{
	uint16_t	time;
	float		temperature;
	uint8_t		step;
	bool		stop;
};
struct PR_Power
{
	bool heaterStatus;
	uint8_t	heaterPower;
	uint8_t heaterPowerCorr;
	uint8_t inPowerHigh;
	uint8_t inPowerLow;
};
extern struct TPL_Str tpl2web;
extern struct DS_Str temperatureSensor[DS_Cnt];
extern struct BMP_Str pressureSensor;
extern struct OUT_Pwm pwmOut[PWM_Cnt];
extern struct IN_Adc adcIn[ADC_Cnt];
extern struct PR_Type processMode;
extern struct PR_Mashing processMashing[4];
extern struct PR_Power power;

extern Ticker tickerSet;

extern Adafruit_PWMServoDriver pwm;

// Объект для обновления с web страницы
//extern ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
extern ESP8266WebServer HTTP;

extern PID myPID;

// Для файловой системы
//extern fs::File fsUploadFile;

extern String curVersion;
// Определяем переменные wifi
extern String _ssid;
extern String _password;
extern String _ssidAP;
extern String _ssidAPconnect;
extern String _passwordAP;
extern String SSDP_Name;
extern String addrMacMod;
extern bool touchInvert;
extern bool tftInvert;
extern String jsonConfig;
extern int port;
extern int timezone;
extern byte DS_Count;
extern int temp_min;
extern int temp_max;
extern byte DS_Cube;
extern byte DS_Tube;
extern byte DS_Out;
extern byte DS_Def;
extern byte DS_Res1;
extern byte DS_Res2;
extern byte DS_Res3;
extern byte DS_Res4;
extern unsigned long timePauseOff;
extern unsigned long timeAllertInterval;
extern unsigned long displayTimeInterval;
extern unsigned long adcTimeRead;
extern unsigned long sdTimeWriteInterval;
extern uint16_t graphOutInterval;
extern uint16_t scaleCount;
extern byte tempBigOut;
extern byte tempBigOutOld;
extern float settingBoilTube;
extern float settingColumn;
extern float temperatureStartPressure;
extern float settingColumnShow;
extern float temperatureAlcoholBoil;
extern float temperatureCubeAlcohol;
extern unsigned long sensorTimeRead;
extern unsigned long touchTimeRead;
extern bool settingAlarm;
extern bool headValve;
extern unsigned long headValveOn;
extern unsigned long headValveOff;
extern byte touchArea;
extern byte touchScreen;
//extern uint8_t State;
extern boolean outHeater;
extern uint16_t Voltage;
extern uint16_t servoOld;
extern int modeWiFi;
extern uint8_t DefCubOut;
extern int16_t touch_x;
extern int16_t touch_y;
extern bool touch_in;
extern bool sdStatus;
extern uint8_t numSenseMashBrew;
extern double Setpoint, Input, Output;
extern double Kp, Ki, Kd;
extern double setKp, setKi, setKd;
extern float setTempForPID;
extern int WindowSize;
extern unsigned long windowStartTime, stepTime;
extern unsigned long stepStartTime;
extern unsigned long wifiTimeInterval;
extern String nameProcessStep;
extern bool CH1;
extern bool CH2;
extern bool CH3;
extern bool CH4;

#endif
