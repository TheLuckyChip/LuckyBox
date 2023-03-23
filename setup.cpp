#include "setup.h"

void setup()
{
	//String payload;
	//int ver_fw = versionForWeb;
	//bool fw_update = false;

	// Настройка вывода для ТЭНа
	pinMode(heater, OUTPUT);
	digitalWrite(heater, LOW);
	power.heaterStatus = 0;
	power.heaterPower = 0;
	power.inPowerHigh = 100;
	power.inPowerLow = 65;

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
	// Закрыли отбор по пару
	setPWM(PWM_CH5, 0, 10);
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
	//csOff(PWM_CH5);
	csOff(PWM_CH6);
	csOff(PWM_CH7);
	//csOn(PWM_CH8);
	csOff(PWM_CH9);
	setPWM(PWM_CH8, 0, 2047);
	delay(2);
	csOn(TFT_RES_PRG);
	delay(10);
	csOff(TFT_RES_PRG);
	delay(2);

#if defined language_RUS
	RU = true;
#else
	RU = false;
#endif
#if defined Debug_en
	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	Serial.println("Start Setup");
#endif
	EEPROM.begin(2048);
	StateDsReset = EEPROM.read(0);
	// 0 - датчики и устройства (1298 & 1299 переворот экрана и тачскрина)
	// 1300 - сохраненные данные для процесса дистилляции
	// 1400 - сохраненные данные для процесса ректификации (1497 HiPower, 1498 LoPower, 1499 № процесса по умолчанию)
	// 1500 - сохраненные данные для процесса затирания
	// 1600 - сохраненные данные PID установок
	// 1700 - 1759 имя ssdp, 1760 - 1819 имя ssid, 1820 - 1879 имя ssidAP
	// 1900 - 1931 пароль ssid, 1940 - 1971 пароль ssidAP, 1980 - часовой пояс, 1981 громкость пищалки, 1982 тип БП
	powerType = EEPROM.read(1982);
	if (powerType < 1 || powerType > 2) powerType = 1;
	BuzzerVolumeLevel = EEPROM.read(1981);
	if (BuzzerVolumeLevel > 100) BuzzerVolumeLevel = 4000;
	else BuzzerVolumeLevel *= 40;
	timezone = EEPROM.read(1980);
	if (timezone < -12 || timezone > 12) timezone = 3;
	// Считаем инверсию экрана и тачскрина
	uint8_t tft180 = EEPROM.read(1298);
	uint8_t touch180 = EEPROM.read(1299);
	if (tft180 > 1 || tft180 == 0) tftInvert = false;
	else tftInvert = true;
	//if (touch180 > 1 || touch180 == 0) touchInvert = false;
	if (touch180 == 0) touchInvert = false;
	else if (touch180 == 1) touchInvert = true;
	else {
		touchInvert = true;
		EEPROM.write(1299, 1);
	}

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
#if defined Debug_en
	// инициализация I2C
	Serial.println("Step 1 - I2C Init");

	// Инициализация EEPROM и датчиков температуры
	Serial.println("Step 2 - DS Init");
#endif
	senseWebInit();
	dallSearch();
#if defined Debug_en
	//Запускаем файловую систему
	Serial.println("Step 3 - FS Init");
#endif
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	initFS();
	loadConfig();
#if defined Debug_en
	Serial.println();
	Serial.println("Step 4 - Load WiFi settings");
	Serial.println("Step 5 - WIFI Init");
#endif
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
#if defined Debug_en
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
#endif
	// Получаем время из сети
	initTime();
	//Настраиваем и запускаем SSDP интерфейс
	if (WiFi.status() == WL_CONNECTED)
	{
		//Удалось подключиться - запускаем SSDP
#if defined Debug_en
		Serial.println("Step 7  - SSDP Init");
#endif
#if defined TFT_Display
		// рисуем квадратики для индикации загрузки
		scaleCount += 20;
		if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
//		initSSDP();
	}

	//Настраиваем и запускаем HTTP интерфейс
#if defined Debug_en
	Serial.println("Step 8  - WebServer Start");
	Serial.println("Step 9  - Reflux Init");
#endif
	initHTTP();
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	initReflux();
#if defined Debug_en
	Serial.println("Step 10 - Distillation Init");
#endif
	initDistillation();
	initPID();
#if defined Debug_en
	Serial.println("Step 11 - Mashing Init");
#endif
	initMashing();
#if defined Debug_en
	Serial.println("Step 12 - Brewing Init");
#endif
	initBrewing();
#if defined Debug_en
	Serial.println("Step 13 - Heater Init");
#endif
	initHeater();
#if defined Debug_en
	Serial.println("Step 14 - Pressure sensor Init");
#endif
	pressureSensor.status = 0;
	pressureSensor.data = 760.0;
	initPressureSensor();
#if defined Debug_en
	Serial.println("Step 15 - Buzzer Init");
	Serial.println("Step 16 - Variables Init");
#endif
	dallRead(10);
	delay(750);
	dallRead(10);
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
	delay(750);
	dallRead(10);
	delay(750);
	dallRead(10);
#if defined TFT_Display
	// рисуем квадратики для индикации загрузки
	scaleCount += 20;
	if (scaleCount <= 282) tft.writeFillRect(scaleCount, 215, 15, 15, 0xFFFF);
#endif
#if defined Debug_en
	Serial.println("Temperature sensors:");
	if (DS_Count == 0) Serial.println("Not present...");
	if (DS_Count >= 1) {
		if (temperatureSensor[0].data != 150) Serial.println(temperatureSensor[0].data);
		if (temperatureSensor[1].data != 150) Serial.println(temperatureSensor[1].data);
		if (temperatureSensor[2].data != 150) Serial.println(temperatureSensor[2].data);
		if (temperatureSensor[3].data != 150) Serial.println(temperatureSensor[3].data);
		if (temperatureSensor[4].data != 150) Serial.println(temperatureSensor[4].data);
		if (temperatureSensor[5].data != 150) Serial.println(temperatureSensor[5].data);
		if (temperatureSensor[6].data != 150) Serial.println(temperatureSensor[6].data);
		if (temperatureSensor[7].data != 150) Serial.println(temperatureSensor[7].data);
	}
	else Serial.println("--.-");
	Serial.println("Pressure sensor:");
#endif
	if (pressureSensor.status) pressureRead();
#if defined Debug_en
	Serial.println(pressureSensor.data);
#endif
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
  loadEepromReflux();
  deltaBoilTube = temperatureSensor[DS_Tube].allertValueIn;
  loadEepromPid();
  setKp = Kp;
  setKi = Ki;
  setKd = Kd;
  processMashing[0].time = 20; processMashing[0].temperature = 40;
  processMashing[1].time = 20; processMashing[1].temperature = 55;
  processMashing[2].time = 30; processMashing[2].temperature = 63;
  processMashing[3].time = 60; processMashing[3].temperature = 67;
  processMashing[4].time = 10; processMashing[4].temperature = 78;
  numSenseMashBrew = DS_Cube;

  TX_BUF_IO_Power[0] = 0x41;		// A
  TX_BUF_IO_Power[1] = 0x54;		// T
  TX_BUF_IO_Power[2] = 0x2B;		// +
  TX_BUF_IO_Power[3] = 0x70;		// p
  TX_BUF_IO_Power[4] = 0x3D;		// =
#if defined Debug_en
  Serial.println();
#endif
  touchRead = 1;
  touchArea = 0;
  touchScreen = 0;
#if defined Debug_en
  Serial.println();
  Serial.println("Setup Done!");

  Serial.end();
#endif
  delay(100);
  Serial.begin(9600);
#ifdef power_PMBK
  uint8_t TX_BUF[8] = {0x41, 0x54, 0x2B, 0x4F, 0x4E, 0x3D, 0x30, 0x0D};
  Serial.write(TX_BUF, 8);
#endif
}
