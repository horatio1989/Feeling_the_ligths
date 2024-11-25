#ifndef QUANTUMTFT_H
#define QUANTUMTFT_H

#include "QuantumDEF.h"

#include <Adafruit_GFX.h>
#include <AnimatedGIF.h>
#include <TFT_eSPI.h>
#include <SD.h>

#include <iostream>
#include <vector>
#include <set>

extern TFT_eSPI    tft;
extern TFT_eSprite sprite;

/*AnimatedGIF gif;
File gifFile;
const char *filename = "/gif1.gif"; */

using Point = std::pair<int, int>;

std::vector<std::vector<Point>> generate_triangle_side_points(int x, int y, int side_length, int points_per_side, double angle_shift);

uint16_t read16(File f);
uint32_t read32(File f);
void bmpDraw(const char *filename, uint8_t x, uint16_t y);
void tft_print(const char *format, ...);
void tft_println(const char *format, ...);
void tft_cls();

#endif
