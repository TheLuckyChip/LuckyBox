// 
// 
// 

#include "touch_interrupt.h"
#include "user_config.h"
#include "setting.h"
#include "buzzer.h"
#include "SPI.h"
//uint16_t cnt_tmp;
int16_t old_touch_x = 0, old_touch_y = 0;

//int16_t zraw = 0;
#define Z_THRESHOLD     400
#define Z_THRESHOLD_I   20
#define Z_THRESHOLD_D   -20
//#define Z_THRESHOLD_INT	75
//#define MSEC_THRESHOLD  3
#define SPI_SETTING     SPISettings(2000000, MSBFIRST, SPI_MODE0)

static int16_t besttwoavg(int16_t x, int16_t y, int16_t z) {
	int16_t da, db, dc;
	int16_t reta = 0;
	if (x > y) da = x - y; else da = y - x;
	if (x > z) db = x - z; else db = z - x;
	if (z > y) dc = z - y; else dc = y - z;

	if (da <= db && da <= dc) reta = (x + y) >> 1;
	else if (db <= da && db <= dc) reta = (x + z) >> 1;
	else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

	return (reta);
}

void touchscreenUpdate()
{
	int16_t data[6], z1, z2;
	int z, rotation = 3;

	csOn(15);
	SPI.beginTransaction(SPI_SETTING);
	SPI.transfer(0xB1 /* Z1 */);
	z1 = SPI.transfer16(0xC1 /* Z2 */) >> 3;
	z = z1 + 4095;
	z2 = SPI.transfer16(0x91 /* X */) >> 3;
	z -= z2;
	if (z >= Z_THRESHOLD) {
		SPI.transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
		data[0] = SPI.transfer16(0xD1 /* Y */) >> 3;
		data[1] = SPI.transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
		data[2] = SPI.transfer16(0xD1 /* Y */) >> 3;
		data[3] = SPI.transfer16(0x91 /* X */) >> 3;
	}
	else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
	data[4] = SPI.transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
	data[5] = SPI.transfer16(0) >> 3;
	SPI.endTransaction();
	csOff(15);

	if (z < 0) z = 0;
	if (z < Z_THRESHOLD) return;
	else {
		// Average pair with least distance between each measured x then y
		int16_t x1 = besttwoavg(data[0], data[2], data[4]);
		int16_t y1 = besttwoavg(data[1], data[3], data[5]);
		delay(10);
		csOn(15);
		SPI.beginTransaction(SPI_SETTING);
		SPI.transfer(0xB1 /* Z1 */);
		z1 = SPI.transfer16(0xC1 /* Z2 */) >> 3;
		z = z1 + 4095;
		z2 = SPI.transfer16(0x91 /* X */) >> 3;
		z -= z2;
		if (z >= Z_THRESHOLD) {
			SPI.transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
			data[0] = SPI.transfer16(0xD1 /* Y */) >> 3;
			data[1] = SPI.transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
			data[2] = SPI.transfer16(0xD1 /* Y */) >> 3;
			data[3] = SPI.transfer16(0x91 /* X */) >> 3;
		}
		else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
		data[4] = SPI.transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
		data[5] = SPI.transfer16(0) >> 3;
		SPI.endTransaction();
		csOff(15);

		if (z < 0) z = 0;
		if (z < Z_THRESHOLD) return;
		// Average pair with least distance between each measured x then y
		int16_t x2 = besttwoavg(data[0], data[2], data[4]);
		int16_t y2 = besttwoavg(data[1], data[3], data[5]);

		int16_t dx = x1 - x2;
		int16_t dy = y1 - y2;
		if (dx < Z_THRESHOLD_I && dx > Z_THRESHOLD_D && dy < Z_THRESHOLD_I && dy > Z_THRESHOLD_D) {
			switch (rotation) {
			case 0:
				touch_x = 4095 - y1;
				touch_y = x1;
				break;
			case 1:
				touch_x = x1;
				touch_y = y1;
				break;
			case 2:
				touch_x = y1;
				touch_y = 4095 - x1;
				break;
			default: // 3
				touch_x = 4095 - x1;
				touch_y = 4095 - y1;
			}

			// приведем к нашему разрешению экрана
			if (touch_x <= 400) touch_x = 0;
			else if (touch_x >= 3900) touch_x = 319;
			else {
				float tmp_x = touch_x;
				tmp_x /= 10.9375;
				touch_x = (uint16_t)tmp_x;
			}
			if (touch_y <= 300) touch_y = 0;
			else if (touch_y >= 3800) touch_y = 319;
			else {
				float tmp_y = touch_y;
				tmp_y /= 14.5833;
				touch_y = (uint16_t)tmp_y;
			}
		}
	}


	if (old_touch_x != touch_x || old_touch_y != touch_y) {
		initBuzzer(5);
		old_touch_x = touch_x;
		old_touch_y = touch_y;
		touch_in = true;
	}
}
