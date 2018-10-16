#include "http_config.h"

byte startLoad = 0;
unsigned long timeStartLoad;

void initHTTP(void)
{

	HTTP.on("/configs.json", handleConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
												 // API для устройства
	HTTP.on("/ssdp", handleSetSSDP);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
	HTTP.on("/ssid", handleSetSSID);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
	HTTP.on("/ssidap", handleSetSSIDAP); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
	HTTP.on("/rotate", handleSetRotate); // Переворот экрана и тачскрина
	HTTP.on("/restart", handleRestart);   // Перезагрузка модуля по запросу вида /restart?device=ok

	// Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
	HTTP.on("/update", HTTP_POST, []() {
		HTTP.sendHeader("Connection", "close");
		HTTP.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
		ESP.restart();
	}, []() {
		HTTPUpload& upload = HTTP.upload();
		if (upload.status == UPLOAD_FILE_START) {
			startLoad = 1;
			Serial.setDebugOutput(true);
			//WiFiUDP::stopAll();
			
			String nameBinFile = upload.filename.c_str();

			Serial.print("Update: "); Serial.println(nameBinFile);

			if (nameBinFile == "LuckyBox.spiffs.bin" || nameBinFile == "LuckyBox.ino.spiffs.bin") {	
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - ESP.getSketchSize());
				Serial.println(maxSketchSpace);
				if (!Update.begin(maxSketchSpace, U_SPIFFS)) { //start with max available size
					Update.printError(Serial);
				}
				else {
					Serial.println("...web files update...");
#if defined TFT_Display
					csOn(TFT_CS);
					fillScreenRect(0, 0, 320, 240, ILI9341_BLACK);
					tft.setCursor(0, 10);
					tft.setTextColor(ILI9341_WHITE);
					tft.setTextSize(2);
					tft.print(" Web files update");
					timeStartLoad = millis() + 500;
					csOff(TFT_CS);
#endif
				}
			}
			else {
				
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				Serial.println(maxSketchSpace);
				if (!Update.begin(maxSketchSpace, U_FLASH)) { //start with max available size
					Update.printError(Serial);
				}
				else {
					Serial.println("...scetch files update...");
#if defined TFT_Display
					csOn(TFT_CS);
					fillScreenRect(0, 0, 320, 240, ILI9341_BLACK);
					tft.setCursor(0, 10);
					tft.setTextColor(ILI9341_WHITE);
					tft.setTextSize(2);
					tft.print(" Sketch files update");
					timeStartLoad = millis() + 500;
					csOff(TFT_CS);
#endif
				}
			}
		}
		else if (upload.status == UPLOAD_FILE_WRITE) {
			if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
				Update.printError(Serial);
			}
		}
		else if (upload.status == UPLOAD_FILE_END) {
#if defined TFT_Display
			csOn(TFT_CS);
			tft.println();
			tft.println();
			tft.println(" Succes!");
			tft.println();
			tft.print(" Restart...");
			timeStartLoad = millis() + 500;
			csOff(TFT_CS);
#endif
			startLoad = 0;
			if (Update.end(true)) { //true to set the size to the current progress
				Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
			}
			else {
				Update.printError(Serial);
			}
			Serial.setDebugOutput(false);
		}

		if (startLoad == 1 && timeStartLoad <= millis()) {
			timeStartLoad = millis() + 500;
#if defined TFT_Display
			csOn(TFT_CS);
			tft.print(".");
			csOff(TFT_CS);
#endif
		}

		yield();
	});
	
	// Запускаем HTTP сервер
	HTTP.begin();
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handleSetSSDP()
{
	SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
	//saveConfig();                 // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
	// сохраним в EEPROM
	uint16_t index = 1700;
	if (SSDP_Name.length() >= 2 && SSDP_Name.length() <= 60) {
		for (uint8_t k = 0; k < 60; k++) { EEPROM.write(index, SSDP_Name[k]); index++; }
		EEPROM.commit();
		delay(100);
	}
}
// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handleSetSSID()
{
	_ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
	_password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
	//saveConfig();                        // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
	// сохраним в EEPROM
	uint16_t index = 1760;
	if (_ssid.length() >= 2 && _ssid.length() <= 60 && _password.length() >= 8 && _password.length() <= 32) {
		for (uint8_t k = 0; k < 60; k++) { EEPROM.write(index, _ssid[k]);  index++; }
		index = 1900;
		for (uint8_t k = 0; k < 32; k++) { EEPROM.write(index, _password[k]);  index++; }
		EEPROM.commit();
		delay(100);
	}
}
//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handleSetSSIDAP()
{              //
	_ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
	_passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
	//saveConfig();                         // Функция сохранения данных во Flash
	HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
	// сохраним в EEPROM
	uint16_t index = 1820;
	if (_ssidAP.length() >= 2 && _ssid.length() <= 60 && _passwordAP.length() >= 8 && _password.length() <= 32) {
		for (uint8_t k = 0; k < 60; k++) { EEPROM.write(index, _ssidAP[k]);  index++; }
		index = 1940;
		for (uint8_t k = 0; k < 32; k++) { EEPROM.write(index, _passwordAP[k]);  index++; }
		EEPROM.commit();
		delay(100);
	}
}

