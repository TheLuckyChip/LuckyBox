#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

String curVersion = "2.0RC10";
uint16_t versionForWeb = 2010;
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
uint8_t stepNext = 0;
uint8_t answer = 0;

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
