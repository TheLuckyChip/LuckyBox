/*
   Система автоматики винокура. 
   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
*/
#include "display.h"
#include "touch_interrupt.h"
#include "setup.h"
#include "wifi_config.h"
#include "time_config.h"
#include "ssdp.h"
#include "reflux_mode.h"
#include "tft.h"
#include "http_config.h"
#include "fs_config.h"
#include "file_config.h"
#include "distillation_mode.h"
#include "sensors.h"
#include "setting.h"
#include "buzzer.h"
#include "heater.h"
#include "brewing_mode.h"
#include "Wire.h" 
#include "user_config.h"

void loop() {
  HTTP.handleClient();
  distillationLoop();
  refluxLoop();
  brewingLoop();
  heaterLoop();
  sensorLoop();
  displayLoop();
  yield();
}
