@Echo Off
:reEnter
echo.
echo Выберите соответствующий пункт:
echo.
echo 1 - Обновляем только программу.
echo 2 - Обновляем только Web интерфейс.
echo 3 - Обновляем программу и Web интерфейс.
echo 4 - Стираем всю память модуля. Обновляем программу и Web интерфейс.
echo Q - Выход.
echo.
set /p InputFile=
echo.
IF /i "%InputFile%" == "1" goto progSketch
IF /i "%InputFile%" == "2" goto progWeb
IF /i "%InputFile%" == "3" goto ProgSketchAndWeb
IF /i "%InputFile%" == "4" goto eraseProgSketchAndWeb
IF /i "%InputFile%" == "Q" goto exitBat

goto reEnter

:progSketch
echo Введите номер COM порта:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
goto endBat

:progWeb
echo Введите номер COM порта:
echo.
Set /p $InputCom=
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:progSketchAndWeb
echo Введите номер COM порта:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:eraseProgSketchAndWeb
echo Введите номер COM порта:
echo.
Set /p $InputCom=    
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x0 -cz 0x400000 -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
.\DataFiles\esptool.exe -vv -cd nodemcu -cb 921600 -cp COM"%$InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:endBat
echo.
pause

:exitBat