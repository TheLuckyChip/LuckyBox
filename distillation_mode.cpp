#include "distillation_mode.h"
#include "setting.h"
#include "user_config.h"
#include "time_config.h"
#include <ArduinoJson.h>

//ƒистилл¤ци¤

// ќпредел¤ем переменные
bool settingAlarmDistillation = false;     // достижение уставки по температуре куба 
float settingTank = 100;                   // температура отключени¤ нагрева куба при дистилл¤ции браги в спирт-сырец

void initDistillation()
{
	HTTP.on("/SetTempTank", handleSetTempTank);    // остановка уставки дл¤ дистилл¤ции (вкладка Distillation)
	HTTP.on("/distillation.json", handleDistillationJSON); // формирование distillation.json страницы дл¤ передачи данных в web интерфейс
}

//”становка уставки дл¤ дистилл¤ции
void handleSetTempTank()
{
	settingTank = HTTP.arg("SettingTank").toFloat();         // получаем значение уставки из запроса и сохран¤ем в глобальной переменной
	HTTP.send(200, "text/plain", "OK");   // отправл¤ем ответ о выполнении
}

void handleDistillationJSON()
{
	String root = "{}";  // формируем строку дл¤ отправки в файл конфигурации в json формате

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

