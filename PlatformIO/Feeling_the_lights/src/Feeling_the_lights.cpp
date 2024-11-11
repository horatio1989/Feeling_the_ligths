#include <Arduino.h>

#include "QuantumDEF.h"

#include "QuantumINI.h"
#include "QuantumOTH.h"
#include "QuantumLED.h"
#include "QuantumTFT.h"

#include "Core1_LED.h"
#include "Core2_TFT.h"

TaskHandle_t h_Task_LED = NULL;
TaskHandle_t h_Task_TFT = NULL;

int mic_avg = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Current firmware version: ");
  Serial.println(FIRMWARE_VERSION);
  delay(200);

  wifi_init();

  ota_init();
  
  tft_init(5);

  imu_init();

  mic_avg = mic_init(MIC_DOUT_AOUT);

  pinMode(SW_BOOT, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN), isr, CHANGE);

  // Multi-Core Tasks initialization
  xTaskCreatePinnedToCore(
      Task_LED,       // Function to implement the task
      "LED_Task",     // Name of the task
      10000,          // Stack size in 32-bit words
      NULL,           // Task input parameter
      1,              // Priority of the task (1 = lower, 2 = higher)
      &h_Task_LED,    // Task handle.
      0);             // Core where the task should run
  Serial.println("LED Task on core 0 ready");
  Serial.print("Stack LED libre: ");
  Serial.print((int)(uxTaskGetStackHighWaterMark(h_Task_LED)/100));
  Serial.println("%");
  Serial.println("********************");
  
  xTaskCreatePinnedToCore(
      Task_TFT,       // Function to implement the task
      "TFT_Task",     // Name of the task
      10000,          // Stack size in 32-bit words
      NULL,           // Task input parameter
      2,              // Priority of the task (1 = lower, 2 = higher)
      &h_Task_TFT,    // Task handle.
      1);             // Core where the task should run
  Serial.println("TFT Task on core 1 ready");
  Serial.print("Stack TFT libre: ");
  Serial.print((int)(uxTaskGetStackHighWaterMark(h_Task_TFT)/100));
  Serial.println("%");
  Serial.println("********************");
}

void loop() {}
