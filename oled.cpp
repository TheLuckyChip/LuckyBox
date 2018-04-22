#include "oled.h"
#include "time_config.h"
#include "setting.h"
#include "user_config.h"

Adafruit_SSD1306 display(OLED_RESET);

void initOLED()
{
	display.begin(SSD1306_EXTERNALVCC, 0x3C);
	display.invertDisplay(1);
	display.display();
}
void oledStartNormal()
{
	display.clearDisplay();
	display.setTextColor(WHITE, BLACK);
	display.invertDisplay(0);
	display.display();
}
void oledOutDisplay()
{
	String time_ntp = GetTime();
	int count, temp_min = 1000, temp_max = 0;;
	int temp_rez, temp_dec, temp_inc;

	if (readTempInterval >= OLED_out_temp || ( int )temp_in[119] != ( int )( temperature1 * 10 ))
	{

		display.clearDisplay();
		//                 x1, y1, x2, y2
		display.drawLine(0, 63, 127, 63, WHITE); // низ
		display.drawLine(0, 18, 0, 63, WHITE);   // лево
		display.drawLine(0, 18, 127, 18, WHITE); // верх
		display.drawLine(127, 18, 127, 63, WHITE); // право
		display.drawPixel(0, 38, BLACK);
		display.drawPixel(1, 38, WHITE);
		display.drawPixel(2, 38, WHITE);

		// сдвиг показаний массива
		for (count = 0; count<119; count++)
		{
			temp_in[count] = temp_in[count + 1];
			if (temp_in[count] < temp_min) temp_min = temp_in[count];
			if (temp_in[count] > temp_max) temp_max = temp_in[count];
			//Serial.println(temp_in[count]);
		}
		#if defined Debug_en
		Serial.println(read_temp_interval);
		Serial.println("....");
		#endif
		temp_in[119] = temperature1 * 10;
		if (temp_in[119] < temp_min) temp_min = temp_in[119];
		if (temp_in[119] > temp_max) temp_max = temp_in[119];

		display.drawPixel(121, 38, WHITE);
		for (count = 118; count>0; count--)
		{
			// текущая точка меньше последующей - черта вверх
			if (temp_in[count] < temp_in[count + 1])
			{
				temp_inc = temp_in[count + 1] - temp_in[count];
				temp_dec = 0;
			}
			else
			{
				temp_dec = temp_in[count] - temp_in[count + 1];
				temp_inc = 0;
			}
			if (temp_in[count] <= temp_in[119])
			{
				temp_rez = 38 + ( temp_in[119] - temp_in[count] );
				if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
				else
				{
					while (1)
					{
						if (temp_dec > 1)
						{
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
							temp_rez++;
							temp_dec--;
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
						}
						else if (temp_inc > 1)
						{
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
							temp_rez--;
							temp_inc--;
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
						}
						else break;
					}
				}
			}
			if (temp_in[count] > temp_in[119])
			{
				temp_rez = 38 - ( temp_in[count] - temp_in[119] );
				if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
				else
				{
					while (1)
					{
						if (temp_dec > 1)
						{
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
							temp_rez++;
							temp_dec--;
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
						}
						else if (temp_inc > 1)
						{
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
							temp_rez--;
							temp_inc--;
							if (temp_rez <= 62 && temp_rez >= 18) display.drawPixel(count + 2, temp_rez, WHITE);
						}
						else break;
					}
				}
			}
		}
		// вывод текущей температуры для графика крупно
		display.setTextSize(2);
		display.setCursor(14, 1);
		display.printf("%d.%d", temp_in[119] / 10, temp_in[119] % 10);
		// вывод на график показаний мин и макс значений в последних 120 измерениях
		display.setTextSize(1);
		display.setCursor(2, 20);
		display.printf("%d.%d", temp_max / 10, temp_max % 10);
		display.setCursor(2, 55);
		display.printf("%d.%d", temp_min / 10, temp_min % 10);
		// вывод времени и давления
		display.setCursor(80, 0);
		display.print(time_ntp);
		display.setCursor(92, 9);
		display.printf("%d mm", ( int )pressure);
		readTempInterval = 0;
	}
	else
	{
		// вывод времени и давления
		display.setCursor(80, 0);
		display.print(time_ntp);
		readTempInterval++;
	}
	display.display();
}


