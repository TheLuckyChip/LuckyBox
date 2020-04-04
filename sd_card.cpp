#include "sd_card.h"

File myFile;

String fileName, fileData;
byte second, minute, hour, day, month, year;

void sdInit() {
	csOn(SD_CS);
	if (!SD.begin(-1)) {
		Serial.println("SD Card initialization failed!");
		sdStatus = false;
	}
	else {
		Serial.println("SD Card initialization Ok!");
		sdStatus = true;
	}
	csOff(SD_CS);
}

void epoch_to_date_time() {
	time_t epoch = time(nullptr);
	Serial.println(epoch);
	unsigned short days[4][12] = {
		{ 0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334 },  // 1970 -> февраль:28
		{ 365, 396, 424, 455, 485, 516, 546, 577, 608, 638, 669, 699 },  // 1971 -> февраль:28
		{ 730, 761, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065 },  // 1972 -> февраль:29
		{ 1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430 } }; // 1973 -> февраль:28

	second = epoch % 60; epoch /= 60;
	minute = epoch % 60; epoch /= 60;
	hour = epoch % 24; epoch /= 24;

	unsigned int tyears = epoch / (365 * 4 + 1) * 4; epoch %= 365 * 4 + 1;
	unsigned int tyear;
	for (tyear = 3; tyear>0; tyear--) {
		if (epoch >= days[tyear][0]) break;
	}

	unsigned int tmonth;
	for (tmonth = 11; tmonth>0; tmonth--) {
		if (epoch >= days[tyear][tmonth]) break;
	}

	year = ((tyears + tyear) - 30);
	Serial.println(year);
	if (year > 100) year = year%100;
	month = tmonth + 1;
	day = (epoch - days[tyear][tmonth]) + 1;
}

void sdWriteHeader() {
	if (year < 10) fileName = "0" + String(year) + "_";
	else fileName = String(year) + "_";
	if (month < 10) fileName += "0" + String(month) + "_";
	else fileName += String(month) + "_";
	if (day < 10) fileName += "0" + String(day) + ".log";
	else fileName += String(day) + ".log";
	if (RU) {
		switch (processMode.allow) {
			case 0: fileData = "Простой"; break;
			case 1: fileData = "Дистилляция"; break;
			case 2: fileData = "Ректификация"; break;
			case 3: fileData = "Затирание"; break;
			case 4: fileData = "Пивоварение"; break;
			case 5: fileData = ""; break;
			case 6: fileData = ""; break;
		}
	}
	else {
		switch (processMode.allow) {
			case 0: fileData = "Idle mode"; break;
			case 1: fileData = "Distillation"; break;
			case 2: fileData = "Reflux"; break;
			case 3: fileData = "Mashing"; break;
			case 4: fileData = "Brewing"; break;
			case 5: fileData = ""; break;
			case 6: fileData = ""; break;
		}
	}
	csOn(SD_CS);
	myFile = SD.open(fileName, FILE_WRITE);
	myFile.println(fileData);
	if (processMode.allow == 2 && RU) {
		switch (processMode.number) {
			case 0: fileData = "Алгоритм: ручной режим, только сигнализация"; break;
			case 1: fileData = "Алгоритм: Прима - головы по жидкости, тело по пару"; break;
			case 2: fileData = "Алгоритм: РК с отбором по пару"; break;
			case 3: fileData = "Алгоритм: РК с отбором по жидкости, 1 клапан отбора"; break;
			case 4: fileData = "Алгоритм: РК с отбором по жидкости, 2 клапана отбора"; break;
			case 5: fileData = "Алгоритм: БК регулировка охлаждением"; break;
			case 6: fileData = "Алгоритм: БК регулировка "; break;
		}
		myFile.println(fileData);
	}
	else if (processMode.allow == 2) {
		switch (processMode.number) {
			case 0: fileData = "Algorithm: manual, only alarm"; break;
			case 1: fileData = "Algorithm: Prima - heads by liquid, hearts by vapor"; break;
			case 2: fileData = "Algorithm: relfux, picking by vapor"; break;
			case 3: fileData = "Algorithm: reflux, picking by liqiud, one valve"; break;
			case 4: fileData = "Algorithm: reflux, picking by liqiud, two valves"; break;
			case 5: fileData = "Algorithm: BC, cooling management"; break;
			case 6: fileData = "Algorithm: BC, cooling management, heads by liqiud"; break;
		}
		myFile.println(fileData);
	}
	myFile.close();
	csOff(SD_CS);
}

