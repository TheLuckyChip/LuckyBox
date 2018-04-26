// 
// 
// 

#include "touch_interrupt.h"
uint16_t cnt_tmp;

void touchInt() {
	cnt_tmp++;
	Serial.print("Initerrupt: ");
	Serial.println(cnt_tmp);
}
