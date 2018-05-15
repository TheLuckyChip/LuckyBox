// 
// 
// 
#include "touch_interrupt.h"
#include "display.h"
#include "sensors.h"
#include "tft.h"
#include "wifi_config.h"
#include "setting.h"
#include "user_config.h"


void displayLoop() {
	// опрос тачскрина
#if defined TFT_Display
	if ((millis() - touchTimeRead) >= 250) {
		touchTimeRead = millis();
		touchscreenUpdate();
	}
#endif
	// Вывод на экран
	if ((millis() - displayTimeInterval) >= 1000) {
		displayTimeInterval = millis();
#if defined TFT_Display
		tftOutGraphDisplay(); // вывод на дисплей, если он есть
		DefCubOut++;
#endif
		// проверка WiFi
		if ((millis() - displayTimeInterval) >= (Display_out_temp * 1000)) {
			displayTimeInterval = millis();

			reconnectWiFi(timeWiFiReconnect);
			if (timeWiFiReconnect < setRestartWiFi) timeWiFiReconnect++;
			else timeWiFiReconnect = 0;
		}
	}
}