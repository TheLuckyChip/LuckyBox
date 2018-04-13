void setup() {
 
 // HTTP = ESP8266WebServer (port);
  Serial.begin(115200);
  Serial.println("");
  //Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();
  Serial.println("Step7-FileConfig");
  loadConfig();
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit();
  Serial.println("Start 8-Time");
  // Получаем время из сети
  Time_init();
  //Настраиваем и запускаем SSDP интерфейс
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться, и мы в режиме AP, то SSDP нам не нужен. Ничего не делаем
  }
  else {
    //Удалось подключиться - запускаем SSDP
    Serial.println("Start 3-SSDP");
    SSDP_init();
  }
  
  //Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();
  Serial.println("Start 11-Reflux"); 
  Reflux_init();
   Serial.println("Start 12-Distillation");
  Destillation_init();
   Serial.println("Start 13-Brewing");
  Brewing_init();
   Serial.println("Start 14-Heater");
  Heater_init();
   Serial.println("Start 15-BMP");
  BPM085_init ();
   Serial.println("Start 16-Buzzer");
  buzzer_Init ();
// Выставляем разрешение 12 бит на датчиках

 ds.search(addr); //  определяем адрес на шине 1wire 
if (addr[0] == 40) // проверка, что мы будет писать именно в DS18B20
  {
  ds.reset(); // запрос шины 1-Wire
  ds.select(addr); // выбор нашего DS18B20
  ds.write(0x4E); // запись в  scratchPad
  ds.write(0x00); // User byte 0 - Unused
  ds.write(0x00); // User byte 1 - Unused
  ds.write(0x7F); // выставляем 12 бит (0x7F)
  ds.reset(); // сбрасываем 1-Wire
}

ds2.search(addr2); //  определяем адрес на шине 1wire 
if (addr2[0] == 40) // проверка, что мы будет писать именно в DS18B20
  {
  ds2.reset(); // запрос шины 1-Wire
  ds2.select(addr2); // выбор нашего DS18B20
  ds2.write(0x4E); // запись в  scratchPad
  ds2.write(0x00); // User byte 0 - Unused);
  ds2.write(0x00); // User byte 1 - Unused
  ds2.write(0x7F); // выставляем 12 бит (0x7F)
  ds2.reset(); // сбрасываем 1-Wire
  }
  
ds3.search(addr3); //  определяем адрес на шине 1wire 
if (addr3[0] == 40) // проверка, что мы будет писать именно в DS18B20
  {
  ds3.reset(); // запрос шины 1-Wire
  ds3.select(addr3); // выбор нашего DS18B20
  ds3.write(0x4E); // запись в  scratchPad
  ds3.write(0x00); // User byte 0 - Unused
  ds3.write(0x00); // User byte 1 - Unused
  ds3.write(0x7F); // выставляем 12 бит (0x7F)
  ds3.reset(); // сбрасываем 1-Wire
  }

 ds4.search(addr4); //  определяем адрес на шине 1wire 
if (addr2[0] == 40) // проверка, что мы будет писать именно в DS18B20
  {
  ds4.reset(); // запрос шины 1-Wire
  ds4.select(addr4); // выбор нашего DS18B20
  ds4.write(0x4E); // запись в  scratchPad
  ds4.write(0x00); // User byte 0 - Unused
  ds4.write(0x00); // User byte 1 - Unused
  ds4.write(0x7F); // выставляем 12 бит (0x7F)
  ds4.reset(); // сбрасываем 1-Wire
  }
  
 pinMode(heater, OUTPUT);
 pinMode(buzzer, OUTPUT);
 Serial.println("setup");
}