void sdWriteLog() {

	if (hour < 10) fileData = "0" + String(hour) + ":";
	else fileData = String(hour) + ":";
	if (minute < 10) fileData += "0" + String(minute) + ":";
	else fileData += String(minute) + ":";
	if (second < 10) fileData += "0" + String(second) + " ";
	else fileData += String(second) + " ";
	fileData += String(nameProcessStep) + "\t";

	for (int i = 0; i < 8; i++) {
		if (temperatureSensor[i].member != 0) fileData += String(temperatureSensor[i].name) + " = " + String(temperatureSensor[i].data) + "\t";
	}
	if (RU) {
		fileData += "Давление = " + String(pressureSensor.data) + "\t";
		if (processMode.allow == 2) {
			fileData += "Уставка дельта = " + String(temperatureSensor[DS_Tube].allertValueIn) + "\t";
			fileData += "Уставка температура = " + String(temperatureSensor[DS_Tube].allertValue) + "\t";
		}
		fileData += "Мощность = " + String(power.heaterPower);
	}
	else {
		fileData += "Pressure = " + String(pressureSensor.data) + "\t";
		if (processMode.allow == 2) {
			fileData += "Set point delta = " + String(temperatureSensor[DS_Tube].allertValueIn) + "\t";
			fileData += "Set point temperature = " + String(temperatureSensor[DS_Tube].allertValue) + "\t";
		}
		fileData += "Power = " + String(power.heaterPower);
	}
	csOn(SD_CS);
	myFile = SD.open(fileName, FILE_WRITE);
	if (commandSD_en == true) {
		//myFile.println(fileData);
		myFile.println(commandWriteSD);
		commandSD_en = false;
	}
	else myFile.println(fileData);
	myFile.close();
	csOff(SD_CS);
}

void logfileLoop() {
	if (sdTimeWriteInterval <= millis() && endWriteSD == false) {
		sdTimeWriteInterval = SD_out_temp * 1000 + millis();
		if (processMode.allow > 0 && processMode.allow < 4 && sdStatus == true) {

			csOn(TFT_CS);
			tft.fillCircle(7, 236, 3, ILI9341_RED);
			csOff(TFT_CS);

			epoch_to_date_time();
			if (startWriteSD == true) {
				startWriteSD = false;
				sdWriteHeader();
			}
			sdWriteLog();
		}
		else if (commandSD_en == true && sdStatus == true) {
			csOn(TFT_CS);
			if (processMode.allow != 0) tft.fillCircle(7, 236, 3, ILI9341_RED);
			csOff(TFT_CS);

			csOn(SD_CS);
			myFile = SD.open(fileName, FILE_WRITE);
			myFile.println(commandWriteSD);
			if (processMode.allow == 0) myFile.println();
			commandSD_en = false;
			myFile.close();
			csOff(SD_CS);
		}
		// окончание записи на SD карту при дистилляции
		if (processMode.allow == 1) {
			if (endWriteSD == false && processMode.step == 4) {
				endWriteSD = true;
				csOn(SD_CS);
				myFile = SD.open(fileName, FILE_WRITE);
				myFile.println();
				myFile.close();
				csOff(SD_CS);
			}
		}
		// окончание записи на SD карту при ректификации
		else if (processMode.allow == 2) {
			if (endWriteSD == false && processMode.step == 4 && processMode.number == 0) {
				endWriteSD = true;
				csOn(SD_CS);
				myFile = SD.open(fileName, FILE_WRITE);
				myFile.println();
				myFile.close();
				csOff(SD_CS);
			}
			else if (endWriteSD == false && processMode.step == 7) {
				endWriteSD = true;
				csOn(SD_CS);
				myFile = SD.open(fileName, FILE_WRITE);
				myFile.println();
				myFile.close();
				csOff(SD_CS);
			}
		}
	}
}
