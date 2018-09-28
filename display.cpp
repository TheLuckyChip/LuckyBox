#include "display.h"

byte touchRead = 0;

void displayLoop() {
	// опрос тачскрина
#if defined TFT_Display
	if (touch_in == true) {
		if (touchRead == 0) {
			initBuzzer(50);
			delay(15);
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
				if (touch_x > 110 && touch_x < 260 && touch_y < 100) {
					touchArea = 10;
					touchScreen = 1;
				}
				else touchArea = 0;
			}
			else if (processMode.allow > 0 && touchScreen == 2) {
				if (touch_x > 10 && touch_x < 160 && touch_y > 100) touchArea = 21;
				else if (touch_x > 160 && touch_x < 320 && touch_y > 100) touchArea = 22;
				else touchArea = 0;
			}
			delay(35);
			//Serial.print("X="); Serial.println(touch_x);
			//Serial.print("Y="); Serial.println(touch_y);
		}

		if (digitalRead(intTouch) == 1) {
			delay(25);
			touchRead = 0;
			touch_in = false;
			attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
		}
		
	}
#endif

	// Вывод на экран
	if ((millis() - displayTimeInterval) >= 1000) {
		displayTimeInterval = millis();
#if defined TFT_Display
		if (processMode.allow != 0 && processMode.step != 0 && touchScreen == 0) tftOutGraphDisplay(); // вывод на дисплей графиков, если он есть
		DefCubOut++;
#endif
	}
	// проверка WiFi
	/*if ((millis() - wifiTimeInterval) >= (setRestartWiFi * 1000)) {
		wifiTimeInterval = millis();

		reconnectWiFi();

	}*/
}
