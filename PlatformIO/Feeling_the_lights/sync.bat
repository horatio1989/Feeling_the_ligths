@echo off

if "%COMPUTERNAME%" == "C0256-S2364" (
    set "sourcePath=C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights"
    set "destPath=C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\Arduino\Feeling_the_lights"  REM Cambia esto a la ruta de tu proyecto de Arduino IDE
) else if "%COMPUTERNAME%" == "DESKTOP-AVPTRI1" (
    set "sourcePath=C:\Users\MrH\Dropbox\Proyectos\Kaleidoscopio\Feeling_the_ligths\PlatformIO\Feeling_the_lights"
    set "destPath=C:\Users\MrH\Dropbox\Proyectos\Kaleidoscopio\Feeling_the_ligths\Arduino\Feeling_the_lights"  REM Cambia esto a la ruta de tu proyecto de Arduino IDE
) else (
    echo Computadora no reconocida
    exit /b
)

xcopy "%sourcePath%\src" "%destPath%\" /E /Y
xcopy "%sourcePath%\include" "%destPath%\" /E /Y

REM Renombrar main.cpp a Feeling_the_lights_ino.ino en el destino para que ande
if exist "%destPath%\Feeling_the_lights.cpp" (
    ren "%destPath%\Feeling_the_lights.cpp" "Feeling_the_lights.ino"
)

echo Sincronización completada.