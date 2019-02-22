#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

String curVersion = "2.0RC9";
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
struct PR_Mashing processMashing[4];
struct PR_Power power;

//uint8_t State = LOW;
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
unsigned long sensorTimeRead = millis();		// Интервал чтения датчиков
unsigned long adcTimeRead = millis();			// Интервал опроста АЦП
unsigned long touchTimeRead = millis();			// Интервал опроста тачскрина
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

unsigned long timeSetHeadValveOpen;
unsigned long timeSetHeadValveClose;
unsigned long timeSetBodyValveOpen;
unsigned long timeSetBodyValveClose;