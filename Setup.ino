void setup() {

	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	Serial.println("Start Setup");

	Serial.println("Step 1 - DS Init");
	DS_Count = DS_Cnt;
	if (DS_Count > 8) DS_Count = 8;

	// инициализация I2C
	Serial.println("Step 2 - I2C Init");
	Wire.setClock(1000000);
	Wire.begin(pSDA, pSCL);
	delay(100);

	// Инициализация датчиков температуры
	dallSearch();
	dallRead();

#if defined OLED_Display
	// I2C & OLED
	oled_init();
#endif

	//Запускаем файловую систему
	Serial.println("Step 3 - FS Init");
	FS_init();
	Serial.println("Step 4 - File Config");
	loadConfig();
	Serial.println("Step 5 - WIFI Init");
	//Запускаем WIFI
	WIFIinit();
	Serial.println("Step 6 - Time, NTP Init");
	// Получаем время из сети
	Time_init();
	//Настраиваем и запускаем SSDP интерфейс
	if (WiFi.status() != WL_CONNECTED)
	{
		// Если не удалось подключиться, и мы в режиме AP, то SSDP нам не нужен. Ничего не делаем
	}
	else {
		//Удалось подключиться - запускаем SSDP
		Serial.println("Step 7  - SSDP Init");
		SSDP_init();
	}

	//Настраиваем и запускаем HTTP интерфейс
	Serial.println("Step 8  - WebServer Start");
	HTTP_init();
	Serial.println("Step 9  - Reflux Init");
	Reflux_init();
	Serial.println("Step 10 - Distillation Init");
	Destillation_init();
	Serial.println("Step 11 - Brewing Init");
	Brewing_init();
	Serial.println("Step 12 - Heater Init");
	Heater_init();
	Serial.println("Step 13 - Pressure sensor Init");
	Pressure_sensor_init();
	Serial.println("Step 14 - Buzzer Init");
	buzzer_Init();
	Serial.println("Step 15 - Variables Init");
	pinMode(heater, OUTPUT);
	pinMode(buzzer, OUTPUT);

	int count_w = 10;
	while (1) {
		dallRead();
		if (temperature1 != 5 || count_w == 0) break;
		count_w--;
		delay(200);
	}
	Serial.println("Temperature sensors:");
	if (DS_Count >= 1) Serial.println(temperature1);
	if (DS_Count >= 2) Serial.println(temperature2);
	if (DS_Count >= 3) Serial.println(temperature3);
	if (DS_Count >= 4) Serial.println(temperature4);
	if (DS_Count >= 5) Serial.println(temperature5);
	if (DS_Count >= 6) Serial.println(temperature6);
	if (DS_Count >= 7) Serial.println(temperature7);
	if (DS_Count >= 8) Serial.println(temperature8);
	Serial.println("Pressure sensor:");
	if (pressure_status) pressure = Pressure_read();
	Serial.println(pressure);
 
  for(int cnt=0; cnt<120; cnt++) temp_in[cnt] = temperature1 * 10;

  flipper.attach(1.0, myTimer);
#if defined OLED_Display
  oled_start_normal();
  read_temp_interval = 10;
#endif
  Serial.println("Setup Done!");
}
