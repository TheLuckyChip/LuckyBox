#include "tft.h"
#include "time_config.h"
#include "setting.h"
#include "user_config.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RES);
struct DS_Graph dallas_graph[3];
String time_ntp_old;
int temp_in_old;
int max_old;
int min_old;
int max_old_old;
int min_old_old;

#define UP_Y_axis	50
#define DW_Y_axis	237
#define LF_X_axis	2
#define RT_X_axis	317
#define MID_Y_axis	(DW_Y_axis - UP_Y_axis) / 2 + UP_Y_axis		// середина по Y

void initTFT() {
	tft.begin(80000000);
	tft.fillScreen(ILI9341_BLACK);
	tft.setCursor(0, 0);
	tft.setTextColor(ILI9341_GREEN);
	tft.setTextSize(1);
	tft.setRotation(1);
	tft.println("Setup...");
	tft.println("");
}
void tftStartForGraph() {
	dallas_graph[0].color = ILI9341_CYAN;
	dallas_graph[1].color = ILI9341_GREEN;
	dallas_graph[2].color = ILI9341_LIGHTMAROON;
	int temp_graph_1 = temperature1 * 10.0;
	int temp_graph_2 = temperature2 * 10.0;
	int temp_graph_3 = temperature3 * 10.0;
	for (int cnt = 0; cnt < 310; cnt++) {
		dallas_graph[0].temp_in_tft[cnt] = temp_graph_1;
		dallas_graph[1].temp_in_tft[cnt] = temp_graph_2;
		dallas_graph[2].temp_in_tft[cnt] = temp_graph_3;
	}
	readTempInterval = Display_out_temp;
	tft.setTextColor(ILI9341_GREEN);
	tft.println("");
	tft.println("Done!!!");
	delay(2000);
	tft.fillScreen(ILI9341_BLACK);
}

int dravGraph(int iCnt, int count) {
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
	if (dallas_graph[iCnt].temp_in_tft[count] <= dallas_graph[0].temp_in_tft[309]) {
		temp_rez = MID_Y_axis + (dallas_graph[0].temp_in_tft[309] - dallas_graph[iCnt].temp_in_tft[count]);
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
		else {
			while (1) {
				if (temp_dec > 1) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
					temp_rez++;
					temp_dec--;
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
				}
				else if (temp_inc > 1) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
					temp_rez--;
					temp_inc--;
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
				}
				else break;
			}
		}
	}
	if (dallas_graph[iCnt].temp_in_tft[count] > dallas_graph[0].temp_in_tft[309]) {
		temp_rez = MID_Y_axis - (dallas_graph[iCnt].temp_in_tft[count] - dallas_graph[0].temp_in_tft[309]);
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
		else {
			while (1) {
				if (temp_dec > 1) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
					temp_rez++;
					temp_dec--;
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
				}
				else if (temp_inc > 1) {
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
					temp_rez--;
					temp_inc--;
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) tft.drawPixel(count + 2, temp_rez, dallas_graph[iCnt].color);
				}
				else break;
			}
		}
	}

	return temp_rez;
}

