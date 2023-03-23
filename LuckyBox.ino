//   Система автоматики винокура. 
//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
//   Версия 2.0.22 Release

#include "device_view.h"
#include "adc.h"
#include "sd_card.h"
#include "display.h"
#include "tft.h"
#include "sensors.h"
#include "heater.h"
#include "setting.h"
#include "reflux_mode.h"
#include "distillation_mode.h"
#include "mashing_mode.h"
#include "pid_config.h"
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

	  if (processMode.allow < 3 || processMode.allow == 6) {
		  adcLoop();
		  stepApLoop();
//#ifndef power_PWM
//		  if (powerType <= 1) heaterLoop();
//#endif
	  }
	  if (powerType == 2) comHeaterLoop();
    else heaterLoop();
	  sensorLoop();
	  displayLoop();
	  logfileLoop();

  yield();
}
