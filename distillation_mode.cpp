#include "distillation_mode.h"
#include "setting.h"
#include "user_config.h"
#include "time_config.h"
#include "buzzer.h"
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

	json["temperature"] = dallas_my_sensor[DS_Cube].temperature;
	json["temperature2"] = dallas_my_sensor[DS_Tube].temperature;
	json["temperature3"] = dallas_my_sensor[DS_Out].temperature;
	json["temperature4"] = dallas_my_sensor[DS_Def].temperature;
	json["time"] = GetTime();
	json["settingTank"] = settingTank;
	json["settingAlarmDistillation"] = settingAlarmDistillation;

	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}

void distillationLoop()
{

	if (dallas_my_sensor[DS_Cube].temperature >= settingTank)
	{
		//if (settingAlarmDistillation == false) initBuzzer();
		settingAlarmDistillation = true;
		//digitalWrite(buzzer, HIGH);
	}
	else
	{
		//if (settingAlarmDistillation == true) deinitBuzzer();
		settingAlarmDistillation = false;
		//digitalWrite(buzzer, LOW);
	}
}

