
/*
   Система автоматики винокура. 
   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru

*/
#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>   
#include <ESP8266SSDP.h>        
#include <FS.h>                 
#include <ArduinoJson.h>                        
#include <ESP8266HTTPUpdateServer.h>  
#include <OneWire.h>

int heater = 15;            // Выход на реле ТЭНа
int buzzer = 16;             // Выход на пищалку

int flagDallRead = 1; //Переменная опроса датчиков, для определения периодичности опроса в секундах

// Объект для обновления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// Для файловой системы
File fsUploadFile;

// Определяем переменные wifi
String _ssid     = "SSID";     // Для хранения SSID
String _password = "password";    // Для хранения пароля сети
String _ssidAP = "LuckyBox";      // SSID AP точки доступа
String _passwordAP = "12345678";  // пароль точки доступа
String SSDP_Name = "LuckyBox";    // Имя SSDP
String jsonConfig = "{}";
int port = 80;

int timezone = 3;                 // часовой пояс GTM
float temperature =  5;           //Температура 1
float temperature2 = 5;           //Температура 2
float temperature3 = 5;           //Температура 3
float temperature4 = 5;           //Температура 4

bool settingAlarm = false;        // Пересечение границы уставки

void loop() {
  HTTP.handleClient();
  dallRead(flagDallRead*1000);
  distillationLoop();
  refluxLoop();
  brewingLoop();
  heaterLoop();
  delay(1);

}



