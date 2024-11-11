@echo off

sourcePath="xxx/Quantum/PlatformIO/Feeling_the_lights"
destPath="xxx/Quantum/Arduino/Feeling_the_lights"

cp -r "$sourcePath/src" "$destPath/"
cp -r "$sourcePath/include" "$destPath/"

# Renombrar 'Feeling_the_lights.cpp' a 'Feeling_the_lights.ino' en el destino
if [ -f "$destPath/Feeling_the_lights.cpp" ]; then
    mv "$destPath/Feeling_the_lights.cpp" "$destPath/Feeling_the_lights.ino"
fi

echo "Sincronización completada."