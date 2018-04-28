#include "fs_config.h"
#include "setting.h"
#include "user_config.h"

#ifdef ESP8266
#include <FS.h>

#else
#include <SPIFFS.h>

#endif

// инициализация FFS
void initFS(void)
{
  if(!SPIFFS.begin()) {
	Serial.println("SPIFFS Mount Failed");
  return;
  }
  #ifdef ESP8266
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {    
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
  }
  #else
    listDir("/", 0);
  #endif
    Serial.printf("\n");
 
	//HTTP страницы дл¤ работы с FFS
	//list directory
	HTTP.on("/list", HTTP_GET, handleFileList);
	//загрузка редактора editor
	HTTP.on("/edit", HTTP_GET, []()
	{
		if (!handleFileRead("/edit.htm")) HTTP.send(404, "text/plain", "FileNotFound");
	});
	//—оздание файла
	HTTP.on("/edit", HTTP_PUT, handleFileCreate);
	//”даление файла
	HTTP.on("/edit", HTTP_DELETE, handleFileDelete);
	//first callback is called after the request has ended with all parsed arguments
	//second callback handles file uploads at that location
	HTTP.on("/edit", HTTP_POST, []()
	{
		HTTP.send(200, "text/plain", "");
	}, handleFileUpload);
	//called when the url is not defined here
	//use it to load content from SPIFFS
	HTTP.onNotFound([]()
	{
		if (!handleFileRead(HTTP.uri()))
			HTTP.send(404, "text/plain", "FileNotFound");
	});
}

// Здесь функции дл¤ работы с файловой системой
String getContentType(String filename)
{
	if (HTTP.hasArg("download")) return "application/octet-stream";
	else if (filename.endsWith(".htm")) return "text/html";
	else if (filename.endsWith(".html")) return "text/html";
	else if (filename.endsWith(".json")) return "application/json";
	else if (filename.endsWith(".css")) return "text/css";
	else if (filename.endsWith(".js")) return "application/javascript";
	else if (filename.endsWith(".png")) return "image/png";
	else if (filename.endsWith(".gif")) return "image/gif";
	else if (filename.endsWith(".jpg")) return "image/jpeg";
	else if (filename.endsWith(".ico")) return "image/x-icon";
	else if (filename.endsWith(".xml")) return "text/xml";
	else if (filename.endsWith(".pdf")) return "application/x-pdf";
	else if (filename.endsWith(".zip")) return "application/x-zip";
	else if (filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}

bool handleFileRead(String path)
{
	if (path.endsWith("/")) path += "index.htm";
	String contentType = getContentType(path);
	String pathWithGz = path + ".gz";
	if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
	{
		if (SPIFFS.exists(pathWithGz))
			path += ".gz";
		File file = SPIFFS.open(path, "r");
		size_t sent = HTTP.streamFile(file, contentType);
		file.close();
		return true;
	}
	return false;
}

void handleFileUpload()
{
	if (HTTP.uri() != "/edit") return;
	HTTPUpload& upload = HTTP.upload();
	if (upload.status == UPLOAD_FILE_START)
	{
		String filename = upload.filename;
		if (!filename.startsWith("/")) filename = "/" + filename;
		fsUploadFile = SPIFFS.open(filename, "w");
		filename = String();
	}
	else if (upload.status == UPLOAD_FILE_WRITE)
	{
		//DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
		if (fsUploadFile)
			fsUploadFile.write(upload.buf, upload.currentSize);
	}
	else if (upload.status == UPLOAD_FILE_END)
	{
		if (fsUploadFile)
			fsUploadFile.close();
	}
}

void handleFileDelete()
{
	if (HTTP.args() == 0) return HTTP.send(500, "text/plain", "BAD ARGS");
	String path = HTTP.arg(0);
	if (path == "/")
		return HTTP.send(500, "text/plain", "BAD PATH");
	if (!SPIFFS.exists(path))
		return HTTP.send(404, "text/plain", "FileNotFound");
	SPIFFS.remove(path);
	HTTP.send(200, "text/plain", "");
	path = String();
}

void handleFileCreate()
{
	if (HTTP.args() == 0)
		return HTTP.send(500, "text/plain", "BAD ARGS");
	String path = HTTP.arg(0);
	if (path == "/")
		return HTTP.send(500, "text/plain", "BAD PATH");
	if (SPIFFS.exists(path))
		return HTTP.send(500, "text/plain", "FILE EXISTS");
	File file = SPIFFS.open(path, "w");
	if (file)
		file.close();
	else
		return HTTP.send(500, "text/plain", "CREATE FAILED");
	HTTP.send(200, "text/plain", "");
	path = String();

}

void returnFail(String msg) {
  HTTP.send(500, "text/plain", msg + "\r\n");
}



#ifdef ESP8266
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

void handleFileList() {
  if (!HTTP.hasArg("dir")) {
    returnFail("BAD ARGS");
    return;
  }
  String path = HTTP.arg("dir");
  Serial.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();
  
  String output = "[";
  while(dir.next()){
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir)?"dir":"file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }
  output += "]";
  HTTP.send(200, "text/json", output);
}

#else
void handleFileList() {
  if(!HTTP.hasArg("dir")) {
    returnFail("BAD ARGS");
    return;
  }
  String path = HTTP.arg("dir");
  if(path != "/" && !SPIFFS.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  File dir = SPIFFS.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    returnFail("NOT DIR");
    return;
  }
  dir.rewindDirectory();

  String output = "[";
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
    break;

    if (cnt > 0)
      output += ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    // Ignore '/' prefix
    output += entry.name()+1;
    output += "\"";
    output += "}";
    entry.close();
  }
  output += "]";
  HTTP.send(200, "text/json", output);
  dir.close();
}

void listDir(const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);
  
  File root = SPIFFS.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
#endif


