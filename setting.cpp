#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

String curVersion = "2.0RC11";
uint16_t versionForWeb = 2011;
// Определяем переменные wifi
String _ssid;      // Для хранения SSID
String _password;  // Для хранения пароля сети
String _ssidAP;  // SSID AP точки доступа
String _ssidAPconnect;
String _passwordAP;  // пароль точки доступа
String SSDP_Name;  // Имя SSDP
String addrMacMod;
// Настройки TFT
bool touchInvert = false;
bool tftInvert = false;
String jsonConfig	= "{}";
int port = 80;

int timezone;                 // часовой пояс GTM
byte DS_Count;
int temp_min;
int temp_max;
unsigned long timeSecDsRead;
byte byteDsRead = 0;
byte DS_Cube = 10;
byte DS_Tube = 10;
byte DS_Out = 10;
byte DS_Def = 10;
byte DS_Res1 = 10;
byte DS_Res2 = 10;
byte DS_Res3 = 10;
byte DS_Res4 = 10;
struct TPL_Str tpl2web;
struct DS_Str temperatureSensor[DS_Cnt];
struct BMP_Str pressureSensor;
struct OUT_Pwm pwmOut[PWM_Cnt];
struct IN_Adc adcIn[ADC_Cnt];
struct PR_Type processMode;
struct PR_Mashing processMashing[5];
struct PR_Power power;
									// 0% - 100% с шагом 5%
uint16_t percentCorrectSquare[21] = { 350, 2250, 4250, 5125, 5875, 6500, 6750, 7125, 7500, 7750, 8000, 8250, 8500, 8750, 8875, 9125, 9250, 9375, 9500, 9750, 10000 };

uint8_t StateDsReset;
boolean outHeater;
uint16_t Voltage;
uint16_t servoOld = 100;		// Старая позиция сервопривода
unsigned long displayTimeInterval = 0;
unsigned long sdTimeWriteInterval = 0;
uint16_t graphOutInterval = Display_out_temp;	// Инетрал времени для вывода графика температуры если показания неизменны
uint16_t scaleCount;
byte tempBigOut;
byte tempBigOutOld;
bool reSetTemperatureStartPressure;
float settingBoilTube;
float settingColumn = 101;         // Температура срабатывания оповещения от датчика в царге
float temperatureStartPressure = 78;   //Температура кипения спирта при запуске отслеживания ректификации
float settingColumnShow = 0;
float temperatureAlcoholBoil = 0;
float temperatureCubeAlcohol;
unsigned long timePauseOff = millis();			// интервал времени для применения определенных параметров или ожидания в алгоритмах
unsigned long timeAllertInterval = millis();	// интервал времени для звукового сигнала
uint8_t counterStartStop = 0;					// Счетчик срабатываний Старт/Стоп
uint8_t sensorNumberRead = 0;					// № датчика температуры для текущего опроса
unsigned long adcTimeRead = millis();			// Интервал опроста АЦП
unsigned long timeSec = millis();				// Секундный интервал
bool settingAlarm = false;						// Пересечение границы уставки
bool headValve;									// Состояние клапана отбора
unsigned long headValveOn;						// контроль времени клапана отбора в открытом состоянии
unsigned long headValveOff;						// контроль времени клапана отбора в закрытом состоянии
byte touchArea = 0;								// Область нажатия
byte touchScreen = 0;							// На каком экране контролируем нажатие
byte touchScreenDV = 0;							// Нажатие на клапана для перерисовки
int modeWiFi;
uint8_t DefCubOut = 9;
int16_t touch_x = 0;
int16_t touch_y = 0;
bool touch_in = false;
bool sdStatus = false;
uint8_t numSenseMashBrew;
double Setpoint, Input, Output;
double Kp = 120, Ki = 0.02, Kd = 75;
double setKp, setKi, setKd;
float setTempForPID = 65;
int WindowSize = 250;
unsigned long windowStartTime, stepTime;
unsigned long stepStartTime;
unsigned long wifiTimeInterval;
String nameProcessStep = " ";
String commandWriteSD;
bool startWriteSD = false;
bool commandSD_en = false;
bool CH1 = false;
bool CH2 = false;
bool CH3 = false;
bool CH4 = false;
unsigned long timeSetHighVoltage;
uint8_t timeStabilizationReflux = 20;
uint8_t timeBoilTubeSetReflux = 5;

uint8_t headTimeCycle = 10;
float headtimeOn = 3.5;
uint8_t bodyTimeCycle = 12;
float bodytimeOn = 8.5;
uint8_t decline = 10;
unsigned long bodyTimeOffCount;

