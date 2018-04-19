#include "distillation_mode.h"
#include "setting.h"
#include "user_config.h"
#include "time_config.h"
#include <ArduinoJson.h>

//Дистилляция

// Определяем переменные
bool settingAlarmDistillation = false;     // Достижение уставки по температуре куба 
float settingTank = 100;                   // Температура отключения нагрева куба при дистилляции браги в спирт-сырец

void initDistillation()
{
	HTTP.on("/SetTempTank", handleSetTempTank);    // Установка уставки для дистилляции (вкладка Distillation)
	HTTP.on("/distillation.json", handleDistillationJSON); // формирование distillation.json страницы для передачи данных в web интерфейс
}

//Установка уставки для дистилляции
void handleSetTempTank()
{
	settingTank = HTTP.arg("SettingTank").toFloat();         // Получаем значение уставки из запроса и сохраняем в глобальной переменной
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handleDistillationJSON()
{
	String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате

	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(root);

	json["temperature"] = temperature1;
	json["temperature2"] = temperature2;
	json["temperature3"] = temperature3;
	json["temperature4"] = temperature4;
	json["time"] = GetTime();
	json["settingTank"] = settingTank;
	json["settingAlarmDistillation"] = settingAlarmDistillation;

	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}

void distillationLoop()
{

	if (temperature1 >= settingTank)
	{
		settingAlarmDistillation = true;
		digitalWrite(buzzer, HIGH);
	}
	else
	{
		settingAlarmDistillation = false;
		digitalWrite(buzzer, LOW);
	}
}

