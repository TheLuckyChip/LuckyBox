#include "tft.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS_Lib, TFT_DC_Lib, TFT_RES_Lib);
struct DS_Graph dallas_graph[4];
String time_ntp_old;
int temp_in_old, temp_convert;
uint16_t grColor;
unsigned long timeOffMenu;

void fillScreenRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
	SPI1U1 = ((SPI1U1 & mask) | ((7 << SPILMOSI) | (7 << SPILMISO)));
	tft.setAddrWindow(x, y, w, h);
	SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
	for (uint32_t t = w * h; t; t--) {
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color >> 8;
		SPI1CMD |= SPIBUSY;
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color;
		SPI1CMD |= SPIBUSY;
	}
	while (SPI1CMD & SPIBUSY) {}
	SPI.endTransaction();
}
void drawScreen4bitMonoBitmap(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *count) {
	uint16_t data_count = 0;
	uint16_t color, tmp_color;
	uint8_t data_mem, data_rec;
	const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
	SPI1U1 = ((SPI1U1 & mask) | ((7 << SPILMOSI) | (7 << SPILMISO)));
	tft.setAddrWindow(x, y, w, h);
	SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
	for (uint32_t t = (w * h)/2; t; t--) {
		data_mem = pgm_read_byte_near(count + data_count);
		
		data_rec = data_mem >> 4;
		if (data_rec == 0x0F) color = 0xFFFF;
		else {
			color = (data_rec * 2) << 11;
			tmp_color = (data_rec * 4) << 5;
			color += (data_rec * 2 + tmp_color);
		}
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color >> 8;
		SPI1CMD |= SPIBUSY;
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color;
		SPI1CMD |= SPIBUSY;

		data_rec = data_mem & ~0xF0;
		if (data_rec == 0x0F) color = 0xFFFF;
		else {
			color = (data_rec * 2) << 11;
			tmp_color = (data_rec * 4) << 5;
			color += (data_rec * 2 + tmp_color);
		}
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color >> 8;
		SPI1CMD |= SPIBUSY;
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color;
		SPI1CMD |= SPIBUSY;
		data_count++;
	}
	while (SPI1CMD & SPIBUSY) {}
	SPI.endTransaction();
}
void drawScreenBigNum(uint16_t x, uint16_t y, uint8_t num, uint16_t color, uint16_t bgcolor) {
	uint8_t simbol;
	uint8_t i;
	uint16_t drawColor;
	const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
	SPI1U1 = ((SPI1U1 & mask) | ((7 << SPILMOSI) | (7 << SPILMISO)));
	tft.setAddrWindow(x, y, 32, 40);
	SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
	uint16_t ptr = num * 160;
	uint8_t b = 160;
	while (b) {
		simbol = image_data_Font_Num[ptr];
		for (i = 0; i < 8; i++) {
			if (simbol & 0x80) drawColor = bgcolor;
			else drawColor = color;
			simbol <<= 1;
			while (SPI1CMD & SPIBUSY) {}
			SPI1W0 = drawColor >> 8;
			SPI1CMD |= SPIBUSY;
			while (SPI1CMD & SPIBUSY) {}
			SPI1W0 = drawColor;
			SPI1CMD |= SPIBUSY;
		}
		b--;
		ptr++;
	}
	while (SPI1CMD & SPIBUSY) {}
	SPI.endTransaction();
}
void drawBigTemp(uint16_t x, uint16_t y, int data, uint16_t color, uint16_t bgcolor) {
	uint8_t r1, r2, r3;
	if (data < 1000 && data > 0) {
		r1 = (uint8_t)(data / 100);
		data -= r1 * 100;
		r2 = (uint8_t)(data / 10);
		r3 = (uint8_t)(data - r2 * 10);
	}
	else {
		r1 = 10;
		r2 = 10;
		r3 = 10;
	}
	if (r1 == 0) fillScreenRect(x, y, 32, 40, bgcolor);
	else drawScreenBigNum(x, y, r1, color, bgcolor);
	drawScreenBigNum(x + 32, y, r2, color, bgcolor);
	tft.fillCircle(x + 67, y + 35, 3, color);
	drawScreenBigNum(x + 72, 2, r3, color, bgcolor);
	tft.drawCircle(x + 114, y + 5, 5, color);
	tft.drawCircle(x + 114, y + 5, 4, color);
	tft.drawCircle(x + 114, y + 5, 3, color);
}
String utf8rus(String source)
{
	int i, k;
	String target;
	unsigned char n;
	char m[2] = { '0', '\0' };

	k = source.length(); i = 0;

	while (i < k) {
		n = source[i]; i++;

		if (n >= 0xC0) {
			switch (n) {
			case 0xD0: {
				n = source[i]; i++;
				if (n == 0x81) { n = 0xA8; break; }
				if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
				break;
			}
			case 0xD1: {
				n = source[i]; i++;
				if (n == 0x91) { n = 0xB8; break; }
				if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
				break;
			}
			}
		}
		m[0] = n; target = target + String(m);
	}
	return target;
}
void drawBitmapString(uint16_t x, uint16_t y, const tImage *image, uint16_t color, uint16_t bgcolor) {
	uint16_t data;
	uint8_t  data_mem;
	uint16_t data_count = 0;
	//uint16_t w = image->width;
	//uint16_t h = image->height;
	tft.setAddrWindow(x, y, image->width, image->height);
	for (uint32_t t = (image->width*image->height)/8; t; t--) {
		data_mem = pgm_read_byte_near(image->data + data_count);
		for (uint8_t i = 0; i < 8; i++) {
			if (data_mem & 0x80) data = bgcolor;
			else data = color;
			while (SPI1CMD & SPIBUSY) {}
			SPI1W0 = data >> 8;
			SPI1CMD |= SPIBUSY;
			while (SPI1CMD & SPIBUSY) {}
			SPI1W0 = data;
			SPI1CMD |= SPIBUSY;
			data_mem <<= 1;
		}
		data_count++;
	}
	while (SPI1CMD & SPIBUSY) {}
	SPI.endTransaction();
}

