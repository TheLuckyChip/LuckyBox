//   Проект центра открытого проектирования у Счастливчика https://LuckyCenter.ru
#ifndef _FS_h
#define _FS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "setting.h"
#include "user_config.h"

extern void initFS(void);
extern String getContentType(String filename);
extern bool handleFileRead(String path);
extern void handleFileUpload();
extern void handleFileDelete();
extern void handleFileCreate();
extern void handleFileList();
#ifdef ESP8266
String formatBytes(size_t bytes);
#else
void listDir(const char * dirname, uint8_t levels);
#endif

#endif
