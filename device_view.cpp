#include "device_view.h"

unsigned long timeOut;
uint16_t color;

#define DEG2RAD 0.0174532925
void fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour) {

  byte seg = 3; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 3; // Draw segments every 3 degrees, increase to 6 for segmented ring

  // Calculate first pair of coordinates for segment start
  float sx = cos((start_angle - 90) * DEG2RAD);
  float sy = sin((start_angle - 90) * DEG2RAD);
  uint16_t x0 = sx * (rx - w) + x;
  uint16_t y0 = sy * (ry - w) + y;
  uint16_t x1 = sx * rx + x;
  uint16_t y1 = sy * ry + y;

  // Draw colour blocks every inc degrees
	csOn(TFT_CS);
  for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

    // Calculate pair of coordinates for segment end
    float sx2 = cos((i + seg - 90) * DEG2RAD);
    float sy2 = sin((i + seg - 90) * DEG2RAD);
    int x2 = sx2 * (rx - w) + x;
    int y2 = sy2 * (ry - w) + y;
    int x3 = sx2 * rx + x;
    int y3 = sy2 * ry + y;
		
    tft.fillTriangle(x0, y0, x1, y1, x2, y2, colour);
    tft.fillTriangle(x1, y1, x2, y2, x3, y3, colour);
		
    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
	csOff(TFT_CS);
}
void outPercent(uint16_t data, uint16_t x, uint16_t y) {
	csOn(TFT_CS);
	// значение АЦП в процентах
	uint8_t percent = data / 114;
  if (percent < 10) { 
		tft.fillRect(x,y,6,8,ILI9341_BLACK);
		tft.setCursor(x+6, y);
		tft.printf("%d%%", percent);
		tft.fillRect(x+18,y,6,8,ILI9341_BLACK);
	}	else if (percent < 100) {
		tft.fillRect(x,y,3,8,ILI9341_BLACK);
		tft.setCursor(x+3, y);
		tft.printf("%d%%", percent);
		tft.fillRect(x+21,y,3,8,ILI9341_BLACK);
	}	else {
		tft.setCursor(x, y);
		tft.printf("%d%%", percent);
	}

	csOff(TFT_CS);
}
void printTftTemp(float data) {
	if (data >= 100.0) tft.printf("%.1f ", data);
	else if (data >= 10.0) tft.printf("%.2f ", data);
	//else if (data > 0.0 && data < 10.0) { tft.printf(" %.2f ", data); }
	else if (data > -10.0) { tft.printf("%.2f ", data); }
	else tft.printf("%.1f ", data);
}
void deviceTempAdcOutScreen() {
	uint16_t color;
	String time_ntp = GetTime();
	csOn(TFT_CS);
	// вывод времени и давления
	tft.setTextSize(2);

	tft.setCursor(223, 2);
	tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
	tft.print(time_ntp);
	tft.setCursor(223, 21);
	tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK); // голубой
	tft.printf("%d mm", (int)pressureSensor.data);
	if (WiFi.status() == WL_CONNECTED) {
		tft.setCursor(223, 40);
		int32_t mDbi = WiFi.RSSI(myNetworkNum);
		if (mDbi < -80) tft.setTextColor(ILI9341_PINK, ILI9341_BLACK); // красный
		else if (mDbi < -60) tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK); // оранжевый
		else tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK); // зеленый
		tft.printf("%d dBm", WiFi.RSSI(myNetworkNum));
	}
	tft.setTextSize(1);
	tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
	tft.setCursor(223, 58);
	tft.printf("FREE HEAP: %dKb", ESP.getFreeHeap()/1000);

	tft.setCursor(223, 68);
	tft.print("SN: ");
	String addrMacM = "            ";
	addrMacM[0] = addrMacMod[0];	addrMacM[1] = addrMacMod[1];
	addrMacM[2] = addrMacMod[3];	addrMacM[3] = addrMacMod[4];
	addrMacM[4] = addrMacMod[6];	addrMacM[5] = addrMacMod[7];
	addrMacM[6] = addrMacMod[9];	addrMacM[7] = addrMacMod[10];
	addrMacM[8] = addrMacMod[12]; addrMacM[9] = addrMacMod[13];
	addrMacM[10] = addrMacMod[15]; addrMacM[11] = addrMacMod[16];
	tft.print(addrMacM);
	
	// вывод температур
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setCursor(30, 2);
	if (DS_Count > 0) printTftTemp(temperatureSensor[0].data);
	else tft.print("--.--");
	tft.setCursor(140, 2);
	if (DS_Count > 1) printTftTemp(temperatureSensor[1].data); 
	else tft.print("--.--");
	tft.setCursor(30, 22);
	if (DS_Count > 2) printTftTemp(temperatureSensor[2].data);
	else tft.print("--.--");
	tft.setCursor(140, 22);
	if (DS_Count > 3) printTftTemp(temperatureSensor[3].data);
	else tft.print("--.--");
	tft.setCursor(30, 42);
	if (DS_Count > 4) printTftTemp(temperatureSensor[4].data);
	else tft.print("--.--");
	tft.setCursor(140, 42);
	if (DS_Count > 5) printTftTemp(temperatureSensor[5].data);
	else tft.print("--.--");
	tft.setCursor(30, 62);
	if (DS_Count > 6) printTftTemp(temperatureSensor[6].data);
	else tft.print("--.--");
	tft.setCursor(140, 62);
	if (DS_Count > 7) printTftTemp(temperatureSensor[7].data);
	else tft.print("--.--");

	color = ILI9341_LIGHTGREY;
	tft.setTextColor(color, ILI9341_BLACK);

	tft.setTextSize(1);
	csOff(TFT_CS);

	outPercent(adcIn[0].data,29,100);
	outPercent(adcIn[1].data,109,100);
	outPercent(adcIn[2].data,189,100);
	outPercent(adcIn[3].data,269,100);

	int deg = (int)(adcIn[0].data / 126); if (deg > 90) deg = 90;
	fillArc(40, 110, 225, deg, 28, 28, 5, ILI9341_ORANGE); fillArc(40, 110, (225 + deg * 3), (90 - deg), 28, 28, 5, ILI9341_DARKDARK);
	
	deg = (int)(adcIn[1].data / 126); if (deg > 90) deg = 90;
	fillArc(120, 110, 225, deg, 28, 28, 5, ILI9341_ORANGE); fillArc(120, 110, (225 + deg * 3), (90 - deg), 28, 28, 5, ILI9341_DARKDARK);

  deg = (int)(adcIn[2].data / 126); if (deg > 90) deg = 90;
	fillArc(200, 110, 225, deg, 28, 28, 5, ILI9341_ORANGE); fillArc(200, 110, (225 + deg * 3), (90 - deg), 28, 28, 5, ILI9341_DARKDARK);

	deg = (int)(adcIn[3].data / 126); if (deg > 90) deg = 90;
	fillArc(280, 110, 225, deg, 28, 28, 5, ILI9341_ORANGE); fillArc(280, 110, (225 + deg * 3), (90 - deg), 28, 28, 5, ILI9341_DARKDARK);

	//if (adcIn[0].data > 4095) { processMode.volume = 100; settingAlarm = true; initBuzzer(200); }
}

