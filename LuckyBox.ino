//   Система автоматики винокура. 
//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
//   Версия 2.0 Release Candidate 16

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
#include "misc.h"

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

  if (processMode.allow < 3 || processMode.allow > 5) {
	  adcLoop();
	  heaterLoop();
	  serialLoop();
	  stepApLoop();
  }

  sensorLoop();
  displayLoop();
  logfileLoop();

  yield();
}