void handleSetRotate() {
	uint8_t tft180, touch180;
	if (processMode.allow == 0) {
		tft180 = HTTP.arg("tft_rotate").toInt();
		touch180 = HTTP.arg("touchpad_rotate").toInt();
		HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
		if (tft180 == 1) {
			if (tftInvert == true) {
				tftInvert = false;
				EEPROM.write(1298, 0);
#if defined TFT_Display
				csOn(TFT_CS);
				tft.setRotation(3);
				csOff(TFT_CS);
#endif
			}
			else {
				tftInvert = true;
				EEPROM.write(1298, 1);
#if defined TFT_Display
				csOn(TFT_CS);
				tft.setRotation(1);
				csOff(TFT_CS);
#endif
			}
		}
		if (touch180 == 1) {
			if (touchInvert == true) {
				touchInvert = false;
				EEPROM.write(1299, 0);
			}
			else {
				touchInvert = true;
				EEPROM.write(1299, 1);
			}
		}
		if (tft180 == 1 || touch180 == 1) {
			EEPROM.commit();
			delay(100);
			processMode.step = 0;
		}
	}
	else HTTP.send(200, "text/plain", "ERR");   // отправляем ответ о выполнении
}

// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handleRestart()
{
	String restart = HTTP.arg("device");          // Получаем значение device из запроса
	if (restart == "ok")
	{                         // Если значение равно Ок
		HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
		ESP.restart();                                // перезагружаем модуль
	}
	else
	{                                        // иначе
		HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
	}
}

void handleConfigJSON()
{
	String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате
						 //{"SSDP":"LuckyBox","ssid":"LuckyBox","password":"12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101" и т.д.}
						 // Резервируем память для json объекта буфер может рости по мере необходимости, предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	//  вызовите парсер JSON через экземпляр jsonBuffer
	JsonObject& json = jsonBuffer.parseObject(root);
	// Заполняем поля json
	json["SSDP"] = SSDP_Name;
	json["ssidAP"] = _ssidAP;
	//json["passwordAP"] = _passwordAP;
	json["ssid"] = _ssid;
	//json["password"] = _password;
	json["timezone"] = timezone;
	//json["ip"] = WiFi.localIP().toString();
	//json["time"] = GetTime();
	//json["date"] = GetDate();
	//json["temperature"] = temperatureSensor[DS_Cube].data;
	//json["temperature2"] = temperatureSensor[DS_Tube].data;
	//json["temperature3"] = temperatureSensor[DS_Out].data;
	//json["temperature4"] = temperatureSensor[DS_Def].data;
	//json["setting"] = settingColumn;
	//json["settingAlarm"] = settingAlarm;


	// Помещаем созданный json в переменную root
	root = "";
	json.printTo(root);
	HTTP.send(200, "text/json", root);
}
