#include "http_config.h"
#include "setting.h"
#include "sensors.h"
#include "file_config.h"
#include "time_config.h"
#include <ArduinoJson.h>
#include "heater.h"

void initHTTP(void)
{

	// API для устройства
	
	HTTP.on("/configs.json", HTTP_GET, []() {	// формирование configs.json страницы для передачи данных в web интерфейс
		HTTP.sendHeader("Access-Control-Allow-Origin", "*");	// Доступ с любого домена. (Для отладки удобно)
		HTTP.send(200, "text/plain", getConfigJSON());
	});

	HTTP.on("/GetDto", HTTP_GET, []() {	
		HTTP.sendHeader("Access-Control-Allow-Origin", "*");
		HTTP.send(200, "text/plain", getDto());
	});

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


String getConfigJSON()
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
	json["timezone"] = timezone;
	json["ip"] = WiFi.localIP().toString();
	json["time"] = GetTime();
	json["date"] = GetDate();
	json["temperature"] = temperature1;
	json["temperature2"] = temperature2;
	json["temperature3"] = temperature3;
	json["temperature4"] = temperature4;

	// json["temperatures"] = new float[temperature1, temperature2];	// TODO завести все температуры в массив
	//json["setting"] = settingColumn;
	//json["settingAlarm"] = settingAlarm;


	// Помещаем созданный json в переменную root
	root = "";
	json.printTo(root);

	return root;
}


// Почему на С++ нет рефлексии... (((
String getDto()
{
	String root = "";
	DynamicJsonBuffer jsonBuffer;						// Резервируем память для json объекта буфер может рости по мере необходимости, предпочтительно для ESP8266
	JsonObject& json = jsonBuffer.parseObject("{}");	//  вызовите парсер JSON через экземпляр jsonBuffer

	/*
	Заполняем JSON
	*/

	JsonArray& data = json.createNestedArray("temperatures");
	// Датчики температуры
	for (int i = 0; i < DS_Cnt - 1; i++)
	{
		if (temperatures[i] <= 0)	// Можно сделать проверку на клиенте?
			continue;
		data.add(temperatures[i]);
	}
	
	//json["settingTank"] = settingTank;
	json["heaterPower"] = heaterPower;
	//json["heaterStatus"] = heaterStatus;

	// Помещаем созданный json в переменную root
	json.printTo(root);


	/****************************************\
	******************************************
	******** TODO реализовать остальное ******
	******************************************
	*****************************************/

	return root;	// {"SSDP":"LuckyBox","ssid":"LuckyBox","password":"12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101" и т.д.}
}
