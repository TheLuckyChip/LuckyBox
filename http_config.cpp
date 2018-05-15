#include "http_config.h"
#include "setting.h"
#include "file_config.h"
#include "time_config.h"
#include <ArduinoJson.h>

void initHTTP(void)
{

	HTTP.on("/configs.json", handleConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
												 // API для устройства
	HTTP.on("/ssdp", handleSetSSDP);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
	HTTP.on("/ssid", handleSetSSID);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
	HTTP.on("/ssidap", handleSetSSIDAP); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
	HTTP.on("/restart", handleRestart);   // Перезагрузка модуля по запросу вида /restart?device=ok

										   // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
	httpUpdater.setup(&HTTP);
	// Запускаем HTTP сервер
	HTTP.begin();
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handleSetSSDP()
{
	SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
	saveConfig();                 // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}
// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handleSetSSID()
{
	_ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
	_password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
	saveConfig();                        // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handleSetSSIDAP()
{              //
	_ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
	_passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
	saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handleRestart()
{
	String restart = HTTP.arg("device");          // Получаем значение device из запроса
	if (restart == "ok")
	{                         // Если значение равно Ок
		HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
		ESP.restart();                                // перезагружаем модуль
	}
	else
	{                                        // иначе
		HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
	}
}

void handleConfigJSON()
{
	String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате
						 //{"SSDP":"LuckyBox","ssid":"LuckyBox","password":"12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101" и т.д.}
						 // Резервируем память для json объекта буфер может рости по мере необходимости, предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	//  вызовите парсер JSON через экземпляр jsonBuffer
	JsonObject& json = jsonBuffer.parseObject(root);
	// Заполняем поля json
	json["SSDP"] = SSDP_Name;
	json["ssidAP"] = _ssidAP;
	json["passwordAP"] = _passwordAP;
	json["ssid"] = _ssid;
	json["password"] = _password;
	json["timezone"] = timezone;
	json["ip"] = WiFi.localIP().toString();
	json["time"] = GetTime();
	json["date"] = GetDate();
	json["temperature"] = dallas_my_sensor[DS_Cube].temperature;
	json["temperature2"] = dallas_my_sensor[DS_Tube].temperature;
	json["temperature3"] = dallas_my_sensor[DS_Out].temperature;
	json["temperature4"] = dallas_my_sensor[DS_Def].temperature;
	//json["setting"] = settingColumn;
	//json["settingAlarm"] = settingAlarm;


	// Помещаем созданный json в переменную root
	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}


