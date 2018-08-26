#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// Определяем переменные wifi
String _ssid		= "SSID";      // Для хранения SSID
String _password	= "PASSWORD";  // Для хранения пароля сети
String _ssidAP		= "LuckyBox";  // SSID AP точки доступа
String _passwordAP	= "12345678";  // пароль точки доступа
String SSDP_Name	= "LuckyBox";  // Имя SSDP
String jsonConfig	= "{}";
int port = 80;

int timezone = 3;                 // часовой пояс GTM
byte DS_Count;
int temp_min;
int temp_max;
byte DS_Cube = 2;
byte DS_Tube = 1;
byte DS_Out = 0;
byte DS_Def = 3;
byte DS_Res1 = 4;
byte DS_Res2 = 5;
byte DS_Res3 = 6;
byte DS_Res4 = 7;
struct DS_Str temperatureSensor[DS_Cnt];
struct BMP_Str pressureSensor;
struct OUT_Pwm pwmOut[PWM_Cnt];
struct IN_Adc adcIn[ADC_Cnt];
struct PR_Type processMode;
struct PR_Mashing processMashing[4];
struct PR_Power power;

//bool heaterStatus = 0;		// статус ТЭНа (включен/выключен)
//int	heaterPower = 0;		// Мощность ТЭНа в % от 0 до 100
uint16_t servoOld = 0;		// Старая позиция сервопривода
unsigned long displayTimeInterval = 0;
unsigned long sdTimeWriteInterval = 0;
uint16_t graphOutInterval = Display_out_temp;	// Инетрал времени для вывода графика температуры если показания неизменны
uint16_t scaleCount;
//float temperatureTubeRect;
float settingBoilTube;
float settingColumn = 101;         // Температура срабатывания оповещения от датчика в царге
float temperatureStartPressure = 78;   //Температура кипения спирта при запуске отслеживания ректификации
float settingColumnShow = 0;
float temperatureAlcoholBoil = 0;
float temperatureCubeAlcohol;
unsigned long timePauseOff;
unsigned long sensorTimeRead = millis();		// Интервал чтения датчиков
unsigned long adcTimeRead = millis();			// Интервал опроста АЦП
unsigned long touchTimeRead = millis();			// Интервал опроста тачскрина
bool settingAlarm = false;						// Пересечение границы уставки
bool headValve;									// Состояние клапана отбора
unsigned long headValveOn;						// контроль времени клапана отбора в открытом состоянии
unsigned long headValveOff;						// контроль времени клапана отбора в закрытом состоянии
byte touchArea = 0;								// Область нажатия
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
int WindowSize = 250;
unsigned long windowStartTime, stepTime;
unsigned long stepStartTime;
unsigned long wifiTimeInterval;
