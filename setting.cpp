// 
// 
// 

#include "setting.h"

Ticker flipper;

#ifdef ESP8266
// Объект для обновления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

#else
// Объект для обновления с web страницы 
ESP32HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP32WebServer HTTP;

#endif

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
float temperature1 = 5.0;           // Температура 1
float temperature2 = 5.0;           // Температура 2
float temperature3 = 5.0;           // Температура 3
float temperature4 = 5.0;           // Температура 4
float temperature5 = 5.0;           // Температура 5
float temperature6 = 5.0;           // Температура 6
float temperature7 = 5.0;           // Температура 7
float temperature8 = 5.0;           // Температура 8
float pressure = 760.0;				// Давление
bool  pressureStatus = 0;			// Если датчик давления отсутствует
int readTempInterval;				// Инетрал времени для вывода графика температуры если показания неизменны
int timeWiFiReconnect = 0;
bool settingAlarm = false;          // Пересечение границы уставки
int modeWiFi;
