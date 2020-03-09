#include "display.h"

byte touchRead = 0;
uint8_t cnt_p = 0;
unsigned long displayPowerInterval;

void displayLoop() {
	// опрос тачскрина
#if defined TFT_Display
	if (touch_in == true && stopInfoOutScreen == true) {
		if (touchRead == 0) {
			initBuzzer(50);
			//delay(5);
			touchscreenUpdate();
			touchRead = 1;
			// определим область нажатия для меню
			if (processMode.allow == 0 && touchScreen == 0) {
				if (touch_x > 0 && touch_x < 160 && touch_y > 20 && touch_y < 130) touchArea = 1;
				else if (touch_x > 160 && touch_x < 320 && touch_y > 20 && touch_y < 130) touchArea = 2;
				else if (touch_x > 0 && touch_x < 160 && touch_y > 130 && touch_y < 240) touchArea = 3;
				else if (touch_x > 160 && touch_x < 320 && touch_y > 130 && touch_y < 240) touchArea = 4;
				else touchArea = 0;
			}
			else if (processMode.allow > 0 && touchScreen == 0) {
				// меню подтверждения выхода
				if (touch_x >= 0 && touch_y <= 100) {
					touchArea = 10;
					touchScreen = 1;
				}
				// выбор температуры крупного плана
				else if (touch_x >= 20 && touch_x <= 106 && touch_y >= 140 && processMode.allow < 6) {
					touchArea = 11;
				}
				// выбор температуры крупного плана
				else if (touch_x >= 126 && touch_x <= 212 && touch_y >= 140 && processMode.allow < 6) {
					touchArea = 12;
				}
				// выбор температуры крупного плана
				else if (touch_x >= 232 && touch_y >= 140 && processMode.allow < 6) {
					touchArea = 13;
				}
				// выбор клапана 1
				else if (touch_x < 80 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 61;
					touchScreenDV = 1;
				}
				// выбор клапана 2
				else if (touch_x > 80 && touch_x < 160 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 62;
					touchScreenDV = 1;
				}
				// выбор клапана 3
				else if (touch_x > 160 && touch_x < 240 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 63;
					touchScreenDV = 1;
				}
				// выбор клапана 4
				else if (touch_x > 240 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 64;
					touchScreenDV = 1;
				}
				else touchArea = 0;
			}
			else if ((processMode.allow > 0 && touchScreen == 2) || touchScreen == 3) {
				if (touch_x > 10 && touch_x < 160 && touch_y > 100) touchArea = 21;
				else if (touch_x > 160 && touch_x < 320 && touch_y > 100) touchArea = 22;
				else touchArea = 0;
			}
			delay(50);
		}
		delay(500);
		touchRead = 0;
		touch_in = false;
		attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
	}

	if ((touchArea == 10 || touchScreen != 0) && stopInfoOutScreen == true) {
		tftStopLoop();
	}
	else if (touchArea == 11 && touchScreen == 0) {
		touchArea = 0;
		tempBigOutOld = tempBigOut;
		switch (tempBigOut) {
		case 1: tempBigOut = 2; break;
		case 2: tempBigOut = 1; break;
		case 3: tempBigOut = 1; break;
		case 4: tempBigOut = 1; break;
		}
	}
	else if (touchArea == 12 && touchScreen == 0) {
		touchArea = 0;
		tempBigOutOld = tempBigOut;
		switch (tempBigOut) {
		case 1: tempBigOut = 3; break;
		case 2: tempBigOut = 3; break;
		case 3: tempBigOut = 2; break;
		case 4: tempBigOut = 2; break;
		}
	}
	else if (touchArea == 13 && touchScreen == 0) {
		touchArea = 0;
		tempBigOutOld = tempBigOut;
		switch (tempBigOut) {
		case 1: tempBigOut = 4; break;
		case 2: tempBigOut = 4; break;
		case 3: tempBigOut = 4; break;
		case 4: tempBigOut = 3; break;
		}
	}
#endif

	// Вывод на экран
	if ((millis() - displayTimeInterval) >= 1000) {
		displayTimeInterval = millis();
#if defined TFT_Display
		if (processMode.allow != 0 && processMode.allow != 6 && processMode.step != 0 && touchScreen == 0) tftOutGraphDisplay(); // вывод на дисплей графиков, если он есть
		DefCubOut++;
#endif
	}
#if defined TFT_Display
	// Вывод бегущей строчки мощности и самой мощности, так же клапанов и датчиков
	if ((processMode.allow == 1 || processMode.allow == 2) && millis() > displayPowerInterval) {
		displayPowerInterval = millis() + 250;
		csOn(TFT_CS);

		// клапана и датчики
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
		tft.setCursor(122, 47);
		tft.setTextColor(ILI9341_LIGHTGREY);
		tft.printf(">>");
		if (pwmOut[0].member == 0) tft.drawCircle(160, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(160, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[0].allert == true) tft.fillCircle(160, 54, 5, 0x47EA);
			else tft.fillCircle(160, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[1].member == 0) tft.drawCircle(180, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(180, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[1].allert == true) tft.fillCircle(180, 54, 5, 0x47EA);
			else tft.fillCircle(180, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[2].member == 0) tft.drawCircle(200, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(200, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[2].allert == true) tft.fillCircle(200, 54, 5, 0x47EA);
			else tft.fillCircle(200, 54, 5, ILI9341_BLACK);
		}
		if (pwmOut[3].member == 0) tft.drawCircle(220, 54, 6, ILI9341_DARKDARK);
		else {
			tft.drawCircle(220, 54, 6, ILI9341_LIGHTGREY);
			if (pwmOut[3].allert == true) tft.fillCircle(220, 54, 5, 0x47EA);
			else tft.fillCircle(220, 54, 5, ILI9341_BLACK);
		}

		// мощность
		tft.setTextSize(2);
		tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
		tft.setCursor(235, 46);
		if (power.heaterPower == 100) tft.printf("%d", (int)power.heaterPower);
		else if (power.heaterPower >= 10) tft.printf(" %d", (int)power.heaterPower);
		else tft.printf("  %d", (int)power.heaterPower);
		tft.setCursor(274, 52);
		tft.setTextSize(1);
		tft.print("%");
		tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
		tft.setCursor(283, 46);
		tft.setTextSize(2);
		switch (cnt_p) {
			case 0: tft.print(">  "); cnt_p = 1; break;
			case 1: tft.print(">> "); cnt_p = 2; break;
			case 2: tft.print(" >>"); cnt_p = 3; break;
			case 3: tft.print("  >"); cnt_p = 0; break;
		}
		csOff(TFT_CS);
	}
#endif
}
