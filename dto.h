// dto.h

#ifndef _DTO_h
#define _DTO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
// структура для передачи данных клиенту
class dto
{

public:
	float temperatures[8];	// Датчики температуры

	/*		ТЭН		*/
	float settingTank;		// Температура отключения нагрева куба
	float heaterPower;		// Установленная мощность ТЭНа
	bool heaterStatus;		// Состояние ТЭНа вкл/выкл

	void init();
	String getJson();		// Формируем строку для отправки в файл конфигурации в json формате
};


#endif

