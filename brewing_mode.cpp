//  Затирание

#include "brewing_mode.h"

// Переменные для пауз затирания
float pauseTemp1 = 48;
float pauseTemp2 = 64;
float pauseTemp3 = 72;
float pauseTemp4 = 78;

int pauseTime1 = 15;
int pauseTime2 = 40;
int pauseTime3 = 30;
int pauseTime4 = 5;

unsigned long currentTime=0;     // переменная текущего времени
unsigned long processStartTime=0;// время начала процесса затирания
int startBrewing = 0;     // статус процесса затирания
int stepBrewing = 0;              // Этап затирания

void initBrewing()
{
	HTTP.on("/SettingBrewing", handleSettingBrewing); // Установка уставки для дистилляции (вкладка Distillation)
	HTTP.on("/brewing.json", handleBrewingJSON); // формирование distillation.json страницы для передачи данных в web интерфейс
}

//Получение и установка температур для затирания
void handleSettingBrewing()
{
	pauseTemp1 = HTTP.arg("pauseTemp1").toFloat();       // Получаем значение температуры из запроса и сохраняем в глобальной переменной
	pauseTemp2 = HTTP.arg("pauseTemp2").toFloat();
	pauseTemp3 = HTTP.arg("pauseTemp3").toFloat();
	pauseTemp4 = HTTP.arg("pauseTemp4").toFloat();

	pauseTime1 = HTTP.arg("pauseTime1").toInt();         // Получаем значение времени паузы из запроса и сохраняем в глобальной переменной
	pauseTime2 = HTTP.arg("pauseTime2").toInt();         // Получаем значение времени паузы из запроса и сохраняем в глобальной переменной
	pauseTime3 = HTTP.arg("pauseTime3").toInt();         // Получаем значение времени паузы из запроса и сохраняем в глобальной переменной
	pauseTime4 = HTTP.arg("pauseTime4").toInt();         // Получаем значение времени паузы из запроса и сохраняем в глобальной переменной

	startBrewing = HTTP.arg("startBrewing").toInt();             // Получаем значение старта или стопа из запроса и сохраняем в переменной
	stepBrewing = HTTP.arg("stepBrewing").toInt();

	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handleBrewingJSON()
{
	String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате

	HTTP.send(200, "text/json", root);
}

void brewingLoop()
{
	switch (processMode.step) {
		// пришли при старте затирания
	case 0: {
		// подготовка данных для вывода на TFT
#if defined TFT_Display
		csOn(TFT_CS);
		//tft.fillScreen(ILI9341_BLACK);
		tftStartForGraph();
		displayTimeInterval = millis() + 1000;
		DefCubOut = Display_out_temp;
		csOff(TFT_CS);
#endif
		tempBigOut = 1;
		temperatureSensor[DS_Cube].member = 1;
		myPID.SetOutputLimits(0, WindowSize);
		myPID.SetMode(AUTOMATIC);
		Setpoint = 45.0;
		windowStartTime = millis();
		processMode.step = 1;	// перешли на следующий шаг алгоритма
		break;
	}
	case 1: {
		if (temperatureSensor[DS_Cube].data >= Setpoint) {
			stepTime = millis();
			processMode.step = 2;
		}
		break;
	}
	case 2: {
		if (millis() - stepTime >= 1200000) {
			processMode.step = 3;
			Setpoint = 65.0;
			windowStartTime = millis();
		}
		break;
	}
	case 3: {
		if (temperatureSensor[DS_Cube].data >= Setpoint) {
			stepTime = millis();
			processMode.step = 4;
		}
		break;
	}
	case 4: {
		if (millis() - stepTime >= 1200000) {
			processMode.step = 0;
			processMode.allow = 0;
		}
		break;
	}
	}

	Input = temperatureSensor[DS_Cube].data;
	myPID.Compute();

	if (millis() > WindowSize + windowStartTime) {
		windowStartTime += WindowSize;
		if (windowStartTime > millis()) windowStartTime = 0;    // защита от переполнения
	}
	if (Output < millis() - windowStartTime) digitalWrite(heater, LOW);
	else digitalWrite(heater, HIGH);

	delay(100);

	//Serial.print("Output: ");
	//Serial.print(Output);
	//Serial.print("     Time: ");
	//Serial.println(millis() - windowStartTime);

}