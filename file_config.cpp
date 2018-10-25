#include "file_config.h"

// Загрузка данных сохраненных в EEPROM
bool loadConfig()
{
	// считаем конфигурацию WiFi
	uint16_t index = 1700;
	uint8_t	cnt = 0;
	SSDP_Name = "";
	while (1) {
		if (EEPROM.read(index) != 255 && EEPROM.read(index) != 0 && cnt < 60) {
			SSDP_Name += " ";
			SSDP_Name[cnt] = EEPROM.read(index);
			index++;
			cnt++;
		}
		else {
			if (cnt <= 2) SSDP_Name = "LuckyBox";
			break;
		}
	}

	index = 1760;
	cnt = 0;
	_ssid = "";
	while (1) {
		if (EEPROM.read(index) != 255 && EEPROM.read(index) != 0 && cnt < 60) {
			_ssid += " ";
			_ssid[cnt] = EEPROM.read(index);
			index++;
			cnt++;
		}
		else {
			if (cnt <= 2) _ssid = "SSID";
			break;
		}
	}

	index = 1820;
	cnt = 0;
	_ssidAP = "";
	while (1) {
		if (EEPROM.read(index) != 255 && EEPROM.read(index) != 0 && cnt < 60) {
			_ssidAP += " ";
			_ssidAP[cnt] = EEPROM.read(index);
			index++;
			cnt++;
		}
		else {
			if (cnt <= 2) _ssidAP = "LuckyBox";
			break;
		}
	}

	index = 1900;
	cnt = 0;
	_password = "";
	while (1) {
		if (EEPROM.read(index) != 255 && EEPROM.read(index) != 0 && cnt < 32) {
			_password += " ";
			_password[cnt] = EEPROM.read(index);
			index++;
			cnt++;
		}
		else {
			if (cnt < 8) _password = "PASSWORD";
			break;
		}
	}

	index = 1940;
	cnt = 0;
	_passwordAP = "";
	while (1) {
		if (EEPROM.read(index) != 255 && EEPROM.read(index) != 0 && cnt < 32) {
			_passwordAP += " ";
			_passwordAP[cnt] = EEPROM.read(index);
			index++;
			cnt++;
		}
		else {
			if (cnt < 8) _passwordAP = "12345678";
			break;
		}
	}
	Serial.print("SSDP Name = "); Serial.println(SSDP_Name);
	Serial.print("SSID = "); Serial.print(_ssid);
	Serial.print("    password = "); Serial.println(_password);
	Serial.print("SSIDAP = "); Serial.print(_ssidAP); Serial.print(" - ");
	String addrMac = WiFi.softAPmacAddress();
	String addrMacMod = "            ";
	addrMacMod[0] = addrMac[0];	addrMacMod[1] = addrMac[1];
	addrMacMod[2] = addrMac[3];	addrMacMod[3] = addrMac[4];
	addrMacMod[4] = addrMac[6];	addrMacMod[5] = addrMac[7];
	addrMacMod[6] = addrMac[9];	addrMacMod[7] = addrMac[10];
	addrMacMod[8] = addrMac[12]; addrMacMod[9] = addrMac[13];
	addrMacMod[10] = addrMac[15]; addrMacMod[11] = addrMac[16];
	Serial.print(addrMacMod);
	Serial.print("    passwordAP = "); Serial.println(_passwordAP);
	Serial.println();
}