void tftOutGraphDisplay() {
	String time_ntp = GetTime();
	int count, temp_min = 1000, temp_max = 0;
	int temp_rez;

	if (readTempInterval >= Display_out_temp || dallas_graph[0].temp_in_tft[309] != (int)(temperature1 * 10.0)) {
		readTempInterval = 0;

		// сдвиг массива выводимых данных
		for (count = 0; count<309; count++) {
			dallas_graph[2].temp_in_tft[count] = dallas_graph[2].temp_in_tft[count + 1];
			dallas_graph[1].temp_in_tft[count] = dallas_graph[1].temp_in_tft[count + 1];
			dallas_graph[0].temp_in_tft[count] = dallas_graph[0].temp_in_tft[count + 1];
			if (dallas_graph[0].temp_in_tft[count] < temp_min) temp_min = dallas_graph[0].temp_in_tft[count];
			if (dallas_graph[0].temp_in_tft[count] > temp_max) temp_max = dallas_graph[0].temp_in_tft[count];
		}
		dallas_graph[2].temp_in_tft[309] = (int)(temperature3 * 10.0);
		dallas_graph[1].temp_in_tft[309] = (int)(temperature2 * 10.0);
		dallas_graph[0].temp_in_tft[309] = (int)(temperature1 * 10.0);
		if (dallas_graph[0].temp_in_tft[309] < temp_min) temp_min = dallas_graph[0].temp_in_tft[309];
		if (dallas_graph[0].temp_in_tft[309] > temp_max) temp_max = dallas_graph[0].temp_in_tft[309];

		//tft.drawPixel(311, MID_Y_axis, ILI9341_WHITE); // вывод первой точки всегда в центре
		max_old = MID_Y_axis;
		min_old = MID_Y_axis;

		for (count = 308; count>0; count--) {
			// для упрощения стираем старый график вертикальными линиями от max до min предидущего вывода
			tft.drawLine(count + 2, min_old_old, count + 2, max_old_old, ILI9341_BLACK);

			// выводим сетку
			if ((count + 2) % 10 == 0) {
				for (int i = UP_Y_axis; i < DW_Y_axis; i += 10) {
					tft.drawPixel(count + 2, i, ILI9341_DARKDARK);
				}
			}

			// график с самым низким приоритетом
			temp_rez = dravGraph(2, count);
			// для последующего стирания области где был график
			if (min_old > temp_rez) min_old = temp_rez; // верх
			if (max_old < temp_rez) max_old = temp_rez; // низ
														// график со средним приоритетом
			temp_rez = dravGraph(1, count);
			// для последующего стирания области где был график
			if (min_old > temp_rez) min_old = temp_rez; // верх
			if (max_old < temp_rez) max_old = temp_rez; // низ
														// график с самым высоким приоритетом
			temp_rez = dravGraph(0, count);
			// для последующего стирания области где был график
			if (min_old > temp_rez) min_old = temp_rez; // верх
			if (max_old < temp_rez) max_old = temp_rez; // низ
		}

		min_old_old = min_old - 1;
		if (min_old_old < UP_Y_axis) min_old_old = UP_Y_axis;
		max_old_old = max_old + 1;
		if (max_old_old >= DW_Y_axis) max_old_old = DW_Y_axis - 1;

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
		tft.setCursor(8, UP_Y_axis + 4);
		tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		tft.printf("%d.%d", temp_max / 10, temp_max % 10);
		tft.setCursor(8, DW_Y_axis - 18);
		tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		tft.printf("%d.%d", temp_min / 10, temp_min % 10);
		// крупно температура основного графика
		if (temp_in_old != dallas_graph[0].temp_in_tft[309]) {
			// сначала сотрем старый текст
			tft.setTextSize(1);
			tft.setFont(&FreeSerifBold24pt7b);
			tft.setCursor(28, 36);
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
			tft.setCursor(28, 36);
			tft.setTextColor(ILI9341_YELLOW);
			tft.printf("%d.%d", dallas_graph[0].temp_in_tft[309] / 10, dallas_graph[0].temp_in_tft[309] % 10);
			k = tft.getCursorX();
			tft.setFont();
			tft.setTextSize(2);
			tft.setCursor(k + 8, 0);
			tft.printf("o");
		}
		// вывод времени идавления
		tft.setTextSize(2);
		tft.setCursor(218, 0);
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
		tft.print(time_ntp);
		tft.setCursor(228, 18);
		tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
		tft.printf("%d mm", (int)pressure);
		readTempInterval++;
	}
	else {
		// вывод времени, если не обновляем график
		tft.setTextSize(2);
		tft.setCursor(218, 0);
		tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
		tft.print(time_ntp);
		readTempInterval++;
	}
	temp_in_old = dallas_graph[0].temp_in_tft[309];
}
