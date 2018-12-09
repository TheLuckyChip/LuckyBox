#include "touch_interrupt.h"

void touchscreenUpdate()
{
	SPI.beginTransaction(SPI_SETTING_LOW);
	csOn(TOUCH_CS);
	
	SPI.transfer(0x80);
	SPI.transfer(0);
	SPI.transfer(0);
	delay(1);
	SPI.transfer(0x90); // запрос координаты X
	int16_t x1 = SPI.transfer16(0) >> 3;
	SPI.transfer(0xD0); // запрос координаты Y
	int16_t y1 = SPI.transfer16(0) >> 3;
	
	csOff(TOUCH_CS);
	SPI.endTransaction();

	if (touchInvert == true) {
		touch_x = x1;
		touch_y = y1;
	}
	else {
		touch_x = 4095 - x1;
		touch_y = 4095 - y1;
	}
#if defined Debug_en
	Serial.print("x = "); Serial.println(touch_x);
	Serial.print("y = "); Serial.println(touch_y);
#endif
	// приведем к нашему разрешению экрана
	float tmp_x = touch_x - 370;
	tmp_x /= 10.9375;
	if (tmp_x < 0) tmp_x = 0;
	else if (tmp_x > 319) tmp_x = 319;
	touch_x = (uint16_t)tmp_x;

	float tmp_y = touch_y - 300;
	tmp_y /= 14.5833;
	if (tmp_y < 0) tmp_y = 0;
	else if (tmp_y > 239) tmp_y = 239;
	touch_y = (uint16_t)tmp_y;
#if defined Debug_en
	Serial.print("x calc = "); Serial.println(touch_x);
	Serial.print("y calc = "); Serial.println(touch_y);
#endif
}

void touchscreenUpdateSet() {
	touch_in = true;
	detachInterrupt(intTouch);
}