void initTFT() {
	tft.begin();
	tft.cp437(true);
	if (tftInvert == false) tft.setRotation(3);
	else tft.setRotation(1);
	fillScreenRect(0, 0, 320, 240, ILI9341_BLACK);
	tft.setCursor(0, 2);
	tft.setTextColor(ILI9341_WHITE);
	tft.setTextSize(1);
	tft.print(utf8rus("Версия ПО: "));
	tft.print(curVersion);
	tft.setCursor(199, 2);
	tft.print("    SN: ");
	tft.print(addrMacMod);
}
void tftStartForGraph() {
	fillScreenRect(0, 0, 320, 240, ILI9341_BLACK);
	if (sdStatus == true) tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	tft.setTextSize(1);
	tft.setCursor(2, DW_Y_axis + 5);
	tft.printf("SD");
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setTextSize(2);
	tft.setCursor(0, 2);
	if (processMode.allow == 1) drawScreen4bitMonoBitmap(0, 10, 32, 32, imageProcessDistillation);
	else if (processMode.allow == 2) drawScreen4bitMonoBitmap(0, 10, 32, 32, imageProcessRectification);
	else if (processMode.allow == 3) drawScreen4bitMonoBitmap(0, 10, 32, 32, imageProcessMashing);
	else if (processMode.allow == 4) tft.print("PID set");

	if (DS_Cube == 10) dallas_graph[0].color = ILI9341_DARKDARK;
	else if (temperatureSensor[DS_Cube].color == 0) dallas_graph[0].color = 0xFC4C; // розовый
	else dallas_graph[0].color = temperatureSensor[DS_Cube].color;

	if (DS_Tube == 10) dallas_graph[1].color = ILI9341_DARKDARK;
	else if (temperatureSensor[DS_Tube].color == 0) dallas_graph[1].color = ILI9341_YELLOW; // желтый
	else dallas_graph[1].color = temperatureSensor[DS_Tube].color;
	
	if (DS_Out == 10) dallas_graph[2].color = ILI9341_DARKDARK;
	else if (temperatureSensor[DS_Out].color == 0) dallas_graph[2].color = 0x1EE1; // зеленый
	else dallas_graph[2].color = temperatureSensor[DS_Out].color;

	if (DS_Def == 10) dallas_graph[3].color = ILI9341_DARKDARK;
	else if (temperatureSensor[DS_Def].color == 0) dallas_graph[3].color = 0x419F; // светло синий
	else dallas_graph[3].color = temperatureSensor[DS_Def].color;

	int temp_graph_1 = temperatureSensor[DS_Cube].data * 10.0;
	int temp_graph_2 = temperatureSensor[DS_Tube].data * 10.0;
	int temp_graph_3 = temperatureSensor[DS_Out].data * 10.0;
	int temp_graph_4 = temperatureSensor[DS_Def].data * 10.0;
	for (int cnt = 0; cnt < 312; cnt++) {
		dallas_graph[0].temp_in_tft[cnt] = temp_graph_1;
		dallas_graph[1].temp_in_tft[cnt] = temp_graph_2;
		dallas_graph[2].temp_in_tft[cnt] = temp_graph_3;
		dallas_graph[3].temp_in_tft[cnt] = temp_graph_4;
	}
	temp_in_old = 0;
}

