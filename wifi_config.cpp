#include "wifi_config.h"

void initWifi() {
	WiFi.disconnect();
	wifi_station_set_hostname("LuckyBox");
	WiFi.hostname("LuckyBox");
	IPAddress apIP(192, 168, 4, 1);
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	WiFi.softAP(_ssidAPconnect.c_str(), _passwordAP.c_str());
	modeWiFi = 0;
	int n_network = WiFi.scanNetworks(); // запрос количества доступных сетей
	for (int i = 0; i < n_network; ++i) {

#if defined Debug_en
		Serial.print(i + 1);
		Serial.print(" -> ");
		Serial.print(WiFi.SSID(i));
		Serial.print(" (");
		Serial.print(WiFi.RSSI(i));
		Serial.println(")  ");
#endif

		if (WiFi.SSID(i) == _ssid.c_str()) modeWiFi = 1; // наша сеть присутствует
	}

	if (modeWiFi == 1) {
	    // пробуем подключиться
		Serial.printf("Connecting to %s\n", _ssid.c_str());
		WiFi.disconnect(true);
		WiFi.mode(WIFI_AP_STA);
		WiFi.begin(_ssid.c_str(), _password.c_str());
		// ждем N кол-во попыток, если нет, то AP Mode
		byte tmp_while = 0;
		while (WiFi.status() != WL_CONNECTED) {
			delay(1000);
			Serial.print(".");
			if (tmp_while < 20) tmp_while++;
			else {
				modeWiFi = 0;
				break;
			}
#if defined TFT_Display
			if (tmp_while % 2 == 0) {
				// рисуем квадратики для индикации загрузки
				scaleCount += 20;
				if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
			}
#endif
		}
		if (WiFi.status() == WL_CONNECTED) WiFi.setAutoReconnect(true);
		else {
			WiFi.disconnect(true);
			WiFi.mode(WIFI_AP);
			WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
			WiFi.softAP(_ssidAPconnect.c_str(), _passwordAP.c_str());
		}
	}
	else {
		WiFi.disconnect(true);
		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
		WiFi.softAP(_ssidAPconnect.c_str(), _passwordAP.c_str());
	}
}
