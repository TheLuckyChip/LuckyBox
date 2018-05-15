#include "tft.h"
#include "sensors.h"
#include "time_config.h"
#include "setting.h"
#include "user_config.h"

#define SPI_SETTING     SPISettings(24000000, MSBFIRST, SPI_MODE0)

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RES);
struct DS_Graph dallas_graph[4];
String time_ntp_old;
int temp_in_old;

#define UP_Y_axis	50
#define DW_Y_axis	220
#define LF_X_axis	2
#define RT_X_axis	317
#define MID_Y_axis	((DW_Y_axis - UP_Y_axis) / 2 + UP_Y_axis) - 30		// середина по Y

void initTFT() {
	tft.begin();
	SPI.setFrequency(24000000);
	tft.fillScreen(ILI9341_BLACK);
	tft.setCursor(0, 0);
	tft.setTextColor(ILI9341_GREEN);
	tft.setTextSize(1);
	tft.setRotation(3);
	tft.println("Setup...");
	tft.println("");
}
void tftStartForGraph() {
	dallas_graph[DS_Tube].color = ILI9341_CYAN;
	dallas_graph[DS_Out].color = ILI9341_GREEN;
	dallas_graph[DS_Cube].color = ILI9341_RED;// ILI9341_LIGHTMAROON;
	dallas_graph[DS_Def].color = ILI9341_DARKGREY;
	int temp_graph_1 = dallas_my_sensor[DS_Tube].temperature * 10.0;
	int temp_graph_2 = dallas_my_sensor[DS_Out].temperature * 10.0;
	int temp_graph_3 = dallas_my_sensor[DS_Cube].temperature * 10.0;
	int temp_graph_4 = dallas_my_sensor[DS_Def].temperature * 10.0;
	for (int cnt = 0; cnt < 312; cnt++) {
		dallas_graph[DS_Tube].temp_in_tft[cnt] = temp_graph_1; // пар
		dallas_graph[DS_Out].temp_in_tft[cnt] = temp_graph_2; // царга
		dallas_graph[DS_Cube].temp_in_tft[cnt] = temp_graph_3; //10; // куб
		dallas_graph[DS_Def].temp_in_tft[cnt] = temp_graph_4; //10; // деф
	}
}

// Вывод температуры дефлегматора
void dravGraphDef(uint16_t iCnt, uint16_t count, uint8_t shiftCnt, uint16_t color) {
	int temp_rez, temp_rez_prev;
	int temp_dec, temp_inc;

	if (dallas_graph[iCnt].temp_in_tft[count] >= 100 || dallas_graph[iCnt].temp_in_tft[count + 1] >= 100) {
		if (dallas_graph[iCnt].temp_in_tft[count] >= 100) temp_rez = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count] - 100) / 10);
		else temp_rez = DW_Y_axis - 1;
		if (dallas_graph[iCnt].temp_in_tft[count + 1] >= 100) temp_rez_prev = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count + 1] - 100) / 10);
		else temp_rez_prev = DW_Y_axis - 1;

		// куда выводить, вверх или вниз
		if (temp_rez < temp_rez_prev) {
			temp_dec = temp_rez_prev - temp_rez;
			temp_inc = 0;
		}
		else {
			temp_inc = temp_rez - temp_rez_prev;
			temp_dec = 0;
		}
		// вывод графика
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
		else {
			while (1) {
				if (temp_dec > 0) { // черта вниз
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez++;	temp_dec--;
				}
				else if (temp_inc > 0) { // черта вверх
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez--;	temp_inc--;
				}
				else break;
			}
		}
	}
}

// вывод температуры куба от 20 до 100
void dravGraphCub(uint16_t iCnt, uint16_t count, uint8_t shiftCnt, uint16_t color) {
	int temp_rez, temp_rez_prev;
	int temp_dec, temp_inc;

	if (dallas_graph[iCnt].temp_in_tft[count] >= 200 || dallas_graph[iCnt].temp_in_tft[count + 1] >= 200) {
		if (dallas_graph[iCnt].temp_in_tft[count] >= 200) temp_rez = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count] - 200) / 4);
		else temp_rez = DW_Y_axis - 1;
		if (dallas_graph[iCnt].temp_in_tft[count + 1] >= 200) temp_rez_prev = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count + 1] - 200) / 4);
		else temp_rez_prev = DW_Y_axis - 1;

		// куда выводить, вверх или вниз
		if (temp_rez < temp_rez_prev) {
			temp_dec = temp_rez_prev - temp_rez;
			temp_inc = 0;
		}
		else {
			temp_inc = temp_rez - temp_rez_prev;
			temp_dec = 0;
		}
		// вывод графика
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
		else {
			while (1) {
				if (temp_dec > 0) { // черта вниз
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez++;	temp_dec--;
				}
				else if (temp_inc > 0) { // черта вверх
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez--;	temp_inc--;
				}
				else break;
			}
		}
	}
}

