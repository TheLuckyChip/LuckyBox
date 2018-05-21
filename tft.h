// tft.h

#ifndef _TFT_h
#define _TFT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Fonts/FreeSerifBold24pt7b.h"
#include "TFT_ILI9341.h"

typedef struct DS_Graph {
	uint16_t color;
	uint8_t	 number;
	int temp_in_tft[312];
};

extern Adafruit_ILI9341 tft;
extern void initTFT();
extern void tftStartForGraph();
extern void tftOutGraphDisplay();
extern void tftOutMenu();

#endif