void deviceViewLoop() {
	uint16_t color;
	if (timeOut <= millis()) {
		timeOut = millis() + 250;
		switch (processMode.step) {
			case 0: {
				timeOut = millis();
				/*for (int i=0; i<8; i++) {
					pwmChannel[i].member = 1;
					if (i < 4) adcSensors[i].member = 1;
				}*/

				// подготовка данных для вывода на TFT
				csOn(TFT_CS);
				tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
				tft.setTextSize(2);
				color = ILI9341_LIGHTGREY;
				tft.setTextColor(color, ILI9341_BLACK);
				tft.setCursor(29, 114); tft.print("A1");
				tft.setCursor(109, 114); tft.print("A2");
				tft.setCursor(189, 114); tft.print("A3");
				tft.setCursor(269, 114); tft.print("A4");
				tft.drawRect(10, 150, 60, 60, color); tft.drawRect(11, 151, 58, 58, color);
				tft.setCursor(29, 162);
				tft.print("CH");
				tft.setCursor(36, 182);
				tft.print("1");
				tft.fillRect(20, 146, 39, 3, color);
				tft.fillRect(20, 210, 39, 2, color);
				tft.fillTriangle(14, 212, 65, 212, 40, 230, color);

				if (CH1 == false) color = ILI9341_LIGHTGREY;
        else color = ILI9341_GREEN;
        tft.drawRect(10, 150, 60, 60, color); tft.drawRect(11, 151, 58, 58, color);
        tft.setTextColor(color, ILI9341_BLACK);
        tft.setCursor(29, 162);
        tft.print("CH");
        tft.setCursor(36, 182);
        tft.print("1");
        tft.fillRect(20, 146, 39, 3, color);
        tft.fillRect(20, 210, 39, 2, color);
        tft.fillTriangle(14, 212, 65, 212, 40, 230, color);

        if (CH2 == false) color = ILI9341_LIGHTGREY;
        else color = ILI9341_GREEN;
        tft.drawRect(90, 150, 60, 60, color); tft.drawRect(91, 151, 58, 58, color);
        tft.setTextColor(color, ILI9341_BLACK);
        tft.setCursor(109, 162);
        tft.print("CH");
        tft.setCursor(116, 182);
        tft.print("2");
        tft.fillRect(100, 146, 39, 3, color);
        tft.fillRect(100, 210, 39, 2, color);
        tft.fillTriangle(94, 212, 145, 212, 120, 230, color);

        if (CH3 == false) color = ILI9341_LIGHTGREY;
        else color = ILI9341_GREEN;
        tft.drawRect(170, 150, 60, 60, color); tft.drawRect(171, 151, 58, 58, color);
        tft.setTextColor(color, ILI9341_BLACK);
        tft.setCursor(189, 162);
        tft.print("CH");
        tft.setCursor(196, 182);
        tft.print("3");
        tft.fillRect(180, 146, 39, 3, color);
        tft.fillRect(180, 210, 39, 2, color);
        tft.fillTriangle(174, 212, 225, 212, 200, 230, color);

        if (CH4 == false) color = ILI9341_LIGHTGREY;
        else color = ILI9341_GREEN;
        tft.drawRect(250, 150, 60, 60, color); tft.drawRect(251, 151, 58, 58, color);
        tft.setTextColor(color, ILI9341_BLACK);
        tft.setCursor(269, 162);
        tft.print("CH");
        tft.setCursor(276, 182);
        tft.print("4");
        tft.fillRect(260, 146, 39, 3, color);
        tft.fillRect(260, 210, 39, 2, color);
        tft.fillTriangle(254, 212, 305, 212, 280, 230, color);

				// вывод заголовков температур
				tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
				tft.setTextSize(2); tft.setCursor(0, 2); tft.print("T");	tft.setTextSize(1); tft.setCursor(12, 9); tft.print("1");
				tft.setTextSize(2); tft.setCursor(110, 2); tft.print("T");	tft.setTextSize(1); tft.setCursor(122, 9); tft.print("2");
				tft.setTextSize(2); tft.setCursor(0, 22); tft.print("T");	tft.setTextSize(1); tft.setCursor(12, 29); tft.print("3");
				tft.setTextSize(2); tft.setCursor(110, 22); tft.print("T");	tft.setTextSize(1); tft.setCursor(122, 29); tft.print("4");
				tft.setTextSize(2); tft.setCursor(0, 42); tft.print("T");	tft.setTextSize(1); tft.setCursor(12, 49); tft.print("5");
				tft.setTextSize(2); tft.setCursor(110, 42); tft.print("T");	tft.setTextSize(1); tft.setCursor(122, 49); tft.print("6");
				tft.setTextSize(2); tft.setCursor(0, 62); tft.print("T");	tft.setTextSize(1); tft.setCursor(12, 69); tft.print("7");
				tft.setTextSize(2); tft.setCursor(110, 62); tft.print("T");	tft.setTextSize(1); tft.setCursor(122, 69); tft.print("8");
				csOff(TFT_CS);

				deviceTempAdcOutScreen();

				processMode.step = 1;
				break;
			}
			case 1: {
				if (touchScreen == 0) {
					deviceTempAdcOutScreen();
          if (adcIn[0].allert || adcIn[1].allert || adcIn[2].allert || adcIn[3].allert) settingAlarm = true;
          else settingAlarm = false;

					if (touchScreenDV == 1) {
            csOn(TFT_CS);
            touchScreenDV = 0;
            // вывод клапанов
            if (touchArea == 61) {
              touchArea = 0;
              if (CH1 == false) { CH1 = true; csOn(PWM_CH1); color = ILI9341_GREEN; }
              else { CH1 = false; csOff(PWM_CH1); color = ILI9341_LIGHTGREY; }
              tft.drawRect(10, 150, 60, 60, color); tft.drawRect(11, 151, 58, 58, color);
              tft.fillRect(20, 146, 39, 3, color);
              tft.fillRect(20, 210, 39, 2, color);
              tft.fillTriangle(14, 212, 65, 212, 40, 230, color);
            }
            else if (touchArea == 62) {
              touchArea = 0;
              uint16_t tap = (uint16_t)((float)(TapCorrection * percentCorrectSquare[20]) / 5);
              if (CH2 == false) { CH2 = true; csOn(PWM_CH2); setPWM(PWM_CH5, 0, tap); color = ILI9341_GREEN; }
              else { CH2 = false; csOff(PWM_CH2); setPWM(PWM_CH5, 0, 10); color = ILI9341_LIGHTGREY; }
              tft.drawRect(90, 150, 60, 60, color); tft.drawRect(91, 151, 58, 58, color);
              tft.fillRect(100, 146, 39, 3, color);
              tft.fillRect(100, 210, 39, 2, color);
              tft.fillTriangle(94, 212, 145, 212, 120, 230, color);
            }
            else if (touchArea == 63) {
              touchArea = 0;
              if (CH3 == false) { CH3 = true; csOn(PWM_CH3); color = ILI9341_GREEN; }
              else { CH3 = false; csOff(PWM_CH3); color = ILI9341_LIGHTGREY; }
              tft.drawRect(170, 150, 60, 60, color); tft.drawRect(171, 151, 58, 58, color);
              tft.fillRect(180, 146, 39, 3, color);
              tft.fillRect(180, 210, 39, 2, color);
              tft.fillTriangle(174, 212, 225, 212, 200, 230, color);
            }
            else if (touchArea == 64) {
              touchArea = 0;
              if (CH4 == false) { CH4 = true; csOn(PWM_CH4); color = ILI9341_GREEN; }
              else { CH4 = false; csOff(PWM_CH4); color = ILI9341_LIGHTGREY; }
              tft.drawRect(250, 150, 60, 60, color); tft.drawRect(251, 151, 58, 58, color);
              tft.fillRect(260, 146, 39, 3, color);
              tft.fillRect(260, 210, 39, 2, color);
              tft.fillTriangle(254, 212, 305, 212, 280, 230, color);
            }
          csOff(TFT_CS);
          delay(5);
          }
				}
				break;
			}
		}
	}
}
