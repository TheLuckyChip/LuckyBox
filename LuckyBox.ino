//   Система автоматики винокура. 
//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
//   Версия 2.1 Release Candidate 7

#include "device_view.h"
#include "pid_config.h"
#include "adc.h"
#include "sd_card.h"
#include "display.h"
#include "sensors.h"
#include "heater.h"
#include "tft.h"
#include "setting.h"
#include "reflux_mode.h"
#include "distillation_mode.h"
#include "mashing_mode.h"
#include "pid_config.h"
#include "brewing_mode.h"

//uint8_t powOld = 200;

void loop() {
  HTTP.handleClient();
  switch (processMode.allow) {
    case 0: tftMenuLoop(); break;
	case 1: distillationLoop(); break;
	case 2: refluxLoop(); break;
	case 3: mashingLoop(); break;
	case 4: pidSetLoop(); break;
	//case 5: brewingLoop(); break;
	case 6: deviceViewLoop(); break;
  }
  adcLoop();
  heaterLoop();
  sensorLoop();
  displayLoop();
  logfileLoop();
  yield();

  /*if (power.heaterPower != powOld) {
	  powOld = power.heaterPower;
	  servoAngleSet(4, powOld);
  }*/
}
