// Ректификация

float settingColumn =101;         // Температура срабатывания оповещения от датчика в царге
float temperatureStartPressure = 78;   //Температура кипения спирта при запуске отслеживания ректификации
float settingColumnShow = 0;
float temperatureAlcoholBoil = 0;

//Установка уставки для ректификации
void Reflux_init() {
  HTTP.on("/SetTemp", handleSetTemp);    // Установка уставки для ректификации (вкладка Reflux)
  HTTP.on("/reflux.json", handleRefluxJSON); // формирование reflux.json страницы для передачи данных в web интерфейс
}

void handleSetTemp() {              //
  settingColumn = HTTP.arg("setting").toFloat();         // Получаем значение уставки из запроса и сохраняем в глобальной переменной
  temperatureStartPressure = HTTP.arg("temperatureAlcoholBoil").toFloat(); 
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handleRefluxJSON() {
  String root = "{}";  // Формируем строку для отправки в файл конфигурации в json формате

   temperatureAlcoholBoil = 78.91 -(780-pressure)*0.038; // расчет температуры кипения спирта при данном давлении
   settingColumnShow = settingColumn + (temperatureAlcoholBoil-temperatureStartPressure); // расчет уставки при изменившемся атмосферном давлении
   
   //AlarmSound (settingAlarm);
   
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(root);
 
  json["temperature"] = temperature1;
  json["temperature2"] = temperature2;
  json["temperature3"] = temperature3;
  json["temperature4"] = temperature4;
  json["setting"] = settingColumnShow;
  json["settingAlarm"] = settingAlarm;
  json["temperatureAlcoholBoil"] = temperatureAlcoholBoil;
  json["pressure"] = pressure;
  json["temperatureStartPressure"] = temperatureStartPressure;
  
  root = "";
  json.printTo(root);
  HTTP.send(200, "text/json", root);
}

void refluxLoop() {

  if (temperature2 >= settingColumnShow) {
    settingAlarm = true;
  } else {
    settingAlarm = false;
  }
}




