#include "display.h"

void displayLoop() {
	// опрос тачскрина
#if defined TFT_Display
	if (touch_in == true) {
		initBuzzer(50);
		touchscreenUpdate();
		delay(250);
		// определим область нажатия для меню
		if (processMode.allow == 0) {
			if (touch_x > 0 && touch_x < 160 && touch_y > 20 && touch_y < 130) touchArea = 1;
			else if (touch_x > 160 && touch_x < 320 && touch_y > 20 && touch_y < 130) touchArea = 2;
			else if (touch_x > 0 && touch_x < 160 && touch_y > 130 && touch_y < 240) touchArea = 3;
			else if (touch_x > 160 && touch_x < 320 && touch_y > 130 && touch_y < 240) touchArea = 4;
			else touchArea = 0;
		}
		if (processMode.allow > 0) {
			if (touch_x > 110 && touch_x < 260 && touch_y < 60) touchArea = 10;
			else touchArea = 0;
		}

		//if (touch_x > 110 && touch_x < 260 && touch_y < 60) dallSearch(); // переинициализация датчиков температуры
		touch_in = false;
	}
#endif

	// Вывод на экран
	if ((millis() - displayTimeInterval) >= 1000) {
		displayTimeInterval = millis();
#if defined TFT_Display
		if (processMode.allow != 0 && processMode.step != 0) tftOutGraphDisplay(); // вывод на дисплей графиков, если он есть
		DefCubOut++;
#endif
	}
	// проверка WiFi
	if ((millis() - wifiTimeInterval) >= (setRestartWiFi * 1000)) {
		wifiTimeInterval = millis();

		reconnectWiFi();

	}
}
