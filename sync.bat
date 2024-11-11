@echo off

set "sourcePath=%cd%"
set "destPath=C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\Arduino\Feeling_the_lights"  REM Cambia esto a la ruta de tu proyecto de Arduino IDE

xcopy "%sourcePath%\src" "%destPath%\" /E /Y
xcopy "%sourcePath%\include" "%destPath%\" /E /Y

REM Renombrar main.cpp a Feeling_the_lights_ino.ino en el destino para que ande
if exist "%destPath%\Feeling_the_lights.cpp" (
    ren "%destPath%\Feeling_the_lights.cpp" "Feeling_the_lights.ino"
)

echo Sincronización completada.