// вывод графика температуры от 20 до 100
void dravGraph(uint16_t iCnt, uint16_t count, uint8_t shiftCnt, uint16_t color) {
	int temp_rez, temp_rez_prev;
	int temp_dec, temp_inc;

	if (dallas_graph[iCnt].temp_in_tft[count] >= 200 || dallas_graph[iCnt].temp_in_tft[count + 1] >= 200) {
		if (dallas_graph[iCnt].temp_in_tft[count] >= 200) temp_rez = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count] - 200) / 5.5);
		else temp_rez = DW_Y_axis - 1;
		if (dallas_graph[iCnt].temp_in_tft[count + 1] >= 200) temp_rez_prev = DW_Y_axis - ((dallas_graph[iCnt].temp_in_tft[count + 1] - 200) / 5.5);
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
		if (temp_dec <= 1 && temp_inc <= 1 && temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) {
			if ((count + shiftCnt) > 57 || processMode.allow != 2) tft.drawPixel(count + shiftCnt, temp_rez, color);
			else if (temp_rez < (DW_Y_axis - 22) && temp_rez > (UP_Y_axis + 20)) tft.drawPixel(count + shiftCnt, temp_rez, color);
		}
		else {
			while (1) {
				if (temp_dec > 0) { // черта вниз
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) {
						if ((count + shiftCnt) > 57 || processMode.allow != 2) tft.drawPixel(count + shiftCnt, temp_rez, color);
						else if (temp_rez < (DW_Y_axis - 22) && temp_rez >(UP_Y_axis + 20)) tft.drawPixel(count + shiftCnt, temp_rez, color);
					}
					temp_rez++;	temp_dec--;
				}
				else if (temp_inc > 0) { // черта вверх
					if (temp_rez <= DW_Y_axis && temp_rez >= UP_Y_axis) {
						if ((count + shiftCnt) > 57 || processMode.allow != 2) tft.drawPixel(count + shiftCnt, temp_rez, color);
						else if (temp_rez < (DW_Y_axis - 22) && temp_rez >(UP_Y_axis + 20)) tft.drawPixel(count + shiftCnt, temp_rez, color);
					}
					temp_rez--;	temp_inc--;
				}
				else break;
			}
		}
	}
}

