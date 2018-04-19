// oled.h

#ifndef _OLED_h
#define _OLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP_Adafruit_SSD1306.h>

#define OLED_RESET 0
extern Adafruit_SSD1306 display;
extern void initOLED();
extern void oledStartNormal();
extern void oledOutDisplay();

#endif

