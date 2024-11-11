#include "LED_eff.h"
#include "QuantumLED.h"

const int tailIntensities3[]     = {70, 20, 10};
const int tailIntensities4[]     = {70, 20, 10, 5};
const int tailIntensities5[]     = {70, 20, 10, 5, 5};
const int tailIntensities_eff2[] = {70, 70};

uint32_t ledColors[N_LED]     = {0};
uint32_t headPositions[N_LED] = {0};

void runEffect1(int volume_mapped) {
  // static Quantum<3, N_LED> Quantum_1(strip.Color(142, 69, 133), 2, tailIntensities3, 1, 1);
  // static Quantum<3, N_LED> Quantum_2(strip.Color(128, 0, 32), 4, tailIntensities3, 1, 2);
  // static Quantum<3, N_LED> Quantum_3(strip.Color(0, 0, 128), 6, tailIntensities3, 1, 3);

  static Quantum<3, N_LED> Quantum_1(strip.Color(255, 0, 0), 2, tailIntensities3, 1, 1);
  static Quantum<3, N_LED> Quantum_2(strip.Color(0, 255, 0), 4, tailIntensities3, 1, 2);
  static Quantum<3, N_LED> Quantum_3(strip.Color(0, 0, 255), 6, tailIntensities3, 1, 3);

  Quantum_1.move();
  Quantum_2.move();
  Quantum_3.move();

  uint32_t tempLedColors[N_LED] = {0};
  uint32_t tempHeadPositions[N_LED] = {0};

  int volume_factor = 6;
  Quantum_1.changeVelocity(2*int((volume_mapped)/volume_factor));
  Quantum_2.changeVelocity(3*int((volume_mapped)/volume_factor));
  Quantum_3.changeVelocity(4*int((volume_mapped)/volume_factor));

  if (true) {
    Quantum_2.changeSpin(1);
    Quantum_3.changeSpin(1);
  } else {
    Quantum_2.changeSpin(2);
    Quantum_3.changeSpin(3);
  }; // change spin is not working

  Quantum_1.display(tempLedColors, tempHeadPositions, 100);
  Quantum_2.display(tempLedColors, tempHeadPositions, 100);
  Quantum_3.display(tempLedColors, tempHeadPositions, 100);

  for (int i = 0; i < N_LED; i++) {
    ledColors[i] = tempLedColors[i];
    headPositions[i] = tempHeadPositions[i];
    strip.setPixelColor(i, ledColors[i]);
  }
}

void runEffect2(int volume_mapped) { //Add a shift to start with a white and create colors afterwards
  static Quantum<2, N_LED> Quantum_1(strip.Color(0, 0, 255), 5, tailIntensities_eff2, 1, 1);
  static Quantum<2, N_LED> Quantum_2(strip.Color(255, 0, 0), 5, tailIntensities_eff2, 1, 2);
  static Quantum<2, N_LED> Quantum_3(strip.Color(0, 255, 0), 5, tailIntensities_eff2, 1, 3);

  Quantum_1.move();
  Quantum_2.move();
  Quantum_3.move();

  Quantum_1.setShift(max(0, int(volume_mapped/8)));
  Quantum_2.setShift(min(0, -int(volume_mapped/8)));

  uint32_t tempLedColors[N_LED] = {0};
  uint32_t tempHeadPositions[N_LED] = {0};

  //Quantum_1.addShift(2);

  Quantum_1.display(tempLedColors, tempHeadPositions, 100);
  Quantum_2.display(tempLedColors, tempHeadPositions, 100);
  Quantum_3.display(tempLedColors, tempHeadPositions, 100);


  for (int i = 0; i < N_LED; i++) {
    ledColors[i] = tempLedColors[i];
    headPositions[i] = tempHeadPositions[i];
    strip.setPixelColor(i, ledColors[i]);
  }
}

