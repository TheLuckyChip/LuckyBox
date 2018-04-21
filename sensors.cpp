#include "sensors.h"
#include "setting.h"
OneWire ds(DS_Pin);

struct DS_Str dallas_my_sensor[DS_Cnt];
void dallSearch()
{
	int i = 1, err_count = DS_Count;
	while (i)
	{
		if (!ds.search(dallas_my_sensor[i - 1].addr, false)) //  определяем адрес на шине 1wire и проверяем наличие датчиков 
		{
			ds.reset(); // запрос шины 1-Wire
			ds.select(dallas_my_sensor[i - 1].addr); // выбор нашего DS18B20
			ds.write(0x4E); // запись в  scratchPad
			ds.write(0x00); // User byte 0 - Unused
			ds.write(0x00); // User byte 1 - Unused
			ds.write(0x7F); // выставляем 12 бит (0x7F)
			ds.reset(); // сбрасываем 1-Wire
		}
		else
		{
			err_count--;
			break;
		}
		i++;
		if (i == DS_Count) break;
	}
	DS_Count -= err_count;

	#if defined Debug_en
	Serial.print("Temperature sensor found: ");
	Serial.println(DS_Count);
	Serial.println("......");
	#endif
}

void dallRead()
{
	int i = DS_Count;
	while (i)
	{
		ds.reset();
		ds.select(dallas_my_sensor[i - 1].addr);
		ds.write(0xBE); //Считывание значения с датчика
		dallas_my_sensor[i - 1].temperature = ( short )( ds.read() | ds.read() << 8 ); //Принимаем два байта температуры
		dallas_my_sensor[i - 1].temperature *= 0.0625;
		dallas_my_sensor[i - 1].temperature *= 100;
		dallas_my_sensor[i - 1].temperature = floor(dallas_my_sensor[i - 1].temperature + 0.5);
		dallas_my_sensor[i - 1].temperature /= 100;
		i--;
	}

	if (DS_Count >= 1) temperature1 = dallas_my_sensor[0].temperature;
	if (DS_Count >= 2) temperature2 = dallas_my_sensor[1].temperature;
	if (DS_Count >= 3) temperature3 = dallas_my_sensor[2].temperature;
	if (DS_Count >= 4) temperature4 = dallas_my_sensor[3].temperature;
	if (DS_Count >= 5) temperature5 = dallas_my_sensor[4].temperature;
	if (DS_Count >= 6) temperature6 = dallas_my_sensor[5].temperature;
	if (DS_Count >= 7) temperature7 = dallas_my_sensor[6].temperature;
	if (DS_Count >= 8) temperature8 = dallas_my_sensor[7].temperature;

	ds.reset();
	ds.write(0xCC); //Обращение ко всем датчикам
	ds.write(0x44); //Команда на конвертацию
}