// 
// 
// 

#include "setting.h"

Ticker tickerSet;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Объект для обновления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// Для файловой системы
File fsUploadFile;

// Определяем переменные wifi
String _ssid       = "SSID";      // Для хранения SSID
String _password   = "PASSWORD";  // Для хранения пароля сети
String _ssidAP     = "LuckyBox";  // SSID AP точки доступа
String _passwordAP = "12345678";  // пароль точки доступа
String SSDP_Name   = "LuckyBox";  // Имя SSDP
String jsonConfig  = "{}";
int port = 80;

int timezone = 3;                 // часовой пояс GTM
int DS_Count;
struct DS_Str dallas_my_sensor[DS_Cnt];
float pressure = 760.0;				// Давление
bool  pressureStatus = 0;			// Если датчик давления отсутствует
unsigned long displayTimeInterval;				// Инетрал времени для вывода графика температуры если показания неизменны
unsigned long sensorTimeRead = 0;				// Интервал чтения датчиков
unsigned long touchTimeRead = millis();			// Интервал опроста тачскрина
int timeWiFiReconnect = 0;
bool settingAlarm = false;          // Пересечение границы уставки
int modeWiFi;
uint8_t DefCubOut = 9;
extern int16_t touch_x = 0;
extern int16_t touch_y = 0;
extern bool touch_in = false;

void csOff(uint8_t ch) {
	pwm.setPWM(ch, 4096, 0); // CS TFT = 1
}
void csOn(uint8_t ch) {
	pwm.setPWM(ch, 0, 4096); // CS TFT = 0
	delay(1);
}