uint8_t headSteamPercent = 40;			// % открытия шарового крана на отборе голов по пару
uint8_t bodyPrimaPercentStart = 95;	// % открытия шарового крана в начале отбора тела
uint8_t bodyPrimaPercentStop = 30;		// % открытия шарового крана в конце отбора тела
uint8_t bodyPrimaDecline = 10;			// % уменьшения открытия шарового крана по старт/стопу

uint8_t stepNext = 0;
uint8_t answer = 0;

uint8_t numCrashStop;
bool errA;
bool errT;
unsigned long timePauseErrA;
unsigned long timePauseErrT;

float EEPROM_float_read(int addr) {
	byte x[4];
	for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
	float *y = (float *)&x;
	return y[0];
}
void EEPROM_float_write(int addr, float val) {
	byte *x = (byte *)&val;
	for (byte i = 0; i < 4; i++) EEPROM.write(i + addr, x[i]);
}
void stop_Err() {
	csOff(PWM_CH6);								// выключить дополнительный ТЭН на разгон
	power.heaterStatus = 0;						// выключили ТЭН
	power.heaterPower = 0;						// установили мощность на ТЭН 0 %
	timeAllertInterval = millis() + 10000;		// установим счетчик времени для зв.сигнала
	processMode.timeStep = 0;
	timePauseOff = 60000 * 2 + millis();
	if (processMode.allow == 1) processMode.step = 4;			// дистилляция
	else if (processMode.allow == 2) {							// ректификация
		processMode.step = 7;
		if (processMode.number == 1 || processMode.number == 2) setPWM(PWM_CH5, 0, 10); // Закрыли отбор по пару
	}
}
void check_Err() {
	// датчики безопасности в каналах АЦП
	if (pwmOut[3].member == 0 && adcIn[0].member == 1 && adcIn[0].allert == true) settingAlarm = true;
	else if (adcIn[1].member == 1 && adcIn[1].allert == true) { settingAlarm = true; errA = true; numCrashStop = 1; }
	else if (adcIn[2].member == 1 && adcIn[2].allert == true) settingAlarm = true;
	else if (adcIn[3].member == 1 && adcIn[3].allert == true) settingAlarm = true;
	if (!errA) timePauseErrA = millis() + 10000;		// 10 секунд пауза до защиты
	// датчики безопасности по температурным датчикам кроме Т куба и Т царги
	if (temperatureSensor[DS_Out].cutoff == 1 && temperatureSensor[DS_Out].member == 1 && temperatureSensor[DS_Out].allertValue > 0 && temperatureSensor[DS_Out].data >= temperatureSensor[DS_Out].allertValue) {
		errT = true; numCrashStop = DS_Out;
	}
	else if (temperatureSensor[DS_Def].cutoff == 1 && temperatureSensor[DS_Def].member == 1 && temperatureSensor[DS_Def].allertValue > 0 && temperatureSensor[DS_Def].data >= temperatureSensor[DS_Def].allertValue) {
		errT = true; numCrashStop = DS_Def;
	}
	else if (temperatureSensor[DS_Res1].cutoff == 1 && temperatureSensor[DS_Res1].member == 1 && temperatureSensor[DS_Res1].allertValue > 0 && temperatureSensor[DS_Res1].data >= temperatureSensor[DS_Res1].allertValue) {
		errT = true; numCrashStop = DS_Res1;
	}
	else if (temperatureSensor[DS_Res2].cutoff == 1 && temperatureSensor[DS_Res2].member == 1 && temperatureSensor[DS_Res2].allertValue > 0 && temperatureSensor[DS_Res2].data >= temperatureSensor[DS_Res2].allertValue) {
		errT = true; numCrashStop = DS_Res2;
	}
	else if (temperatureSensor[DS_Res3].cutoff == 1 && temperatureSensor[DS_Res3].member == 1 && temperatureSensor[DS_Res3].allertValue > 0 && temperatureSensor[DS_Res3].data >= temperatureSensor[DS_Res3].allertValue) {
		errT = true; numCrashStop = DS_Res3;
	}
	else if (temperatureSensor[DS_Res4].cutoff == 1 && temperatureSensor[DS_Res4].member == 1 && temperatureSensor[DS_Res4].allertValue > 0 && temperatureSensor[DS_Res4].data >= temperatureSensor[DS_Res4].allertValue) {
		errT = true; numCrashStop = DS_Res4;
	}
	if (!errT) timePauseErrT = millis() + 10000;		// 10 секунд пауза до защиты
}