// вывод графиков температуры в царге и выход пара
void dravGraph(uint16_t iCnt, uint16_t count, uint16_t countCnt, uint8_t shiftCnt, uint16_t color) {
	int temp_rez;
	int temp_dec, temp_inc;

	// куда выводить, вверх или вниз
	if (dallas_graph[iCnt].temp_in_tft[count] < dallas_graph[iCnt].temp_in_tft[count + 1]) {
		temp_inc = dallas_graph[iCnt].temp_in_tft[count + 1] - dallas_graph[iCnt].temp_in_tft[count];
		temp_dec = 0;
	}
	else {
		temp_dec = dallas_graph[iCnt].temp_in_tft[count] - dallas_graph[iCnt].temp_in_tft[count + 1];
		temp_inc = 0;
	}

	// вывод графика
	if (dallas_graph[iCnt].temp_in_tft[count] <= dallas_graph[DS_Tube].temp_in_tft[countCnt]) {
		temp_rez = MID_Y_axis + (dallas_graph[DS_Tube].temp_in_tft[countCnt] - dallas_graph[iCnt].temp_in_tft[count]);
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
		else {
			while (1) {
				if (temp_dec > 0) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez++;	temp_dec--;
				}
				else if (temp_inc > 0) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez--;	temp_inc--;
				}
				else break;
			}
		}
	}
	else {
		temp_rez = MID_Y_axis - (dallas_graph[iCnt].temp_in_tft[count] - dallas_graph[DS_Tube].temp_in_tft[countCnt]);
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
		else {
			while (1) {
				if (temp_dec > 0) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez++;	temp_dec--;
				}
				else if (temp_inc > 0) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + shiftCnt, temp_rez, color);
					temp_rez--;	temp_inc--;
				}
				else break;
			}
		}
	}
}





