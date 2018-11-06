@Echo Off
:reEnter
echo.
echo Programm sketch or web file:
echo 1 - Update only sketch.
echo 2 - Update only sketch and all flash erase.
echo 3 - Update only web files.
echo 4 - All flash erase. Update sketch and web files.
echo Q - Exit
echo.
set /p InputFile=
echo.
IF /i "%InputFile%" == "1" goto progSketch
IF /i "%InputFile%" == "2" goto eraseProgSketch
IF /i "%InputFile%" == "3" goto progWeb
IF /i "%InputFile%" == "4" goto eraseProgSketchAndWeb
IF /i "%InputFile%" == "Q" goto exitBat

goto reEnter

:progSketch
echo Enter COM-port number:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
goto endBat

:eraseProgSketch
echo Enter COM-port number:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x0 -cz 0x400000 -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
goto endBat

:progWeb
echo Enter COM-port number:
echo.
Set /p $InputCom=
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:eraseProgSketchAndWeb
echo Enter COM-port number:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x0 -cz 0x400000 -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:endBat
echo.
pause

:exitBat