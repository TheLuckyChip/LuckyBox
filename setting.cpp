#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
//ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;
// HTTP клиент
WiFiClient client;

// PID
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//String WiFiPower = "192.168.1.250";

#if defined language_RUS
  String curVersion = "2.0.21";
#else
  String curVersion = "2.0.21 en";
#endif
uint16_t versionForWeb = 2021;
// Определяем переменные wifi
String _ssid;      // Для хранения SSID
String _password;  // Для хранения пароля сети
String _ssidAP;  // SSID AP точки доступа
String _ssidAPconnect;
String _passwordAP;  // пароль точки доступа
String SSDP_Name;  // Имя SSDP
char NBNS_Name[60];
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
struct DS_present_Str temperatureSensorPresent[DS_Cnt];
struct DS_Str temperatureSensor[DS_Cnt];
struct BMP_Str pressureSensor;
struct OUT_Pwm pwmOut[PWM_Cnt];
struct IN_Adc adcIn[ADC_Cnt];
struct PR_Type processMode;
struct PR_Mashing processMashing[5];
struct PR_Power power;
									// 0% - 100% с шагом 5%
uint16_t percentCorrectSquare[22] = { 2000,2107,2299,2545,2833,3154,3505,3881,4278,4695,5128,5576,6037,6509,6991,7480,7976,8478,8983,9491,10000,10000 };

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
float deltaBoilTube;				// уставка
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
unsigned long timeStopDistLevelErr;
String processInfo;
String nameProcessStep = " ";
String name_Process_Step = " ";
String commandWriteSD;
bool startWriteSD = false;
bool endWriteSD = false;
bool commandSD_en = false;
bool CH_all = false;		// для режима удержания
bool CH_up1 = true;		// признак что клапан выключен
bool CH_up2 = true;		// признак что клапан выключен
bool CH_up3 = true;		// признак что клапан выключен
bool CH_up4 = true;		// признак что клапан выключен
bool CH1 = false;			// для отрисовки в настройках
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

bool stopInfoOutScreen = true;
uint8_t numOkStop = 0;
uint8_t numCrashStop;
bool errA;
bool errT;
unsigned long timePauseErrA;
unsigned long timePauseErrT;

bool alertEnable = true;
bool alertLevelEnable = true;
uint16_t BuzzerVolumeLevel;

uint8_t powerType;

uint8_t powerSendOld;
uint8_t TX_BUF_IO_Power[8];
uint8_t RX_BUF_IO[8];
uint8_t RXio_cnt;
unsigned long RX_Pause;
unsigned long Tx_WiFi_Pause;
uint8_t powerSendOldWiFi;
bool powerWiFiPresent = false;

//unsigned long timeScaleGet;
uint16_t scaleWiFiOunces;
uint16_t scaleWiFiSpeed;
//bool scaleWiFiPresent = false;

uint8_t DistillationTransitionTemperature = 80;
uint8_t RefluxTransitionTemperature = 55;
uint8_t TapCorrectionWeb = 120;
float TapCorrection = 1.20;

unsigned long timeSetWChead;
float temperatureOld_DS_Out;
float temperatureOld_DS_Def;

byte touchRead = 1;

bool RU;

uint8_t timeScaleResponse = 60;

