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

	touch_x = 4095 - x1;
	touch_y = 4095 - y1;
	// приведем к нашему разрешению экрана
	if (touch_x <= 400) touch_x = 0;
	else if (touch_x >= 3900) touch_x = 319;
	else {
		float tmp_x = touch_x;
		tmp_x /= 10.9375;
		touch_x = (uint16_t)tmp_x;
	}
	if (touch_y <= 300) touch_y = 0;
	else if (touch_y >= 3800) touch_y = 239;
	else {
		float tmp_y = touch_y;
		tmp_y /= 14.5833;
		touch_y = (uint16_t)tmp_y;
	}
}

void touchscreenUpdateSet() {
	touch_in = true;
}
