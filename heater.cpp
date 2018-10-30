#include "heater.h"

int	reg				=	0;		// переменная для расчетов
int	tenRealPower	=	100;	// мощности ТЭН от напряжения сети, может быть от 50% (при напряжении сети 160В) до 135%(при напряжении сети 260В)
int	errorBr			=	0;      // ошибка округления по алгоритму Брезенхема
boolean outHeater;
unsigned long lastTime=0;       //время периода полуволны
int State = LOW;

void ResOut();

void heaterLoop()
{
	if (power.heaterStatus)
	{
		if (lastTime + 250 <= millis())
		{
			if (State == LOW)
			{
				ResOut();
				State = HIGH;
			}
			else
			{
				ResOut();
				State = LOW;
			}
			lastTime = millis();
		}
	}
	else { digitalWrite(heater, 0); }
}

void ResOut()
{      // вызываем функцию ResOut()при каждом переходе напряжения через ноль (каждые 10мс)
	   //delay(1);         // задержка которая устанавливает начало открывания семистора ровно при переходе напряжения через ноль 
	reg = power.heaterPowerCorr + errorBr;
	if (reg < 50)
	{
		outHeater=0;
		errorBr = reg;
	}
	else
	{
		outHeater=1;
		// ставим флаг измерения амплитудs напряжения сетb через 5мс после переходе через ноль
		errorBr=reg - tenRealPower;
	}
	digitalWrite(heater, outHeater); //пин через который осуществляется дискретное управление реле
	;
}

void initHeater()
{
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
	//HeaterOff.attach_ms(10000, setPWM, 0);
	//HTTP.on("/SetHeaterPower", handleSetHeaterPower);    // Установка уставки для ректификации (вкладка Reflux)
	//HTTP.on("/heater.json", handleHeaterJSON); // формирование heater.json страницы для передачи данных в web интерфейс
}

/*
void handleSetHeaterPower()
{              //
	power.heaterPower = HTTP.arg("heaterPower").toInt();         // Получаем значение мощности ТЭНа из запроса и сохраняем в глобальной переменной
	power.heaterStatus = HTTP.arg("heaterStatus").toInt();

	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении

}
void handleHeaterJSON()
{
	String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате

	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(root);

	json["heaterPower"] = power.heaterPower;

	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}
*/