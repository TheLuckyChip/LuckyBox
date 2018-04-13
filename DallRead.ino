// Считывание температуры с датчиков

// Инициация датчиков температуры
OneWire ds(10); //Датчик на SD3
OneWire ds2(9); // Датчик на  SD5
OneWire ds3(0); //Датчик на D3
OneWire ds4(2); // Датчик на  D4
byte addr[8]; //Адрес датчика на SD3 
byte addr2[8]; //Адрес датчика на SD5
byte addr3[8]; //Адрес датчика на D3 
byte addr4[8]; //Адрес датчика на D4



void dallRead(unsigned long interval){
int scratchpad[9];

  static unsigned long prevTime = 0;
  if (millis() - prevTime > interval) { //Проверка заданного интервала
  static boolean flagDall = 0; //Признак операции
  prevTime = millis();
  flagDall =! flagDall; //Инверсия признака
  if (flagDall) {
    ds.reset();
    ds.write(0xCC); //Обращение ко всем датчикам
    ds.write(0x44); //Команда на конвертацию
    
    ds2.reset();
    ds2.write(0xCC); //Обращение ко всем датчикам
    ds2.write(0x44); //Команда на конвертацию
    
    ds3.reset();
    ds3.write(0xCC); //Обращение ко всем датчикам
    ds3.write(0x44); //Команда на конвертацию
    
    ds4.reset();
    ds4.write(0xCC); //Обращение ко всем датчикам
    ds4.write(0x44); //Команда на конвертацию
    
    flagDallRead = 1; //Время возврата в секундах
  }
  else {
   
     ds.reset();
     ds.select(addr);
     ds.write(0xBE); //Считывание значения с датчика
     float temp = (short)(ds.read() | ds.read()<<8); //Принимаем два байта температуры
     
     ds2.reset();
     ds2.select(addr2);
     ds2.write(0xBE); //Считывание значения с датчика
     float temp2 = (short)(ds2.read() | ds2.read()<<8); //Принимаем два байта температуры

     ds3.reset();
     ds3.select(addr3);
     ds3.write(0xBE); //Считывание значения с датчика
     float temp3 = (short)(ds3.read() | ds3.read()<<8); //Принимаем два байта температуры
     
     ds4.reset();
     ds4.select(addr4);
     ds4.write(0xBE); //Считывание значения с датчика
     float temp4 = (short)(ds4.read() | ds4.read()<<8); //Принимаем два байта температуры
     
     temperature  =  (float)temp * 0.0625;   //Т1
     temperature *= 100;                     // Округление до двух знаков после запятой
     temperature = floor(temperature+0.5);
     temperature /= 100;
     
     temperature2 =  (float)temp2 * 0.0625;  //Т2
     temperature2 *= 100;
     temperature2 = floor(temperature2+0.5);
     temperature2 /= 100;
     
     temperature3 =  (float)temp3 * 0.0625;  //Т3
     temperature3 *= 100;
     temperature3 = floor(temperature3+0.5);
     temperature3 /= 100;
     
     temperature4 =  (float)temp4 * 0.0625;  //Т4
     temperature4 *= 100;
     temperature4 = floor(temperature4+0.5);
     temperature4 /= 100; 
     
     Serial.print("Temperature 1:");
     Serial.println(temperature);
     
     Serial.print("Temperature 2:");
     Serial.println(temperature2);

     Serial.print("Temperature 3:");
     Serial.println(temperature3);
     
     Serial.print("Temperature 4:");
     Serial.println(temperature4);
     flagDallRead = 1; //Время возврата в секундах
     
   }
  } 
}
