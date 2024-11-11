#include <Arduino.h>

#include "QuantumDEF.h"

#include "QuantumINI.h"
#include "QuantumOTH.h"
#include "QuantumLED.h"
#include "QuantumTFT.h"

#include "Core1_LED.h"
#include "Core2_TFT.h"

TaskHandle_t Task_LEDstrip = NULL;
TaskHandle_t Task_TFT      = NULL;

int mic_avg = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Current firmware version: ");
  Serial.println(FIRMWARE_VERSION);
  delay(200);

  wifi_init();

  ota_init();
  
  tft_init(5);

  mic_avg = mic_init(MIC_DOUT_AOUT);
  //Serial.println(mic_avg);

  pinMode(SW_BOOT, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), isr, RISING);

  // Multi-Core Tasks initialization
  xTaskCreatePinnedToCore(
      Task_LEDstrip_code,  // Function to implement the task
      "Task_LEDstrip",    // Name of the task
      10000,      // Stack size in words
      NULL,       // Task input parameter
      1,          // Priority of the task
      &Task_LEDstrip,     // Task handle.
      0);         // Core where the task should run
  Serial.println("Task on core 0 ready");
  
  xTaskCreatePinnedToCore(
      Task_TFT_code,  // Function to implement the task
      "Task_TFT",    // Name of the task
      10000,      // Stack size in words
      NULL,       // Task input parameter
      2,          // Priority of the task
      &Task_TFT,     // Task handle.
      1);         // Core where the task should run
  Serial.println("Task on core 1 ready");
}

void loop() {}
