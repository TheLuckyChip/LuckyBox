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
if /i "%InputFile%" == "1" goto stepSetCOM
if /i "%InputFile%" == "2" goto stepSetCOM
if /i "%InputFile%" == "3" goto stepSetCOM
if /i "%InputFile%" == "4" goto stepSetCOM
if /i "%InputFile%" == "Q" goto exitBat
goto reEnter

:stepSetCOM
echo Введите номер COM порта:
echo.
Set /p InputCom=

echo.
echo Введите скорость COM порта:
echo.
echo 1 - 115200
echo 2 - 256000
echo 3 - 512000
echo 4 - 921600
echo.
Set /p InputSpeed=
echo.
set "SpeedSet=115200"
if /i "%InputSpeed%" == "1" set "SpeedSet=115200"
if /i "%InputSpeed%" == "2" set "SpeedSet=256000"
if /i "%InputSpeed%" == "3" set "SpeedSet=512000"
if /i "%InputSpeed%" == "4" set "SpeedSet=921600"

:goProg
if /i "%InputFile%" == "1" goto progSketch
if /i "%InputFile%" == "2" goto progWeb
if /i "%InputFile%" == "3" goto ProgSketchAndWeb
if /i "%InputFile%" == "4" goto eraseProgSketchAndWeb
goto exitBat

:progSketch
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
goto endBat

:progWeb
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:progSketchAndWeb
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:eraseProgSketchAndWeb
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x0 -cz 0x400000 -ca 0x00000 -cf .\DataFiles\LuckyBox.ino.bin
.\DataFiles\esptool.exe -vv -cd nodemcu -cb "%SpeedSet%" -cp COM"%InputCom%" -ca 0x300000 -cf .\DataFiles\LuckyBox.spiffs.bin
goto endBat

:endBat
echo.
pause

:exitBat