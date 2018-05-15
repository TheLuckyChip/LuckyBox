#include "ssdp.h"
#include "setting.h"

#ifdef ESP8266
#include <ESP8266SSDP.h> 

#else
#include <ESP32SSDP.h>

#endif

void initSSDP(void)
{
	// SSDP ����������
	HTTP.on("/description.xml", HTTP_GET, []()
	{
		SSDP.schema(HTTP.client());
	});
	//���� ������  2.0.0 ��������������� ��������� �������
	SSDP.setDeviceType("upnp:rootdevice");
	SSDP.setSchemaURL("description.xml");
	SSDP.setHTTPPort(80);
	SSDP.setName(SSDP_Name);
	SSDP.setSerialNumber("00000000001");
	SSDP.setURL("/");
	SSDP.setModelName("LuckyBox");
	SSDP.setModelNumber("000000000001");
	SSDP.setModelURL("https://luckycenter.ru");
	SSDP.setManufacturer("�����������");
	SSDP.setManufacturerURL("https://luckycenter.ru");
	SSDP.begin();
}