// Вывод текстовой информации
void tftOutText(int temp_min, int temp_max) {
	String time_ntp = GetTime();
	float tempOutTFT = 0;
	float timeOutTFT = 0;
	// вывод максимальной и минимальной температуры основного графика
	if (processMode.allow == 2) {
		tft.setTextSize(2);
		if (settingAlarm == true) tft.setTextColor(ILI9341_MAGENTA, ILI9341_BLACK); // цвет = розовый
		else tft.setTextColor(dallas_graph[1].color, ILI9341_BLACK);

		tft.setCursor(8, UP_Y_axis + 4);
		if (temp_max < 1000) tft.printf("%d.%d", temp_max / 10, temp_max % 10);
		else tft.printf("--.-");
		tft.setTextColor(dallas_graph[1].color, ILI9341_BLACK);

		tft.setCursor(8, DW_Y_axis - 20);
		if (temp_min < 1000) tft.printf("%d.%d", temp_min / 10, temp_min % 10);
		else tft.printf("--.-");
	}

	// вывод % остатка алкоголя в кубе
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setCursor(0, 0);
	tft.setTextSize(1);

	switch (processMode.allow) {
		case 1: {
			tft.print(utf8rus(" Дистилляция"));
			// какой шаг процесса
			tft.setCursor(36, 10);
			if (processMode.step < 2) tft.print(utf8rus("Нагрев"));
			else if (processMode.step < 4) tft.print(utf8rus(" Отбор"));
			else tft.print(utf8rus("  Стоп"));
			// время шага
			tft.setCursor(42, 20);
			if (processMode.timeStep < 36000) tft.print(" ");
			tft.print(processMode.timeStep / 3600);
			if (((processMode.timeStep / 60) % 60) >= 10) tft.print(":");
			else tft.print(":0");
			tft.print((processMode.timeStep / 60) % 60);
			// % алкоголя в кубе
			tft.setCursor(42, 30);
			if (temperatureCubeAlcohol <= 50 && temperatureCubeAlcohol >= 10) tft.printf("%.1f%% ", temperatureCubeAlcohol);
			else if (temperatureCubeAlcohol < 10 && temperatureCubeAlcohol >= 1) tft.printf(" %.1f%% ", temperatureCubeAlcohol);
			else tft.print("--.-% ");
			break;
		}
		case 2: {
			tft.print(utf8rus("Ректификация"));
			// какой шаг процесса
			tft.setCursor(36, 10);
			if (processMode.number > 0) {
				if (processMode.step < 2) tft.print(utf8rus("Нагрев"));
				else if (processMode.step < 4) tft.print(utf8rus(" Стаб."));
				else if (processMode.step < 6) tft.print(utf8rus("Головы"));
				else if (processMode.step < 7) tft.print(utf8rus("  Тело"));
				else tft.print(utf8rus("  Стоп"));
			}
			else tft.print(utf8rus(" Отбор"));
			// время шага
			tft.setCursor(42, 20);
			if (processMode.timeStep < 36000) tft.print(" ");
			tft.print(processMode.timeStep / 3600);
			if (((processMode.timeStep / 60) % 60) >= 10) tft.print(":");
			else tft.print(":0");
			tft.print((processMode.timeStep / 60) % 60);
			// % алкоголя в кубе
			tft.setCursor(42, 30);
			if (temperatureCubeAlcohol <= 50 && temperatureCubeAlcohol >= 10) tft.printf("%.1f%% ", temperatureCubeAlcohol);
			else if (temperatureCubeAlcohol < 10 && temperatureCubeAlcohol >= 1) tft.printf(" %.1f%% ", temperatureCubeAlcohol);
			else tft.print("--.-% ");
			break;
		}
		case 3: {
			tft.print(utf8rus("Затирание"));
			break;
		}
	}

	// температура остальных датчиков нижней строкой
	tft.setTextSize(2);
	switch (tempBigOut) {
		case 1: { 
			tft.setCursor(26, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[1].color, ILI9341_BLACK);
			if (DS_Tube != 10 && temperatureSensor[DS_Tube].data < 100.0) tft.printf("T2:%.1f ", temperatureSensor[DS_Tube].data);
			else tft.printf("T2:--.-");

			tft.setCursor(30 + 100, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[2].color, ILI9341_BLACK);
			if (DS_Out != 10 && temperatureSensor[DS_Out].data < 100.0) tft.printf("T3:%.1f ", temperatureSensor[DS_Out].data);
			else tft.printf("T3:--.-");

			tft.setCursor(30 + 204, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[3].color, ILI9341_BLACK);
			if (DS_Def != 10 && temperatureSensor[DS_Def].data < 100.0) tft.printf("T4:%.1f ", temperatureSensor[DS_Def].data);
			else tft.printf("T4:--.-");
			break;
		}
		case 2: {
			tft.setCursor(26, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[0].color, ILI9341_BLACK);
			if (DS_Cube != 10 && temperatureSensor[DS_Cube].data < 100.0) tft.printf("T1:%.1f ", temperatureSensor[DS_Cube].data);
			else tft.printf("T1:--.-");

			tft.setCursor(30 + 100, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[2].color, ILI9341_BLACK);
			if (DS_Out != 10 && temperatureSensor[DS_Out].data < 100.0) tft.printf("T3:%.1f ", temperatureSensor[DS_Out].data);
			else tft.printf("T3:--.-");

			tft.setCursor(30 + 204, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[3].color, ILI9341_BLACK);
			if (DS_Def != 10 && temperatureSensor[DS_Def].data < 100.0) tft.printf("T4:%.1f ", temperatureSensor[DS_Def].data);
			else tft.printf("T4:--.-");
			break;
		}
		case 3: {
			tft.setCursor(26, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[0].color, ILI9341_BLACK);
			if (DS_Cube != 10 && temperatureSensor[DS_Cube].data < 100.0) tft.printf("T1:%.1f ", temperatureSensor[DS_Cube].data);
			else tft.printf("T1:--.-");

			tft.setCursor(30 + 100, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[1].color, ILI9341_BLACK);
			if (DS_Tube != 10 && temperatureSensor[DS_Tube].data < 100.0) tft.printf("T2:%.1f ", temperatureSensor[DS_Tube].data);
			else tft.printf("T2:--.-");

			tft.setCursor(30 + 204, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[3].color, ILI9341_BLACK);
			if (DS_Def != 10 && temperatureSensor[DS_Def].data < 100.0) tft.printf("T4:%.1f ", temperatureSensor[DS_Def].data);
			else tft.printf("T4:--.-");
			break;
		}
		case 4: {
			tft.setCursor(26, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[0].color, ILI9341_BLACK);
			if (DS_Cube != 10 && temperatureSensor[DS_Cube].data < 100.0) tft.printf("T1:%.1f ", temperatureSensor[DS_Cube].data);
			else tft.printf("T1:--.-");

			tft.setCursor(30 + 100, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[1].color, ILI9341_BLACK);
			if (DS_Tube != 10 && temperatureSensor[DS_Tube].data < 100.0) tft.printf("T2:%.1f ", temperatureSensor[DS_Tube].data);
			else tft.printf("T2:--.-");

			tft.setCursor(30 + 204, DW_Y_axis + 5);
			tft.setTextColor(dallas_graph[2].color, ILI9341_BLACK);
			if (DS_Out != 10 && temperatureSensor[DS_Out].data < 100.0) tft.printf("T3:%.1f ", temperatureSensor[DS_Out].data);
			else tft.printf("T3:--.-");
			break;
		}
	}

	if (processMode.allow < 3) {
		// состояние входов и выходов для дистилляции и ректификации
		tft.setTextSize(2);
		tft.setCursor(0, 47);
		tft.setTextColor(ILI9341_LIGHTGREY);
		tft.printf("<<");
		if (adcIn[0].member == 0) tft.drawCircle(38, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(38, 54, 6, ILI9341_LIGHTGREY);
			if (adcIn[0].allert == true) tft.fillCircle(38, 54, 5, 0xFACB);
			else tft.fillCircle(38, 54, 5, ILI9341_BLACK);
		}
		if (adcIn[1].member == 0) tft.drawCircle(58, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(58, 54, 6, ILI9341_LIGHTGREY);
			if (adcIn[1].allert == true) tft.fillCircle(58, 54, 5, 0xFACB);
			else tft.fillCircle(58, 54, 5, ILI9341_BLACK);
		}
		if (adcIn[2].member == 0) tft.drawCircle(78, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(78, 54, 6, ILI9341_LIGHTGREY);
			if (adcIn[2].allert == true) tft.fillCircle(78, 54, 5, 0xFACB);
			else tft.fillCircle(78, 54, 5, ILI9341_BLACK);
		}
		if (adcIn[3].member == 0) tft.drawCircle(98, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(98, 54, 6, ILI9341_LIGHTGREY);
			if (adcIn[3].allert == true) tft.fillCircle(98, 54, 5, 0xFACB);
			else tft.fillCircle(98, 54, 5, ILI9341_BLACK);
		}
		tft.setCursor(135, 47);
		tft.setTextColor(ILI9341_LIGHTGREY);
		tft.printf(">>");
		if (pwmOut[0].member == 0) tft.drawCircle(173, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(173, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[0].allert == true) tft.fillCircle(173, 54, 5, 0x47EA);
			else tft.fillCircle(173, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[1].member == 0) tft.drawCircle(193, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(193, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[1].allert == true) tft.fillCircle(193, 54, 5, 0x47EA);
			else tft.fillCircle(193, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[2].member == 0) tft.drawCircle(213, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(213, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[2].allert == true) tft.fillCircle(213, 54, 5, 0x47EA);
			else tft.fillCircle(213, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[3].member == 0) tft.drawCircle(233, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(233, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[3].allert == true) tft.fillCircle(233, 54, 5, 0x47EA);
			else tft.fillCircle(233, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[4].member == 0) tft.drawCircle(253, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(253, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[4].allert == true) tft.fillCircle(253, 54, 5, 0x47EA);
			else tft.fillCircle(253, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[5].member == 0) tft.drawCircle(273, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(273, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[5].allert == true) tft.fillCircle(273, 54, 5, 0x47EA);
			else tft.fillCircle(273, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[6].member == 0) tft.drawCircle(293, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(293, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[6].allert == true) tft.fillCircle(293, 54, 5, 0x47EA);
			else tft.fillCircle(293, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[7].member == 0) tft.drawCircle(313, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(313, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[7].allert == true) tft.fillCircle(313, 54, 5, 0x47EA);
			else tft.fillCircle(313, 54, 5, ILI9341_BLACK);
		}
	}
	else if (processMode.allow == 3) {
		// в процессе затирания выводим время и температуру паузы

		if (processMashing[0].step == 1) {
			tempOutTFT = processMashing[0].temperature;
			if ((processMashing[0].time * 60) >= processMode.timeStep) timeOutTFT = (processMashing[0].time * 60) - processMode.timeStep;
			else timeOutTFT = 0;
		}
		else if (processMashing[1].step == 1) {
			tempOutTFT = processMashing[1].temperature;
			if ((processMashing[1].time * 60) >= processMode.timeStep) timeOutTFT = (processMashing[1].time * 60) - processMode.timeStep;
			else timeOutTFT = 0;
		}
		else if (processMashing[2].step == 1) {
			tempOutTFT = processMashing[2].temperature;
			if ((processMashing[2].time * 60) >= processMode.timeStep) timeOutTFT = (processMashing[2].time * 60) - processMode.timeStep;
			else timeOutTFT = 0;
		}
		else if (processMashing[3].step == 1) {
			tempOutTFT = processMashing[3].temperature;
			if ((processMashing[3].time * 60) >= processMode.timeStep) timeOutTFT = (processMashing[3].time * 60) - processMode.timeStep;
			else timeOutTFT = 0;
		}
		else if (processMashing[4].step == 1) {
			tempOutTFT = processMashing[4].temperature;
			if ((processMashing[4].time * 60) >= processMode.timeStep) timeOutTFT = (processMashing[4].time * 60) - processMode.timeStep;
			else timeOutTFT = 0;
		}

		// время
		tft.setTextSize(2);
		tft.setCursor(0, 47);
		tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
		if (processMode.step < 9) {
			if (timeOutTFT == 0) tft.print(utf8rus(" Пауза    "));
			else if (processMode.timeStep <= 1) tft.printf("> %.1f min. = ", timeOutTFT / 60);
			else tft.printf(" %.1f min. = ", timeOutTFT / 60);
			// температура
			tft.printf("%.1f C    ", tempOutTFT);
		}
		else tft.print(utf8rus(" Процесс завершен       "));
	}

	switch (tempBigOut) {
		case 1: temp_convert = (int)(temperatureSensor[DS_Cube].data * 10); grColor = dallas_graph[0].color; break;
		case 2: temp_convert = (int)(temperatureSensor[DS_Tube].data * 10); grColor = dallas_graph[1].color; break;
		case 3: temp_convert = (int)(temperatureSensor[DS_Out].data * 10); grColor = dallas_graph[2].color; break;
		case 4: temp_convert = (int)(temperatureSensor[DS_Def].data * 10); grColor = dallas_graph[3].color; break;
	}
	if (temp_in_old != temp_convert || tempBigOutOld != tempBigOut) {
		tempBigOutOld = tempBigOut;
		drawBigTemp(88, 2, temp_convert, grColor, ILI9341_BLACK);
	}

	// вывод времени и давления
	tft.setTextSize(2);
	tft.setCursor(223, 2);
	tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
	tft.print(time_ntp);
	tft.setCursor(236, 22);
	tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK); // голубой
	tft.printf("%d mm", (int)pressureSensor.data);
}

void tftOutGraphDisplay() {
	int count;

	csOn(TFT_CS);

	if (graphOutInterval >= Display_out_temp) {
		graphOutInterval = 1;
		temp_min = 1000;
		temp_max = 0;
		// сдвиг массива выводимых данных
		for (count = 0; count < 311; count++) {
			dallas_graph[0].temp_in_tft[count] = dallas_graph[0].temp_in_tft[count + 1];
			dallas_graph[1].temp_in_tft[count] = dallas_graph[1].temp_in_tft[count + 1];
			dallas_graph[2].temp_in_tft[count] = dallas_graph[2].temp_in_tft[count + 1];
			dallas_graph[3].temp_in_tft[count] = dallas_graph[3].temp_in_tft[count + 1];
			if (processMode.allow == 2) {
				if (dallas_graph[1].temp_in_tft[count] < temp_min) temp_min = dallas_graph[1].temp_in_tft[count];
				if (dallas_graph[1].temp_in_tft[count] > temp_max) temp_max = dallas_graph[1].temp_in_tft[count];
			}
		}
		dallas_graph[0].temp_in_tft[311] = (int)(temperatureSensor[DS_Cube].data * 10.0);
		dallas_graph[1].temp_in_tft[311] = (int)(temperatureSensor[DS_Tube].data * 10.0);
		dallas_graph[2].temp_in_tft[311] = (int)(temperatureSensor[DS_Out].data * 10.0);
		dallas_graph[3].temp_in_tft[311] = (int)(temperatureSensor[DS_Def].data * 10.0);
		if (processMode.allow == 2) {
			if (dallas_graph[1].temp_in_tft[311] < temp_min) temp_min = dallas_graph[1].temp_in_tft[311];
			if (dallas_graph[1].temp_in_tft[311] > temp_max) temp_max = dallas_graph[1].temp_in_tft[311];
		}

		for (count = 309; count > 1; count--) {
			// стираем старый график
			if (temperatureSensor[DS_Cube].member != 0) dravGraph(0, count - 1, 3, ILI9341_BLACK);
			if (temperatureSensor[DS_Tube].member != 0) dravGraph(1, count - 1, 3, ILI9341_BLACK);
			if (temperatureSensor[DS_Out].member != 0) dravGraph(2, count - 1, 3, ILI9341_BLACK);
			if (temperatureSensor[DS_Def].member != 0) dravGraph(3, count - 1, 3, ILI9341_BLACK);
			// выводим сетку
			if ((count + 2) % 10 == 0) {
				for (int i = UP_Y_axis; i < DW_Y_axis; i += 10) {
					if ((count + 2) > 57 || processMode.allow != 2) tft.drawPixel(count + 2, i, ILI9341_DARKDARK);
					else if (i < (DW_Y_axis - 22) && i >(UP_Y_axis + 20)) tft.drawPixel(count + 2, i, ILI9341_DARKDARK);
				}
			}
			// выводим новый график
			if (temperatureSensor[DS_Cube].member != 0) dravGraph(0, count, 2, dallas_graph[0].color);
			if (temperatureSensor[DS_Tube].member != 0) dravGraph(1, count, 2, dallas_graph[1].color);
			if (temperatureSensor[DS_Out].member != 0) dravGraph(2, count, 2, dallas_graph[2].color);
			if (temperatureSensor[DS_Def].member != 0) dravGraph(3, count, 2, dallas_graph[3].color);
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
	}
	else {
		graphOutInterval++;
	}
	// вывод цифровых значений
	tftOutText(temp_min, temp_max);
	tft.fillCircle(7, 236, 3, ILI9341_BLACK);
	csOff(TFT_CS);
	if (processMode.allow == 2)	temp_in_old = (int)(temperatureSensor[DS_Tube].data * 10);
	else temp_in_old = (int)(temperatureSensor[DS_Cube].data * 10);
}

// Вывод первоначального меню
void drawMenuScreen() {
	String ipAddr;
	int lengthAddr;
	csOn(TFT_CS);
	fillScreenRect(0, 21, 320, 220, ILI9341_WHITE);
	fillScreenRect(0, 0, 320, 21, ILI9341_BLACK);
	// Вывод IP адреса
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_CYAN);
	if (WiFi.status() != WL_CONNECTED) ipAddr = "192.168.4.1";
	else ipAddr = WiFi.localIP().toString().c_str();
	lengthAddr = (319 - (sizeof(ipAddr) * 12)) >> 1;
	tft.setCursor(lengthAddr, 0);
	tft.print(ipAddr);
	// Разделение кнопок
	tft.drawLine(159, 20, 159, 239, ILI9341_BLACK);
	tft.drawLine(160, 20, 160, 239, ILI9341_BLACK);
	tft.drawLine(0, 129, 319, 129, ILI9341_BLACK);
	tft.drawLine(0, 130, 319, 130, ILI9341_BLACK);
// Картинки на кнопки
  // Меню дистилляции
	drawScreen4bitMonoBitmap(30, 25, 100, 100, imageDataDistillation);
  // Меню ректификации
	drawScreen4bitMonoBitmap(190, 25, 100, 100, imageDataRectification);
  // Меню затирания
	drawScreen4bitMonoBitmap(30, 135, 100, 100, imageDataBrewing);
  // Меню настроек
	drawScreen4bitMonoBitmap(190, 135, 100, 100, imageDataSetting);

	csOff(TFT_CS);
	processMode.step = 1;
}

// Вывод менюшки на подтверждение остановки процесса
void tftStopLoop() {
	// вывод менюшки Да Нет
	if (touchScreen == 1) {
		csOn(TFT_CS);
		fillScreenRect(25, 65, 270, 140, 0xFFF6);
		tft.drawRect(27, 67, 266, 136, 0xFC00);
		tft.drawRect(28, 68, 264, 134, 0xFC00);
		fillScreenRect(45, 125, 100, 60, 0xFB6D);
		tft.drawRect(45, 125, 100, 60, 0x0000);
		fillScreenRect(175, 125, 100, 60, 0x67EC);
		tft.drawRect(175, 125, 100, 60, 0x0000);

		drawBitmapString(80, 78, &Exit, ILI9341_RED, 0xFFF6);
		drawBitmapString(60, 141, &Esc, ILI9341_BLACK, 0xFB6D);
		drawBitmapString(196, 140, &Ok, ILI9341_BLACK, 0x67EC);

		csOff(TFT_CS);
		touchScreen = 2;
		timeOffMenu = millis() + 10000;
	}
	//Serial.println("menu");
	else if (touchScreen == 2) {
		// ждем нажатие
		if (touchArea == 21 || timeOffMenu <= millis()) { // Нет - перерисовываем графики
			graphOutInterval = Display_out_temp;
			touchArea = 0;
			touchScreen = 0;
			csOn(TFT_CS);
			if (processMode.allow < 6) fillScreenRect(25, 65, 270, 140, ILI9341_BLACK);
			csOff(TFT_CS);
			if (processMode.allow == 6) processMode.step = 0;
		}
		else if (touchArea == 22) { // Да - останавливаем процесс и выходим в меню
			if (processMode.allow == 6) {
				CH1 = false;
				CH2 = false;
				CH3 = false;
				CH4 = false;
			}
			if (processMode.allow < 4) {
				commandWriteSD = "TouchSend: Стоп";
				commandSD_en = true;
			}
			processMode.step = 0;
			processMode.allow = 0;
			touchArea = 0;
			touchScreen = 0;
			// Закрыли отбор по пару
			//setPWM(PWM_CH5, 0, 10);
			//settingAlarm = false;
			//csOff(PWM_CH1);
			//csOff(PWM_CH2);
			//csOff(PWM_CH3);
			//csOff(PWM_CH4);
		}
		yield();
	}
}

void getTouchArea() {
	if (touchArea == 1) {
		//touchStart = true;
		commandWriteSD = "TouchSend: Старт";
		commandSD_en = true;
		processMode.allow = 1;
		processMode.step = 0;
	}
	else if (touchArea == 2) {
		//touchStart = true;
		commandWriteSD = "TouchSend: Старт";
		commandSD_en = true;
		processMode.allow = 2;
		processMode.step = 0;
	}
	else if (touchArea == 3) {
		//touchStart = true;
		commandWriteSD = "TouchSend: Старт";
		commandSD_en = true;
		processMode.allow = 3;
		processMode.step = 0;
	}
	else if (touchArea == 4) {
		processMode.allow = 6;
		processMode.step = 0;
	}
	touchArea = 0;
}

void tftMenuLoop() {
	if (processMode.allow == 0) {
		settingAlarm = false;
		power.heaterStatus = 0;
		power.heaterPower = 0;
		processMode.timeStart = 0;
		processMode.timeStep = 0;
		csOff(PWM_CH1);		// закрыли все клапана
		csOff(PWM_CH2);
		csOff(PWM_CH3);
		csOff(PWM_CH4);
		csOff(PWM_CH6);		// выключить дополнительный ТЭН на разгон
		// Закрыли отбор по пару
		setPWM(PWM_CH5, 0, 10);
		// Для затирания
		processMashing[0].step = 0;
		processMashing[1].step = 0;
		processMashing[2].step = 0;
		processMashing[3].step = 0;
		processMashing[4].step = 0;
	}
#if defined TFT_Display
	// processMode.num = 0 вывод экрана, processMode.num = 1 ждем нажатия
	switch (processMode.step) {
		case 0: drawMenuScreen(); break;
		case 1: getTouchArea(); break;
	}
#endif
}
