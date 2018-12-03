#include "setup.h"

void setup()
{
	// Настройка вывода для ТЭНа
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
	power.heaterStatus = 0;
	power.heaterPower = 0;
	power.inPowerHigh = 100;
	power.inPowerLow = 65;

	delay(200);

	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	Serial.println("Start Setup");

	String addrMac = WiFi.softAPmacAddress();
	addrMacMod = "            ";
	addrMacMod[0] = addrMac[0];	addrMacMod[1] = addrMac[1];
	addrMacMod[2] = addrMac[3];	addrMacMod[3] = addrMac[4];
	addrMacMod[4] = addrMac[6];	addrMacMod[5] = addrMac[7];
	addrMacMod[6] = addrMac[9];	addrMacMod[7] = addrMac[10];
	addrMacMod[8] = addrMac[12]; addrMacMod[9] = addrMac[13];
	addrMacMod[10] = addrMac[15]; addrMacMod[11] = addrMac[16];

	Wire.setClock(400000);
	Wire.begin(pSDA, pSCL);
	delay(100);

	// Установим CS для всех SPI устройств
	pwm = Adafruit_PWMServoDriver();
	pwm.begin();
	pwm.setPWMFreq(1000);
	csOff(TFT_RES_PRG);
	csOff(SIREN_OUT);
	csOff(SD_CS);
	csOff(TOUCH_CS);
	setPWM(BUZ_VOL, 0, 650); //csOff(BUZ_VOL);
	csOff(BUZ_OUT);
	pwmOut[0].invert = PWM_CH1_Invert;
	pwmOut[1].invert = PWM_CH2_Invert;
	pwmOut[2].invert = PWM_CH3_Invert;
	pwmOut[3].invert = PWM_CH4_Invert;
	pwmOut[4].invert = PWM_CH5_Invert;
	pwmOut[5].invert = PWM_CH6_Invert;
	pwmOut[6].invert = PWM_CH7_Invert;
	pwmOut[7].invert = PWM_CH8_Invert;
	pwmOut[8].invert = PWM_CH9_Invert;
	csOff(PWM_CH1);
	csOff(PWM_CH2);
	csOff(PWM_CH3);
	csOff(PWM_CH4);
	csOff(PWM_CH5);
	csOff(PWM_CH6);
	csOff(PWM_CH7);
	csOff(PWM_CH8);
	csOff(PWM_CH9);
	delay(2);
	csOn(TFT_RES_PRG);
	delay(10);
	csOff(TFT_RES_PRG);
	delay(2);

	EEPROM.begin(2048);
	// 0 - датчики и устройства (1298 & 1299 переворот экрана и тачскрина)
	// 1300 - сохраненные данные для процесса дистилляции
	// 1400 - сохраненные данные для процесса ректификации (1497 HiPower, 1498 LoPower, 1499 № процесса по умолчанию)
	// 1500 - сохраненные данные для процесса затирания
	// 1600 - сохраненные данные PID установок
	// 1700 - 1759 имя ssdp, 1760 - 1819 имя ssid, 1820 - 1879 имя ssidAP
	// 1900 - 1931 пароль ssid, 1940 - 1971 пароль ssidAP, 1980 - часовой пояс
	timezone = EEPROM.read(1980);
	if (timezone > 23) timezone = 3;
	// Считаем инверсию экрана и тачскрина
	uint8_t tft180 = EEPROM.read(1298);
	uint8_t touch180 = EEPROM.read(1299);
	if (tft180 > 1 || tft180 == 0) tftInvert = false;
	else tftInvert = true;
	if (touch180 > 1 || touch180 == 0) touchInvert = false;
	else touchInvert = true;

#if defined TFT_Display
	csOn(TFT_CS);
	initTFT();	    // HSPI & TFT

	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t yLine = 319;
	byte arrayShift = 0;
	byte yShift = 0;
	uint8_t data_mem, data_rect;
	uint16_t color, tmp_color;
	// рисуем две встречные полоски шириной по 20 пикселей
	for(uint16_t i1 = 0; i1 < 320; i1++) {
		tft.drawLine(i1, 15, i1, 34, 0xFFFF);
		tft.drawLine(yLine, 185, yLine, 204, 0xFFFF);
		yLine--;
		delayMicroseconds(100);// delay(1);
	}

	// выводим наш логотип
	tft.setAddrWindow(x, y + 35, 320, 1);
	for (uint16_t i2 = 0; i2 < 24000; i2++) {
		// Выводим с одного байта 2 точки
		data_mem = pgm_read_byte_near(imageDataStartScreen + (y * 160 + arrayShift));
		data_rect = data_mem >> 4;
		if (data_rect == 0x0F) color = 0xFFFF;
		else {
		  color = (data_rect * 2) << 11;
		  tmp_color = (data_rect * 4) << 5;
		  color += (data_rect * 2 + tmp_color);
		}
		//tft.drawPixel(x, y + 35, color);
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color >> 8;
		SPI1CMD |= SPIBUSY;
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color;
		SPI1CMD |= SPIBUSY;

		x++;
		data_rect = data_mem & ~0xF0;
		if (data_rect == 0x0F) color = 0xFFFF;
		else {
		  color = (data_rect * 2) << 11;
		  tmp_color = (data_rect * 4) << 5;
		  color += (data_rect * 2 + tmp_color);
		}
		//tft.drawPixel(x, y + 35, color);
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color >> 8;
		SPI1CMD |= SPIBUSY;
		while (SPI1CMD & SPIBUSY) {}
		SPI1W0 = color;
		SPI1CMD |= SPIBUSY;

		x++;

		if (x == 320) {
			x = 0;
			y += 10;
			if (y >= 150) {
				yShift++;
				y = yShift;
			}
			tft.setAddrWindow(x, y + 35, 320, 1);
		}
		if (arrayShift < 159) arrayShift++;
		else arrayShift = 0;
	}
	while (SPI1CMD & SPIBUSY) {}
	SPI.endTransaction();

	// рисуем квадратики для индикации загрузки
	scaleCount = 2;
	tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
	delay(150);
#endif

	csOff(TFT_CS);
	// инициализация SD карты
	sdInit();
	csOn(TFT_CS);

	// инициализация I2C
	Serial.println("Step 1 - I2C Init");

	// Инициализация EEPROM и датчиков температуры
	Serial.println("Step 2 - DS Init");
	senseWebInit();
	dallSearch();

	//Запускаем файловую систему
	Serial.println("Step 3 - FS Init");
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	initFS();
	Serial.println();
	Serial.println("Step 4 - Load WiFi settings");
	loadConfig();
	Serial.println("Step 5 - WIFI Init");
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	//Запускаем WIFI
	_ssidAPconnect = _ssidAP;
	_ssidAPconnect += " - ";
	_ssidAPconnect += addrMacMod;
	initWifi();

	// просто отчет о подключении
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Not'Connected STA! WiFi up AP.");
		Serial.println("");
	}
	else {
		// Иначе удалось подключиться отправляем сообщение
		// о подключении и выводим адрес IP
		IPAddress Local_IP = WiFi.localIP();
		Serial.println("");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(Local_IP);
	}

	Serial.println("Step 6 - Time, NTP Init");
	// Получаем время из сети
	initTime();
	//Настраиваем и запускаем SSDP интерфейс
	if (WiFi.status() == WL_CONNECTED)
	{
		//Удалось подключиться - запускаем SSDP
		Serial.println("Step 7  - SSDP Init");
#if defined TFT_Display
		// рисуем квадратики для индикации загрузки
		scaleCount += 20;
		if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
		initSSDP();
	}

	//Настраиваем и запускаем HTTP интерфейс
	Serial.println("Step 8  - WebServer Start");
	initHTTP();
	Serial.println("Step 9  - Reflux Init");
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	initReflux();
	Serial.println("Step 10 - Distillation Init");
	initDistillation();

	initPID();

	Serial.println("Step 11 - Mashing Init");
	initMashing();
	Serial.println("Step 12 - Brewing Init");
	initBrewing();
	Serial.println("Step 13 - Heater Init");
	initHeater();
	Serial.println("Step 14 - Pressure sensor Init");
	pressureSensor.status = 0;
	pressureSensor.data = 760.0;
	initPressureSensor();
	Serial.println("Step 15 - Buzzer Init");

	Serial.println("Step 16 - Variables Init");

	dallRead();
	delay(750);
	dallRead();
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	delay(750);
	dallRead();
	delay(750);
	dallRead();
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	Serial.println("Temperature sensors:");
	if (DS_Count == 0) Serial.println("Not present...");
	if (DS_Count >= 1) Serial.println(temperatureSensor[DS_Cube].data);
	if (DS_Count >= 2) Serial.println(temperatureSensor[DS_Tube].data);
	if (DS_Count >= 3) Serial.println(temperatureSensor[DS_Out].data);
	if (DS_Count >= 4) Serial.println(temperatureSensor[DS_Def].data);
	if (DS_Count >= 5) Serial.println(temperatureSensor[DS_Res1].data);
	if (DS_Count >= 6) Serial.println(temperatureSensor[DS_Res2].data);
	if (DS_Count >= 7) Serial.println(temperatureSensor[DS_Res3].data);
	if (DS_Count >= 8) Serial.println(temperatureSensor[DS_Res4].data);

	Serial.println("Pressure sensor:");
	if (pressureSensor.status) pressureRead();
	Serial.println(pressureSensor.data);

  // инициализация АЦП
  adcInit();
  #if defined TFT_Display
  // рисуем квадратики для индикации загрузки
  scaleCount += 20;
  if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);

  // рисуем квадратики для индикации загрузки
  while (1) {
	  scaleCount += 20;
	  if (scaleCount > 302) break;
	  tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
	  delay(150);
  }
  csOff(TFT_CS);
  pinMode(intTouch, INPUT); // прерывание от тачскрина
  attachInterrupt(intTouch, touchscreenUpdateSet, FALLING);
#endif

  processMode.allow = 0; // Стоп
  processMode.number = EEPROM.read(1499);// modeReflux;
  EEPROM.end();
  if (processMode.number > 7) processMode.number = 0;
  processMode.step = 0;
  loadEepromPid();
  setKp = Kp;
  setKi = Ki;
  setKd = Kd;
  processMashing[0].time = 20; processMashing[0].temperature = 45;
  processMashing[1].time = 30; processMashing[1].temperature = 55;
  processMashing[2].time = 120; processMashing[2].temperature = 65;
  processMashing[3].time = 10; processMashing[3].temperature = 72;

  Serial.println("Setup Done!");

  //Serial.println(ESP.getFreeSketchSpace());
}
