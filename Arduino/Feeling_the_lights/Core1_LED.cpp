#include <Arduino.h>

#include "Core1_LED.h"

#include "QuantumDEF.h"

void Task_LEDstrip_code(void * parameter) {
  for( ;; ) {
    ArduinoOTA.handle();
    int volume = getVolume(MIC_DOUT_AOUT, mic_avg);
    int volume_mapped = map(volume, 0, 200, 0, 100);
    double intensidad = 0.0;
    switch (counter) {
      case 1:
        runEffect1(volume_mapped);
        break;
      case 2:
        runEffect2(volume_mapped);
        break;
      case 3:
        runEffect3(volume_mapped);
        break;
      case 4:
        runEffect4(volume_mapped);
        break;
      case 5:
        runEffect4(volume_mapped);
        break;
      case 6:
        intensidad = efectos_bajos(3, 0.5);
        break;
      default:
        strip.clear();
        break;
    }
    strip.show();
  }
  vTaskDelete(NULL);
}