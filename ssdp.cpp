#include "ssdp.h"
#include "setting.h"
#include <ESP32SSDP.h>

void initSSDP(void)
{
	// SSDP дескриптор
	HTTP.on("/description.xml", HTTP_GET, []()
	{
		SSDP.schema(HTTP.client());
	});
	//Если версия  2.0.0 закомментируйте следующую строчку
	SSDP.setDeviceType("upnp:rootdevice");
	SSDP.setSchemaURL("description.xml");
	SSDP.setHTTPPort(80);
	SSDP.setName(SSDP_Name);
	SSDP.setSerialNumber("00000000001");
	SSDP.setURL("/");
	SSDP.setModelName("LuckyBox");
	SSDP.setModelNumber("000000000001");
	SSDP.setModelURL("https://luckycenter.ru");
	SSDP.setManufacturer("Счастливчик");
	SSDP.setManufacturerURL("https://luckycenter.ru");
	SSDP.begin();
}

