#include "file_config.h"
#include "user_config.h"
#include "setting.h"
#include <FS.h>
#include <ArduinoJson.h>

// Загрузка данных сохраненных в файл  config.json
bool loadConfig()
{
	// Открываем файл для чтения
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile)
	{
		// если файл не найден
		Serial.println("Failed to open config file");
		//  Создаем файл записав в него даные по умолчанию
		saveConfig();
		configFile.close();
		return false;
	}
	// Проверяем размер файла, будем использовать файл размером меньше 2048 байта
	size_t size = configFile.size();
	if (size > 2048)
	{
		Serial.println("Config file size is too large");
		configFile.close();
		return false;
	}

	// загружаем файл конфигурации в глобальную переменную
	jsonConfig = configFile.readString();
	configFile.close();
	// Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	//  вызовите парсер JSON через экземпляр jsonBuffer
	//  строку возьмем из глобальной переменной String jsonConfig
	JsonObject& root = jsonBuffer.parseObject(jsonConfig);
	// Теперь можно получить значения из root
	_ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
	_passwordAP = root["ssidAPPassword"].as<String>();
	timezone = root["timezone"];               // Так получаем число
	SSDP_Name = root["SSDPName"].as<String>();
	_ssid = root["ssidName"].as<String>();
	_password = root["ssidPassword"].as<String>();
	return true;
}

// Запись данных в файл config.json
bool saveConfig()
{
	// Резервируем память для json объекта, буфер может расти по мере необходимости, предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	//  вызовите парсер JSON через экземпляр jsonBuffer
	JsonObject& json = jsonBuffer.parseObject(jsonConfig);
	// Заполняем поля json
	json["SSDPName"] = SSDP_Name;
	json["ssidAPName"] = _ssidAP;
	json["ssidAPPassword"] = _passwordAP;
	json["ssidName"] = _ssid;
	json["ssidPassword"] = _password;
	json["timezone"] = timezone;

	// Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
	json.printTo(jsonConfig);
	// Открываем файл для записи
	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile)
	{
		//Serial.println("Failed to open config file for writing");
		configFile.close();
		return false;
	}
	// Записываем строку json в файл
	json.printTo(configFile);
	configFile.close();
	return true;
}



