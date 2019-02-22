#include "device_view.h"

unsigned long timeOut;
uint16_t color;

void deviceTempAdcOutScreen() {
	String time_ntp = GetTime();
	
	// вывод времени и давления
	tft.setTextSize(2);
	tft.setCursor(223, 2);
	tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLACK);
	tft.print(time_ntp);
	tft.setCursor(236, 26);
	tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK); // голубой
	tft.printf("%d mm", (int)pressureSensor.data);
	tft.setCursor(242, 52);
	tft.setTextColor(ILI9341_ORANGE, ILI9341_BLACK); // оранжевый
	if (Voltage > 999) Voltage = 999; // чтобы не выйти за пределы индикации в настройках
	tft.printf("%d V", Voltage);

	// вывод температур
	tft.setCursor(0, 2);
	if (DS_Cube != 10 && temperatureSensor[DS_Cube].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Cube].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Cube != 10 && temperatureSensor[DS_Cube].data < 150.0) tft.printf("T1:%.1f ", temperatureSensor[DS_Cube].data);
	else if (temperatureSensor[0].data < 150.0) tft.printf("T1:%.1f ", temperatureSensor[0].data);
	else tft.print("T1:--.-");

	tft.setCursor(110, 2);
	if (DS_Tube != 10 && temperatureSensor[DS_Tube].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Tube].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Tube != 10 && temperatureSensor[DS_Tube].data < 150.0) tft.printf("T2:%.1f ", temperatureSensor[DS_Tube].data);
	else if (temperatureSensor[1].data < 150.0) tft.printf("T2:%.1f ", temperatureSensor[1].data);
	else tft.print("T2:--.-");

	tft.setCursor(0, 22);
	if (DS_Out != 10 && temperatureSensor[DS_Out].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Out].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Out != 10 && temperatureSensor[DS_Out].data < 150.0) tft.printf("T3:%.1f ", temperatureSensor[DS_Out].data);
	else if (temperatureSensor[2].data < 150.0) tft.printf("T3:%.1f ", temperatureSensor[2].data);
	else tft.print("T3:--.-");

	tft.setCursor(110, 22);
	if (DS_Def != 10 && temperatureSensor[DS_Def].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Def].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Def != 10 && temperatureSensor[DS_Def].data < 150.0) tft.printf("T4:%.1f ", temperatureSensor[DS_Def].data);
	else if (temperatureSensor[3].data < 150.0) tft.printf("T4:%.1f ", temperatureSensor[3].data);
	else tft.print("T4:--.-");

	tft.setCursor(0, 42);
	if (DS_Res1 != 10 && temperatureSensor[DS_Res1].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Res1].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Res1 != 10 && temperatureSensor[DS_Res1].data < 150.0) tft.printf("T5:%.1f ", temperatureSensor[DS_Res1].data);
	else if (temperatureSensor[4].data < 150.0) tft.printf("T5:%.1f ", temperatureSensor[4].data);
	else tft.print("T5:--.-");

	tft.setCursor(110, 42);
	if (DS_Res2 != 10 && temperatureSensor[DS_Res2].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Res2].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Res2 != 10 && temperatureSensor[DS_Res2].data < 150.0) tft.printf("T6:%.1f ", temperatureSensor[DS_Res2].data);
	else if (temperatureSensor[5].data < 150.0) tft.printf("T6:%.1f ", temperatureSensor[5].data);
	else tft.print("T6:--.-");

	tft.setCursor(0, 62);
	if (DS_Res3 != 10 && temperatureSensor[DS_Res3].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Res3].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Res3 != 10 && temperatureSensor[DS_Res3].data < 150.0) tft.printf("T7:%.1f ", temperatureSensor[DS_Res3].data);
	else if (temperatureSensor[6].data < 150.0) tft.printf("T7:%.1f ", temperatureSensor[6].data);
	else tft.print("T7:--.-");

	tft.setCursor(110, 62);
	if (DS_Res4 != 10 && temperatureSensor[DS_Res4].color != ILI9341_BLACK) tft.setTextColor(temperatureSensor[DS_Res4].color, ILI9341_BLACK);
	else tft.setTextColor(ILI9341_DARKDARK, ILI9341_BLACK);
	if (DS_Res4 != 10 && temperatureSensor[DS_Res4].data < 150.0) tft.printf("T8:%.1f ", temperatureSensor[DS_Res4].data);
	else if (temperatureSensor[7].data < 150.0) tft.printf("T8:%.1f ", temperatureSensor[7].data);
	else tft.print("T8:--.-");

	// вывод датчиков безопасности
	if (adcIn[0].allert == true) color = ILI9341_RED;
	else color = ILI9341_LIGHTGREY;
	tft.setTextColor(color, ILI9341_BLACK);
	tft.drawCircle(39, 110, 22, color); tft.drawCircle(39, 110, 23, color); tft.drawCircle(39, 110, 21, color);
	tft.setCursor(29, 103);
	tft.print("A0");
	if (adcIn[1].allert == true) color = ILI9341_RED;
	else color = ILI9341_LIGHTGREY;
	tft.setTextColor(color, ILI9341_BLACK);
	tft.drawCircle(119, 110, 22, color); tft.drawCircle(119, 110, 23, color); tft.drawCircle(119, 110, 21, color);
	tft.setCursor(110, 103);
	tft.print("A1");
	if (adcIn[2].allert == true) color = ILI9341_RED;
	else color = ILI9341_LIGHTGREY;
	tft.setTextColor(color, ILI9341_BLACK);
	tft.drawCircle(199, 110, 22, color); tft.drawCircle(199, 110, 23, color); tft.drawCircle(199, 110, 21, color);
	tft.setCursor(189, 103);
	tft.print("A2");
	if (adcIn[3].allert == true) color = ILI9341_RED;
	else color = ILI9341_LIGHTGREY;
	tft.setTextColor(color, ILI9341_BLACK);
	tft.drawCircle(279, 110, 22, color); tft.drawCircle(279, 110, 23, color); tft.drawCircle(279, 110, 21, color);
	tft.setCursor(269, 103);
	tft.print("A3");
}