void tftOutGraphDisplay() {
	String time_ntp = GetTime();
	int count, temp_min = 1000, temp_max = 0;

	csOn(14);
	SPI.beginTransaction(SPI_SETTING);

	//if (readTempInterval >= Display_out_temp || dallas_graph[DS_Tube].temp_in_tft[309] != (int)(dallas_my_sensor[DS_Cube].temperature * 10.0)) {
		//readTempInterval = 0;

		// сдвиг массива выводимых данных
		for (count = 0; count < 311; count++) {
			if (DefCubOut >= 10) {
				dallas_graph[DS_Def].temp_in_tft[count] = dallas_graph[DS_Def].temp_in_tft[count + 1];
				dallas_graph[DS_Cube].temp_in_tft[count] = dallas_graph[DS_Cube].temp_in_tft[count + 1];
			}
			dallas_graph[DS_Out].temp_in_tft[count] = dallas_graph[DS_Out].temp_in_tft[count + 1];
			dallas_graph[DS_Tube].temp_in_tft[count] = dallas_graph[DS_Tube].temp_in_tft[count + 1];
			if (dallas_graph[DS_Tube].temp_in_tft[count] < temp_min) temp_min = dallas_graph[DS_Tube].temp_in_tft[count];
			if (dallas_graph[DS_Tube].temp_in_tft[count] > temp_max) temp_max = dallas_graph[DS_Tube].temp_in_tft[count];
		}
		if (DefCubOut >= 10) {
			dallas_graph[DS_Def].temp_in_tft[311] = (int)(dallas_my_sensor[DS_Def].temperature * 10.0);
			dallas_graph[DS_Cube].temp_in_tft[311] = (int)(dallas_my_sensor[DS_Cube].temperature * 10.0);
			DefCubOut = 0;
		}
		dallas_graph[DS_Out].temp_in_tft[311] = (int)(dallas_my_sensor[DS_Out].temperature * 10.0);
		dallas_graph[DS_Tube].temp_in_tft[311] = (int)(dallas_my_sensor[DS_Tube].temperature * 10.0);
		if (dallas_graph[DS_Tube].temp_in_tft[311] < temp_min) temp_min = dallas_graph[DS_Tube].temp_in_tft[311];
		if (dallas_graph[DS_Tube].temp_in_tft[311] > temp_max) temp_max = dallas_graph[DS_Tube].temp_in_tft[311];

		for (count = 309; count > 1; count--) {
			// стираем старый график
			dravGraphDef(DS_Def, count - 1, 3, ILI9341_BLACK);
			dravGraphCub(DS_Cube, count - 1, 3, ILI9341_BLACK);
			dravGraph(DS_Out, count - 1, 309, 3, ILI9341_BLACK);
			dravGraph(DS_Tube, count - 1, 309, 3, ILI9341_BLACK);

			// выводим сетку
			if ((count + 2) % 10 == 0) {
				for (int i = UP_Y_axis; i < DW_Y_axis; i += 10) {
					tft.drawPixel(count + 2, i, ILI9341_DARKDARK);
				}
			}

			// графики с самым низким приоритетом
			dravGraphDef(DS_Def, count, 2, dallas_graph[DS_Def].color);
			dravGraphCub(DS_Cube, count, 2, dallas_graph[DS_Cube].color);
			// график со средним приоритетом
			dravGraph(DS_Out, count, 310, 2, dallas_graph[DS_Out].color);
			// график с самым высоким приоритетом
			dravGraph(DS_Tube, count, 310, 2, dallas_graph[DS_Tube].color);
		}

		// выводим координаты
		//                 x1, y1, x2, y2
		tft.drawLine(LF_X_axis, DW_Y_axis, RT_X_axis, DW_Y_axis, ILI9341_DARKGREY);  // X axis
		tft.drawPixel(LF_X_axis - 1, UP_Y_axis + 1, ILI9341_DARKGREY); tft.drawPixel(LF_X_axis + 1, UP_Y_axis + 1, ILI9341_DARKGREY);
		tft.drawPixel(LF_X_axis - 2, UP_Y_axis + 2, ILI9341_DARKGREY); tft.drawPixel(LF_X_axis + 2, UP_Y_axis + 2, ILI9341_DARKGREY);

		tft.drawLine(LF_X_axis, UP_Y_axis, LF_X_axis, DW_Y_axis, ILI9341_DARKGREY);  // Y axis
		tft.drawPixel(RT_X_axis - 1, DW_Y_axis - 1, ILI9341_DARKGREY); tft.drawPixel(RT_X_axis - 1, DW_Y_axis + 1, ILI9341_DARKGREY);
		tft.drawPixel(RT_X_axis - 2, DW_Y_axis - 2, ILI9341_DARKGREY); tft.drawPixel(RT_X_axis - 2, DW_Y_axis + 2, ILI9341_DARKGREY);
		tft.drawLine(LF_X_axis + 1, UP_Y_axis + 2, LF_X_axis + 1, DW_Y_axis - 1, ILI9341_BLACK);

		// вывод цивровых значений
		// вывод максимальной и минимальной температуры основного графика
		tft.setTextSize(2);
		if (settingAlarm == true) tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK); // цвет = розовый
		else tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		tft.setCursor(2, 2);
		tft.printf("\x1e");
		tft.setCursor(20, 2);
		tft.printf("%d.%d", temp_max / 10, temp_max % 10);
		tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		tft.setCursor(2, 22);
		tft.printf("\x1f");
		tft.setCursor(20, 22);
		tft.printf("%d.%d", temp_min / 10, temp_min % 10);

		// температура остальных датчиков снизу графика
		tft.setTextSize(2);
		tft.setCursor(18, DW_Y_axis + 5);
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK); 
		tft.printf("1:%.1f   3:%.1f   4:%.1f", dallas_my_sensor[DS_Cube].temperature, dallas_my_sensor[DS_Out].temperature, dallas_my_sensor[DS_Def].temperature);
		//tft.printf("X:%d     Y:%d     ", touch_x, touch_y); // test TachScreen

		// крупно температура основного графика
		if (temp_in_old != dallas_graph[DS_Tube].temp_in_tft[311]) {
			// сначала сотрем старый текст
			tft.setTextSize(1);
			tft.setFont(&FreeSerifBold24pt7b);
			tft.setCursor(98, 36);
			tft.setTextColor(ILI9341_BLACK);
			tft.printf("%d.%d", temp_in_old / 10, temp_in_old % 10);
			int k = tft.getCursorX();
			tft.setFont();
			tft.setTextSize(2);
			tft.setCursor(k + 8, 0);
			tft.printf("o");
			// выведем новое значение
			tft.setTextSize(1);
			tft.setFont(&FreeSerifBold24pt7b);
			tft.setCursor(98, 36);
			tft.setTextColor(ILI9341_YELLOW);
			tft.printf("%d.%d", dallas_graph[DS_Tube].temp_in_tft[311] / 10, dallas_graph[DS_Tube].temp_in_tft[311] % 10);
			k = tft.getCursorX();
			tft.setFont();
			tft.setTextSize(2);
			tft.setCursor(k + 8, 0);
			tft.printf("o");
		}
		// вывод времени идавления
		tft.setTextSize(2);
		tft.setCursor(223, 2);
		tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		tft.print(time_ntp);
		tft.setCursor(236, 22);
		tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
		tft.printf("%d mm", (int)pressure);
		//readTempInterval++;
	//}
	//else {
		// вывод времени, если не обновляем график
	//	tft.setTextSize(2);
	//	tft.setCursor(218, 2);
	//	tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
	//	tft.print(time_ntp);
	//	readTempInterval++;
	//}
	csOff(14);
	temp_in_old = dallas_graph[DS_Tube].temp_in_tft[311];

	if (touch_in == true) {
		if (touch_x > 110 && touch_x < 260 && touch_y < 60) dallSearch();
		touch_in = false;
	}
}

// Вывод первоначального меню
void tftOutMenu() {

	//fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
}