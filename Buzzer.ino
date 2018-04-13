

#include <Ticker.h>

bool bounce = false;


Ticker tickerSet;

void setPin() {

  if (settingAlarm) {
  digitalWrite(buzzer, bounce);
  bounce = !bounce;
  }
}


  
 void buzzer_Init() {
  
  tickerSet.attach_ms(3, setPin);
  
 }