void deviceViewLoop() {
	if (timeOut <= millis()) {
		timeOut = millis() + 250;
		switch (processMode.step) {
			case 0: {
				timeOut = millis();
	#if defined TFT_Display
				// подготовка данных для вывода на TFT
				csOn(TFT_CS);
				fillScreenRect(0, 0, 320, 240, ILI9341_BLACK);
				tft.setTextSize(2);

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

				deviceTempAdcOutScreen();

				csOff(TFT_CS);
	#endif
				processMode.step = 1;
				break;
			}
			case 1: {
				if (touchScreen == 0) {
#if defined TFT_Display
					csOn(TFT_CS);

					deviceTempAdcOutScreen();

					if (touchScreenDV == 1) {
						touchScreenDV = 0;
						// вывод клапанов
						if (touchArea == 61) {
							touchArea = 0;
							if (CH1 == false) { CH1 = true; csOn(PWM_CH1); color = ILI9341_GREEN; }
							else { CH1 = false; csOff(PWM_CH1); color = ILI9341_LIGHTGREY; }
							tft.drawRect(10, 150, 60, 60, color); tft.drawRect(11, 151, 58, 58, color);
							tft.setTextColor(color, ILI9341_BLACK);
							tft.setCursor(29, 162);
							tft.print("CH");
							tft.setCursor(36, 182);
							tft.print("1");
							tft.fillRect(20, 146, 39, 3, color);
							tft.fillRect(20, 210, 39, 2, color);
							tft.fillTriangle(14, 212, 65, 212, 40, 230, color);
						}
						else if (touchArea == 62) {
							touchArea = 0;
							if (CH2 == false) { CH2 = true; csOn(PWM_CH2); color = ILI9341_GREEN; }
							else { CH2 = false; csOff(PWM_CH2); color = ILI9341_LIGHTGREY; }
							tft.drawRect(90, 150, 60, 60, color); tft.drawRect(91, 151, 58, 58, color);
							tft.setTextColor(color, ILI9341_BLACK);
							tft.setCursor(109, 162);
							tft.print("CH");
							tft.setCursor(116, 182);
							tft.print("2");
							tft.fillRect(100, 146, 39, 3, color);
							tft.fillRect(100, 210, 39, 2, color);
							tft.fillTriangle(94, 212, 145, 212, 120, 230, color);
						}
						else if (touchArea == 63) {
							touchArea = 0;
							if (CH3 == false) { CH3 = true; csOn(PWM_CH3); color = ILI9341_GREEN; }
							else { CH3 = false; csOff(PWM_CH3); color = ILI9341_LIGHTGREY; }
							tft.drawRect(170, 150, 60, 60, color); tft.drawRect(171, 151, 58, 58, color);
							tft.setTextColor(color, ILI9341_BLACK);
							tft.setCursor(189, 162);
							tft.print("CH");
							tft.setCursor(196, 182);
							tft.print("3");
							tft.fillRect(180, 146, 39, 3, color);
							tft.fillRect(180, 210, 39, 2, color);
							tft.fillTriangle(174, 212, 225, 212, 200, 230, color);
						}
						else if (touchArea == 64) {
							touchArea = 0;
							if (CH4 == false) { CH4 = true; csOn(PWM_CH4); color = ILI9341_GREEN; }
							else { CH4 = false; csOff(PWM_CH4); color = ILI9341_LIGHTGREY; }
							tft.drawRect(250, 150, 60, 60, color); tft.drawRect(251, 151, 58, 58, color);
							tft.setTextColor(color, ILI9341_BLACK);
							tft.setCursor(269, 162);
							tft.print("CH");
							tft.setCursor(276, 182);
							tft.print("4");
							tft.fillRect(260, 146, 39, 3, color);
							tft.fillRect(260, 210, 39, 2, color);
							tft.fillTriangle(254, 212, 305, 212, 280, 230, color);
						}
					}
					csOff(TFT_CS);
#endif
				}
				break;
			}
		}
	}
	yield();
}
