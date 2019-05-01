//   Система автоматики винокура. 
//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
//   Версия 2.0 Release Candidate 13

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
	  // прием мощности по UART
	  if (Serial.available() > 0) {
		  uint8_t uartRead = Serial.read();
		  if (processMode.allow == 1) {
			  if (processMode.step < 2) power.inPowerHigh = uartRead;
			  else if (processMode.step < 4) power.inPowerLow = uartRead;
		  }
		  else if (processMode.allow == 2) {
			  if (processMode.step < 2) power.inPowerHigh = uartRead;
			  else if (processMode.step < 7) power.inPowerLow = uartRead;
		  }
	  }
	  // Выключение повышенного напряжения на клапана
	  if (CH_all == true) {
		  CH_all = false;

		  if (timeSetHighVoltage < millis()) {
			  if (pwmOut[8].invert == false) pwm.setPWM(PWM_CH9, 4096, 0);
			  else pwm.setPWM(PWM_CH9, 0, 4096);
		  }
	  }
  }

  sensorLoop();
  displayLoop();
  logfileLoop();

  yield();
}
