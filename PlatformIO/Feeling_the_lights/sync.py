import subprocess
import platform
import sys
import socket
import os

# Verificar el sistema operativo
os_type = platform.system()

if os_type == "Darwin":
    sh_script_path = "xxx/Quantum/PlatformIO/Feeling_the_lights/sync_mac.sh"
elif os_type == "Windows":
    computer_name = socket.gethostname()
    if computer_name == "nombre_computadora_1":
        sh_script_path = r"C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights\sync.bat"
    elif computer_name == "DESKTOP-AVPTRI1":
        sh_script_path = r"C:\Users\MrH\Dropbox\Proyectos\Kaleidoscopio\Feeling_the_ligths\PlatformIO\Feeling_the_lights\sync.bat"
    else:
        print(f"Computer name '{computer_name}' not recognized")
        sys.exit(1)
    # sh_script_path = r"C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights\sync.bat"
    # sh_script_path = r"C:\Users\MrH\Dropbox\Proyectos\Kaleidoscopio\Feeling_the_ligths\PlatformIO\Feeling_the_lights\sync.bat"
else:
    print(f"Unsupported OS: {os_type}")
    sys.exit(1)

# Ejecutar el script dependiendo del sistema operativo
try:
    if os_type == "Windows":
        subprocess.run([sh_script_path], check=True)
        process.communicate()  # Esperar a que termine la ejecución
    else:
        process = subprocess.Popen(["/bin/bash", sh_script_path])
        process.communicate()
    
    print("Sincronización completada.")
except subprocess.CalledProcessError as e:
    print(f"Error al ejecutar el script: {e}")
    sys.exit(1)

# Ruta completa del archivo .bat
#bat_file = r"C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights\sync.bat"
#sh_file
# Ejecutar el archivo .bat
#subprocess.call([bat_file])