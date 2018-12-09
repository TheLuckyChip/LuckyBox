#include "display.h"

byte touchRead = 0;

void displayLoop() {
	// опрос тачскрина
#if defined TFT_Display
	if (touch_in == true) {
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
				}
				// выбор клапана 2
				else if (touch_x > 80 && touch_x < 160 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 62;
				}
				// выбор клапана 3
				else if (touch_x > 160 && touch_x < 240 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 63;
				}
				// выбор клапана 4
				else if (touch_x > 240 && touch_y >= 140 && processMode.allow == 6) {
					touchArea = 64;
				}
				else touchArea = 0;
			}
			else if (processMode.allow > 0 && touchScreen == 2) {
				if (touch_x > 10 && touch_x < 160 && touch_y > 100) touchArea = 21;
				else if (touch_x > 160 && touch_x < 320 && touch_y > 100) touchArea = 22;
				else touchArea = 0;
			}
			delay(50);
		}

		if (digitalRead(intTouch) == 1) {
			delay(25);
			touchRead = 0;
			touch_in = false;
			attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
		}
		
	}

	if (touchArea == 10 || touchScreen != 0) {
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
	// проверка WiFi
	/*if ((millis() - wifiTimeInterval) >= (setRestartWiFi * 1000)) {
		wifiTimeInterval = millis();

		reconnectWiFi();

	}*/
}
