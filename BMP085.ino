
#include "Wire.h"

#include "Adafruit_BMP085.h"

Adafruit_BMP085 bmp;
   
float temperatureBPM085 = 0, pressure = 0, altitude = 0, temperatureAlcoholBoil =0;

void BPM085_init() {

  if(!bmp.begin()) {
     Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
     pressure = 24;
   }
  Wire.begin(4, 5);
} 

  