void runEffect3(int volume_mapped) {
  // Define thresholds for each Quantum instance
  const int threshold_1 = 20;
  const int threshold_2 = 35;
  const int threshold_3 = 50;

  static Quantum<4, N_LED> Quantum_1(strip.Color(0, 0, 155), 3, tailIntensities4, 1, 1);
  static Quantum<4, N_LED> Quantum_2(strip.Color(0, 155, 0), 5, tailIntensities4, -1, 1);
  static Quantum<4, N_LED> Quantum_3(strip.Color(155, 0, 0), 4, tailIntensities4, 1, 2);
  static Quantum<4, N_LED> Quantum_4(strip.Color(100, 100, 100), 7, tailIntensities4, -1, 2);

  handleCollisions(Quantum_1, Quantum_2);
  handleCollisions(Quantum_2, Quantum_3);
  handleCollisions(Quantum_1, Quantum_3);
  handleCollisions(Quantum_1, Quantum_2);
  handleCollisions(Quantum_1, Quantum_4);
  handleCollisions(Quantum_2, Quantum_4);
  handleCollisions(Quantum_3, Quantum_4);

  Quantum_1.move();
  Quantum_2.move();
  Quantum_3.move();
  Quantum_4.move();

  uint32_t tempLedColors[N_LED] = {0};
  uint32_t tempHeadPositions[N_LED] = {0};

  // Update the intensity of each Quantum instance based on the volume_mapped
  int intensity_1 = 100;
  int intensity_2 = (volume_mapped > threshold_1) ? 100 : 0;
  int intensity_3 = (volume_mapped > threshold_2) ? 100 : 0;
  int intensity_4 = (volume_mapped > threshold_3) ? 100 : 0;

  Quantum_1.display(tempLedColors, tempHeadPositions, intensity_1);
  Quantum_2.display(tempLedColors, tempHeadPositions, intensity_2);
  Quantum_3.display(tempLedColors, tempHeadPositions, intensity_3);
  Quantum_4.display(tempLedColors, tempHeadPositions, intensity_4);

  for (int i = 0; i < N_LED; i++) {
    ledColors[i] = tempLedColors[i];
    headPositions[i] = tempHeadPositions[i];
    strip.setPixelColor(i, ledColors[i]);
  }
}

void runEffect4(int volume_mapped) {
  static Quantum<4, N_LED> Quantum_1(strip.Color(0, 0, 155), 3, tailIntensities4, 1, 1);
  static Quantum<4, N_LED> Quantum_2(strip.Color(0, 155, 0), 5, tailIntensities4, -1, 1);
  static Quantum<4, N_LED> Quantum_3(strip.Color(155, 0, 0), 4, tailIntensities4, 1, 2);
  static Quantum<4, N_LED> Quantum_4(strip.Color(100, 100, 100), 7, tailIntensities4, -1, 2);

  handleCollisions(Quantum_1, Quantum_2);
  handleCollisions(Quantum_2, Quantum_3);
  handleCollisions(Quantum_1, Quantum_3);
  handleCollisions(Quantum_1, Quantum_2);
  handleCollisions(Quantum_1, Quantum_4);
  handleCollisions(Quantum_2, Quantum_4);
  handleCollisions(Quantum_3, Quantum_4);

  Quantum_1.move();
  Quantum_2.move();
  Quantum_3.move();
  Quantum_4.move();

  uint32_t tempLedColors[N_LED] = {0};
  uint32_t tempHeadPositions[N_LED] = {0};

  Quantum_1.display(tempLedColors, tempHeadPositions, 100 -  2 * volume_mapped);
  Quantum_2.display(tempLedColors, tempHeadPositions, 100 - 2 * volume_mapped);
  Quantum_3.display(tempLedColors, tempHeadPositions, volume_mapped * 2 + 15);
  Quantum_4.display(tempLedColors, tempHeadPositions, volume_mapped * 2 + 15);

  for (int i = 0; i < N_LED; i++) {
    ledColors[i] = tempLedColors[i];
    headPositions[i] = tempHeadPositions[i];
    strip.setPixelColor(i, ledColors[i]);
  }
}

void runEffect5(int volume_mapped) {
  static Quantum<4, N_LED> Quantum_1(strip.Color(0, 0, 155), 3, tailIntensities4, 1, 1);
  static Quantum<4, N_LED> Quantum_2(strip.Color(155, 0, 0), 3, tailIntensities4, -1, 1);

  Quantum_1.move();
  Quantum_2.move();

  uint32_t tempLedColors[N_LED] = {0};
  uint32_t tempHeadPositions[N_LED] = {0};

  Quantum_1.changeVelocity(int(volume_mapped/2));
  Quantum_1.display(tempLedColors, tempHeadPositions, 100);

  Quantum_2.changeVelocity(int(volume_mapped/3));
  Quantum_2.display(tempLedColors, tempHeadPositions, 100);


  for (int i = 0; i < N_LED; i++) {
    ledColors[i] = tempLedColors[i];
    headPositions[i] = tempHeadPositions[i];
    strip.setPixelColor(i, ledColors[i]);
  }
}
