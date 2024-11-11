#ifndef QUANTUMINI_H
#define QUANTUMINI_H

#include <Arduino.h>

#include "QuantumDEF.h"
#include "QuantumLED.h"

#include <ArduinoOTA.h>
#include <SPI.h>
#include <WiFi.h>

#include <ESPmDNS.h>
#include <NetworkUdp.h>

extern WiFiServer telnetServer;
extern WiFiClient telnetClient;

extern SPIClass spiSD;

extern const char* ssid[];
extern const char* password[];

extern void update_fw(bool fw2update);

extern Adafruit_NeoPixel strip;

extern int counter;
extern bool flag;

void isr();
void wifi_init();
void ota_init();
void sd_init();
void tft_init(int blk_int);
void led_init();
void imu_init();
int  mic_init(int micPin);

#endif