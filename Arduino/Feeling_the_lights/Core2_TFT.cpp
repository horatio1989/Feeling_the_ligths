#include <Arduino.h>

#include "Core2_TFT.h"

int fcounter = 0;

void Task_TFT_code(void * parameter) {
  for( ;; ) {
    //ArduinoOTA.handle();
    int bass_level = 1;
    float side = oscillate(fcounter, 0, 280);

    float oscValue = oscillate(fcounter, -30, 30);
    int centerX = 125 + static_cast<int>(oscValue);
    int centerY = 122 + static_cast<int>(oscValue);

    sprite.createSprite(240, 240);
    sprite.fillSprite(TFT_BLACK);
    sprite.setRotation(1);

    bass_level = efectos_bajos(3, 0.5);
    println_telnet(bass_level);

    // Calculate hue based on fcounter to cycle colors over time
    float hue = fmod(fcounter * 2.0, 360.0); // Adjust the multiplier to control the speed of hue change

    // Draw fractal with dynamic colors
    drawFractal(sprite, centerX, centerY, side, fcounter+bass_level*20, hue, 0); // Start with depth 0

    sprite.pushSprite(0, 0); // Display the sprite on the screen
    delay(5);

    // Increment counter
    fcounter += 2;
  }
  vTaskDelete(NULL);
}