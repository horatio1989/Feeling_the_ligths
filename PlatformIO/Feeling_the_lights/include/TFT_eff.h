#ifndef TFT_EFF_H
#define TFT_EFF_H

#include "QuantumDEF.h"
#include "QuantumINI.h"
#include "QuantumOTH.h"
#include "QuantumTFT.h"

#include <vector>
#include <cmath>

void draw_equilateral_triangle(int x, int y, int side_length, double angle_shift);
void tft_set_brightnes(uint8_t Value);
void triangulos_javi();
void polygons(int centerX, int centerY, int radius, int vertices, int* xPoints, int* yPoints, float rotationAngleDegrees);
void fillPolygon(int* xPoints, int* yPoints, int vertices, TFT_eSprite &sprite, uint32_t color);
void drawPolygon(int centerX, int centerY, int radius, int vertices, float rotationAngleDegrees, uint32_t color);
double efectos_bajos(double dt_factor, double static_threshold);
float oscillate(int counter, float minValue, float maxValue);
void rotatePoint(float px, float py, float cx, float cy, float angleDegrees, float &rx, float &ry);
void HSVtoRGB(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b);
void drawFractal(TFT_eSprite &sprite, int cx, int cy, float side, float counter, float hue, int depth);

// Prototipos que estan siendo trabajados
void figura();
#endif
