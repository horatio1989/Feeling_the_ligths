#ifndef QUANTUMOTH_H
#define QUANTUMOTH_H

#include "QuantumDEF.h"
#include "QuantumINI.h"
#include "QuantumTFT.h"

#include <SensorQMI8658.hpp>
#include <Update.h>
#include <Wire.h>

#define USE_WIRE
#define SENSOR_SDA  6
#define SENSOR_SCL  7
#define SENSOR_IRQ  47 // INT1 = 47, INT2 = 48

extern SensorQMI8658 imu;

extern float angleX, angleY, angleZ;

// FFT Definitions
// Resolution of frequencies is:   res  = Fs/samples
// Number of frequencies bins are: n_f  = samples/2 
// Max frequency will be:          mx_f = Fs/2-resolution
// Asumming:
// Fs      = 20 kHz
// samples = 512
// ==> res = ~40 Hz that means each component of the vector vReal (up to samples/2) will correspond to the intensity for the bin
const double  Fs        = 20000;
const int     samples   = 512;   // samples should be power of 2
extern double vReal[samples];
extern double vImag[samples];

void calIMU();
void readIMU();
void progresscallback(size_t currSize, size_t totalSize);
void update_fw(bool fw2update);
void sampling();
void processAudio();
int getVolume(int micPin, int mic_avg);

template <typename T>
void println_telnet(T var) {
  if (!telnetClient || !telnetClient.connected())
    telnetClient = telnetServer.accept();
  if (telnetClient && telnetClient.connected()) {
      telnetClient.println(var);  // Envía el mensaje por Telnet
      delay(10);  // Controla la frecuencia de envío
  }
}

template <typename A>
void print_tft(A var, int x, int y, char format, int del) {
  char versionText[80];
  switch (format) {
    case 'f':
      snprintf(versionText, sizeof(versionText), "%.2f", var);
      break;
    case 'i':
      snprintf(versionText, sizeof(versionText), "%d", var);
      break;
    case 's':
      snprintf(versionText, sizeof(versionText), "%s", var);
      break;
  }
  sprite.createSprite(240, 240);
  sprite.fillScreen(TFT_BLACK);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK); 
  sprite.setTextSize(2);
  sprite.drawString(versionText, x, y);
  sprite.pushSprite(0, 0);
  delay(del);
}

#endif