Proyecto Feeling the Lights en Platform IO con VS Code.

Actualmente anda y compila con un json personalizado para la placa Waveshare ESP32-S3-LCD-1.28 (https://www.waveshare.com/wiki/ESP32-S3-LCD-1.28?Amazon)
Esquema de particion personalizado sin SPIFFS ya que tenemos acceso a una micro SD de 4Gb

- Configurado actualizacion de firmware via archivo *.bin en la SD o a traves de OTA
- Configurado servidor Telnet para que el Tio Entraña pueda debugear en su MAC
- Uso de ambos nucleos
- Funcion personalizada para el Tio Entraña para que pueda debugear y escribir en consola con telnet:
```
println_telnet(<variable del tipo que quieras>);
```

Cada nucleo del ESP32 se encarga de una tarea especifica LED y TFT y se encuentran en dos *.cpp diferentes

El proyecto se estructura:
- QuantumDEF: todas las definiciones de pines y demas
- QuantumINI: todas las funciones de inizializacion
- QuantumLED: todas las funciones relacionadas con LED para su funcionamiento
- QuantumTFT: todas las funciones relacionadas con TFT para su funcionamiento
- TFT_eff: funciones con efectos para el TFT
- LED_eff: funciones con efectos para los LED
- Core1_LED: Codigo de la tarea del nucleo para el LED
- Core2_TFT: Codigo de la tarea del nucleo para el TFT
- QuantumOTH: Funciones extras que no encajan en previas categorias
- main: contiene el void setup(){} con las inizializaciones

La idea es que desde cada Tarea de los nucleos se llama a la funcion correspondiente dentro de LED_eff o TFT_eff. Eso implica que los distintos efectos deben ser creados y configurados dentro de esos dos archivos.

A la hora de setear el proyecto por primera vez, es necesario descargar las siguientes librerias de en Platform IO:
- 	TFT_eSPI
- 	arduinoFFT
- 	SensorLib
- 	Adafruit GFX Library
- 	AnimatedGIF
- 	Adafruit NeoPixel
Una vez instaladas esas librerias, hay que reemplazar el archivo "User_Setup.h" que se encuentra en ".pio/libdeps/TFT_eSPI" por el "User_Setup.h" que se encuentra en el proyecto (en ninguna carpeta en particular"
En donde se encuentre la carpeta ".platformio" en tu MAC, tenes que pegar la carpeta "waveshare_esp32s3_touch_lcd_128"

El resto de las librerias utilizadas (como SD, SPI, etc.) vienen dentro del paquete de ESP32 o son las librerias creadas por nosotros

En caso de usar OTA para actualizar el firmware dentro del archivo platformio.ini comentar la siguiente linea:
```
upload_port   = COM21
monitor_port  = COM21
monitor_rts   = 0
monitor_dtr   = 0
monitor_speed = 115200
```
Y descomentar las siguientes, agregando el IP correspondiente:
```
;upload_protocol = espota 
;upload_port = <IP> 
;upload_flags = 
;    --port=3232 
```
Ademas, hay que configurar el puerto en VS Code (en la barra inferior, en el ejemplo donde dice "COM21")
![image](https://github.com/user-attachments/assets/aeb61533-ffef-4ed7-a84a-a2152dc2df1f)

La estructura del proyecto es:
- Arduino
-- Feeling_the_lights --> Proyecto en Arduino
- PlatformIO
-- Extra Files --> files necesarias para agregar del proyecto en Platform IO
-- Feeling_the_lights --> Proyecto en PlatformIO
- Python
-- Original --> tus scripts en Python
-- To convert --> scripts que ya estan listos para convertir a C
-- Converted --> scripts que ya estan convertidos a C esperando ser includios en platformIO

FINALMENTE: recomiendo mucho usar VS Code + Platform IO. PERO si sos tan vago de no querer cambiar y quere seguir usando Arduino IDE... Hace lo siguiente:
- Abri el archivo "Feeling_the_lights_ino.ino" que se encuentra en la carpeta "Feeling_the_lights_ino". mas facil no te lo puedo haber dejado! PERO no pushees ningun cambio que hagas ahi, porque yo trabajo en Platform IO y automaticamente cuando commiteo, se copian todos los archivos a esa carpeta, con lo cual perderias cualquier cosa que hayas hecho. De prefencia, lo que tengas de nuevo, pegalo en el archivo correspondiente cpp y h dentro de la estructura de proyecto de Platform IO
- En Arduino IDE configura todo para el ESP32
    - CDC on Boot: Disable
    - CPU Freq: 240MHz
    - Core Debug: None
    - USB DFU: Disable
    - Erase all flash: Enable
    - Events run on: Core1
    - Flash Mode: QIO 80MHz
    - Flash Size: 16Mb
    - JTAG: Disable
    - Arduino runs on: Core 1
    - USB Firmware: Disable
    - Partition scheme: Custom 16MB
    - PSRAM: QSPI PSRAM
    - Upload mode: UART0
    - Upload Speed: 921600
    - USB Mode: Hardware CDC and JTAG
    - Zigbee: Disable
    - Programmer: esptool

- La primera vez tenes que configurar para que Arduino encuentre es esquema de particion que genere. Para eso segui estos pasos:
    - busca en MAC donde se encuentra instalado los archivos del esp32 para Arduino.
    - Luego en el archivo "platform.txt" reemplazar la linea "build.custom_partitions=partitions" por build.custom_partitions=custom_16MB
    - En el archivo boards.txt agregar estas tres lineas:
```
esp32s3.menu.PartitionScheme.custom_16MB=Custom 16MB
esp32s3.menu.PartitionScheme.custom_16MB.build.partitions=partitions/custom_16MB.csv
esp32s3.menu.PartitionScheme.custom_16MB.upload.maximum_size=16777216
```

- Finalmente copiar el archivo de particiones "custom_16MB.csv" que esta junto al archivo ino en la carpeta ...\variants\esp32s3 y en la carpeta ...\tools\partitions

Con eso deberia andar
    
