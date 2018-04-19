#include "brewing_mode.h"
#include "heater.h"
#include "setting.h"
#include <ArduinoJson.h>
#include "time_config.h"

// «атирание

// ѕеременные дл€ пауз затирани€
float pauseTemp1 = 48;
float pauseTemp2 = 64;
float pauseTemp3 = 72;
float pauseTemp4 = 78;

int pauseTime1 = 15;
int pauseTime2 = 40;
int pauseTime3 = 30;
int pauseTime4 = 5;

unsigned long currentTime=0;      // переменна€ текущего времени
unsigned long processStartTime=0; // врем€ начала процесса затирани€
int startBrewing = 0;     // статус процесса затирани€
int stepBrewing = 0;              // Ётап затирани€

void initBrewing()
{
	HTTP.on("/SettingBrewing", handleSettingBrewing);    // ”становка уставки дл€ дистилл€ции (вкладка Distillation)
	HTTP.on("/brewing.json", handleBrewingJSON); // формирование distillation.json страницы дл€ передачи данных в web интерфейс
}

//ѕолучение и установка температур дл€ затирани€
void handleSettingBrewing()
{
	pauseTemp1 = HTTP.arg("pauseTemp1").toFloat();         // ѕолучаем значение температуры из запроса и сохран€ем в глобальной переменной
	pauseTemp2 = HTTP.arg("pauseTemp2").toFloat();
	pauseTemp3 = HTTP.arg("pauseTemp3").toFloat();
	pauseTemp4 = HTTP.arg("pauseTemp4").toFloat();

	pauseTime1 = HTTP.arg("pauseTime1").toInt();         // ѕолучаем значение времени паузы из запроса и сохран€ем в глобальной переменной
	pauseTime2 = HTTP.arg("pauseTime2").toInt();         // ѕолучаем значение времени паузы из запроса и сохран€ем в глобальной переменной
	pauseTime3 = HTTP.arg("pauseTime3").toInt();         // ѕолучаем значение времени паузы из запроса и сохран€ем в глобальной переменной
	pauseTime4 = HTTP.arg("pauseTime4").toInt();         // ѕолучаем значение времени паузы из запроса и сохран€ем в глобальной переменной

	startBrewing = HTTP.arg("startBrewing").toInt();             // ѕолучаем значение старта или стопа из запроса и сохран€ем в переменной
	stepBrewing = HTTP.arg("stepBrewing").toInt();

	HTTP.send(200, "text/plain", "OK");   // отправл€ем ответ о выполнении
}

void handleBrewingJSON()
{
	String root = "{}";  // ‘ормируем строку дл€ отправки в файл конфигурации в json формате

	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(root);

	json["pauseTemp1"] = pauseTemp1;
	json["pauseTemp2"] = pauseTemp2;
	json["pauseTemp3"] = pauseTemp3;
	json["pauseTemp4"] = pauseTemp4;

	json["pauseTime1"] = pauseTime1;
	json["pauseTime2"] = pauseTime2;
	json["pauseTime3"] = pauseTime3;
	json["pauseTime4"] = pauseTime4;

	json["time"] = GetTime();
	json["temperature"] = temperature1;
	json["startBrewing"] = startBrewing;    // ѕередаем значение исключительно дл€ отладки ѕќ
	json["stepBrewing"] = stepBrewing;

	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}

void brewingLoop()
{

	currentTime = millis() / 1000;         // текущее врем€ в сек.

	if (startBrewing)
	{
		switch (stepBrewing)
		{
			case 0:
				heaterStatus=0;
				//stepBrewing=0;       
				break;

			case 1:
				if (temperature1 < pauseTemp1) { heaterStatus = 1; }
				else
				{
					heaterStatus = 0;
					stepBrewing++;                                       // добавить перекраску цвета пауз
					processStartTime = currentTime;
				}
				break;

			case 2:
				if (processStartTime + pauseTime1 * 60 < currentTime) { stepBrewing++; }
				else
				{
					heaterStatus=0;
					if (temperature1 < pauseTemp1) { heaterStatus = 1; }         //добавить гистерезис
				}
				break;

			case 3:
				if (temperature1 < pauseTemp2) { heaterStatus = 1; }
				else
				{
					heaterStatus = 0;
					stepBrewing++;
					processStartTime = currentTime;
				}
				break;

			case 4:
				if (processStartTime + pauseTime2 * 60 < currentTime) { stepBrewing++; }
				else
				{
					heaterStatus = 0;
					if (temperature1 < pauseTemp2) { heaterStatus = 1; }
				}
				break;

			case 5:
				if (temperature1 < pauseTemp3) { heaterStatus = 1; }
				else
				{
					heaterStatus = 0;
					stepBrewing++;
					processStartTime = currentTime;
				}
				break;

			case 6:
				if (processStartTime + pauseTime3 * 60 < currentTime) { stepBrewing++; }
				else
				{
					heaterStatus = 0;
					if (temperature1 < pauseTemp3) { heaterStatus = 1; }
				}
				break;

			case 7:
				if (temperature1 < pauseTemp4) { heaterStatus = 1; }
				else
				{
					heaterStatus = 0;
					stepBrewing++;
					processStartTime = currentTime;
				}
				break;

			case 8:
				if (processStartTime + pauseTime4 * 60 < currentTime)
				{
					stepBrewing = 0;
					heaterStatus = 0;
					startBrewing = 0;
				}
				else
				{
					heaterStatus = 0;
					if (temperature1 < pauseTemp4) { heaterStatus = 1; }
				}
				break;
		}
		// digitalWrite(heater, heaterStatus);

	}

}

