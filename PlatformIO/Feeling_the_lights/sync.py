import subprocess
import platform
import sys

# Verificar el sistema operativo
os_type = platform.system()

if os_type == "Darwin":
    sh_script_path = "xxx/Quantum/PlatformIO/Feeling_the_lights/sync_mac.sh"
elif os_type == "Windows":
    # sh_script_path = r"C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights\sync.bat"
    sh_script_path = r"C:\Users\MrH\Dropbox\Proyectos\Kaleidoscopio\Feeling_the_ligths\PlatformIO\Feeling_the_lights\sync.bat"
else:
    print(f"Unsupported OS: {os_type}")
    sys.exit(1)

# Ejecutar el script dependiendo del sistema operativo
try:
    if os_type == "Windows":
        subprocess.run([sh_script_path], check=True, shell=True)
    else:
        subprocess.run(["/bin/bash", sh_script_path], check=True)
    
    print("Sincronización completada.")
except subprocess.CalledProcessError as e:
    print(f"Error al ejecutar el script: {e}")
    sys.exit(1)

# Ruta completa del archivo .bat
#bat_file = r"C:\Users\didjw296\OneDrive - Dentsply Sirona\Dokumente\PlatformIO\Projects\Quantum\PlatformIO\Feeling_the_lights\sync.bat"
#sh_file
# Ejecutar el archivo .bat
#subprocess.call([bat_file])