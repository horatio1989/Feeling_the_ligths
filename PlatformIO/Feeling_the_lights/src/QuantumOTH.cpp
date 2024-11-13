#include "QuantumOTH.h"
#include "QuantumINI.h"
#include "QuantumTFT.h"

#include <SensorQMI8658.hpp>
#include <Update.h>
#include <Wire.h>
#include <arduinoFFT.h>

//WiFiClient telnetClient;

//#define FFT_SPEED_OVER_PRECISION  // Define this to use reciprocal multiplication for division and some more speedups that might decrease precision
#define FFT_SQRT_APPROXIMATION      // Define this to use a low-precision square root approximation instead of the regular sqrt() call. This might only work for specific use cases, but is significantly faster. Only works for ArduinoFFT<float>.
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, samples, Fs);
double vReal[samples];
double vImag[samples];

SensorQMI8658 imu;
IMUdata acc;
IMUdata gyr;

float biasX = 0.0;
float biasY = 0.0;
float biasZ = 0.0;
void calIMU() {
  int calibrationSamples = 1000;
  float sumX = 0.0;
  float sumY = 0.0;
  float sumZ = 0.0;
  for (int i = 0; i < calibrationSamples; i++) {
    imu.getGyroscope(gyr.x, gyr.y, gyr.z);
    sumX += gyr.x;
    sumY += gyr.y;
    sumZ += gyr.z;
    delay(10);  // Pequeño retraso entre lecturas
  }
  biasX = sumX / calibrationSamples;
  biasY = sumY / calibrationSamples;
  biasZ = sumZ / calibrationSamples;
}

float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;
unsigned long lastTime = 0;
void readIMU() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;  // Convierte a segundos
  lastTime = currentTime;
  float alpha = 0.95;
  
  if (imu.getDataReady()) {
    imu.getGyroscope(gyr.x, gyr.y, gyr.z);
    imu.getAccelerometer(acc.x, acc.y, acc.z);

    float accAngleX = atan2(acc.y, acc.z) * 180 / PI;
    float accAngleY = atan2(-acc.x, acc.z) * 180 / PI;

    float gyroRateX = (gyr.x - biasX);
    float gyroRateY = (gyr.y - biasY);
    float gyroRateZ = (gyr.z - biasZ);

    // Ajuste dinámico de `alpha`: aumentar `alpha` cuando hay movimiento rápido
    float alpha = (abs(gyroRateX) > 0.1 || abs(gyroRateY) > 0.1) ? 0.98 : 0.90;
    angleX = alpha * (angleX + gyroRateX * deltaTime) + (1 - alpha) * accAngleX;
    angleY = alpha * (angleY + gyroRateY * deltaTime) + (1 - alpha) * accAngleY;
    angleZ += (gyr.z - biasZ) * deltaTime;

    /*angleX = fmod((angleX + 360), 360);
    angleY = fmod((angleY + 360), 360);
    angleZ = fmod((angleZ + 360), 360);*/
  
    Serial.print(">");
    Serial.print("X:");
    Serial.print(angleX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.print(angleY);
    Serial.print(",");
    Serial.print("Z:");
    Serial.print(angleZ);
    Serial.println(",");

    lastTime = millis();
  }
}

void progresscallback(size_t currSize, size_t totalSize) {
  Serial.printf("Update process at %d %% (%d of %d bytes)\r\n", (int)(100.0*currSize/totalSize), currSize, totalSize);
  sprite.createSprite(240, 240);
  sprite.setRotation(1);
  sprite.fillScreen(TFT_BLACK);
  sprite.drawString("Updating: " + String((int)(100.0*currSize/totalSize)), 20, 120, 4);
  sprite.pushSprite(0, 0);
}

void update_fw(bool fw2update) {
  if (fw2update) {
    sprite.createSprite(240, 240);
    sprite.setRotation(1);
    sprite.fillScreen(TFT_BLACK);
    sprite.drawString("Update fw", 17, 120, 4);
    sprite.pushSprite(0, 0);
    
    Serial.print("Current firmware version: ");
    Serial.println(FIRMWARE_VERSION);
    File firmware =  SD.open("/firmware.bin");
    Serial.println("Trying to update!");
    Update.onProgress(progresscallback);
    Update.begin(firmware.size(), U_FLASH);
    Update.writeStream(firmware);
    if (Update.end())
      Serial.println("Update finished!");
    else {
      Serial.println("Update error!");
      Serial.println(Update.getError());
    }
    firmware.close();

    if (SD.rename("/firmware.bin", "/firmware_used.bin")) {
      Serial.println("Firmware renamed successfully!");
      fw2update = 1;
    } else
      Serial.println("Firmware rename error!");
    delay(200);

    ESP.restart();
  } else
    Serial.println("No new firmware in SD");
}

void sampling() {
  for (int i = 0 ; i < samples; i++)
    vReal[i] = analogRead(MIC_DOUT_AOUT);
  memset(vImag, 0, sizeof(vImag));
}

void processAudio() {
  FFT.dcRemoval(vReal, samples);
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();
}

int getVolume(int micPin, int mic_avg) {
    int sum = 0;
    for (int i = 0; i < 400; i++) {
        //int val = analogRead(micPin);
        sum += abs(analogRead(micPin) - mic_avg);
        if (i % 45 == 0) delay(1);
    }
    /*Serial.print(">");
    Serial.print("sum:");
    Serial.print(constrain(map(sum / 400, 0, 4095, 0, 255), 0, 255));
    Serial.println(); // Writes \r\n*/
    return constrain(map(sum / 400, 0, 4095, 0, 255), 0, 255);
}