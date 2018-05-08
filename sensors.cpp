#include "sensors.h"
#include "setting.h"
#include "tft.h"

OneWire ds(DS_Pin);

struct DS_Str dallas_my_sensor[DS_Cnt];
void dallSearch()
{
	int i;
	// поиск датчиков, определение их количества и сохранение их адресов
	for (i = 0; i < DS_Cnt; i++) {
		if (!ds.search(dallas_my_sensor[i].addr)) break;
		if (ds.crc8(dallas_my_sensor[i].addr, 7) != dallas_my_sensor[i].addr[7]) break;
	}
	DS_Count = i;
	// установим разрядность, и пропустим симофоры
	for (i = 0; i < DS_Count; i++) {
		ds.reset(); // запрос шины 1-Wire
		ds.select(dallas_my_sensor[i - 1].addr); // выбор нашего DS18B20
		ds.write(0x4E); // запись в  scratchPad
		ds.write(0x00); // User byte 0 - Unused
		ds.write(0x00); // User byte 1 - Unused
		ds.write(0x7F); // выставляем 12 бит (0x7F)
	}
	ds.reset(); // сбрасываем 1-Wire

#if defined TFT_Display
	tft.setTextSize(1);
	tft.println("");
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("DS18B20 found: ");
	tft.println(DS_Count);
	tft.setTextSize(1);
	tft.println("");
	tft.setTextColor(ILI9341_GREEN);
#endif

#if defined Debug_en
	Serial.print("Temperature sensor found: ");
	Serial.println(DS_Count);
	Serial.println("......");
#endif
}

void dallRead()
{
	  int i = DS_Count;
	  while (i) {
		  ds.reset();
		  ds.select(dallas_my_sensor[i-1].addr);
		  ds.write(0xBE); //Считывание значения с датчика
		  dallas_my_sensor[i-1].temperature = (short)(ds.read() | ds.read() << 8); //Принимаем два байта температуры
		  dallas_my_sensor[i-1].temperature *= 0.0625;
		  dallas_my_sensor[i-1].temperature *= 100;
		  dallas_my_sensor[i-1].temperature = floor(dallas_my_sensor[i-1].temperature + 0.5);
		  dallas_my_sensor[i-1].temperature /= 100;

		  temperatures[i - 1] = dallas_my_sensor[i - 1].temperature;
		  i--;
	  }

	  // TODO выпилить
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
