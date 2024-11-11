#include "QuantumTFT.h"

#include <Adafruit_GFX.h>
#include <AnimatedGIF.h>
#include <TFT_eSPI.h>
#include <SD.h>

#include <iostream>
#include <vector>
#include <set>

TFT_eSPI    tft    = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define BUFFPIXEL 20

using Point = std::pair<int, int>;

std::vector<std::vector<Point>> generate_triangle_side_points(int x, int y, int side_length, int points_per_side, double angle_shift) {
    std::vector<std::vector<Point>> triangle_points(3);
    double angle_rad = angle_shift * PI / 180.0;

    double angle1 = angle_rad;
    double angle2 = PI / 3 + angle_rad;

    int x1 = x + side_length * cos(angle1);
    int y1 = y + side_length * sin(angle1);
    int x2 = x + side_length * cos(angle2);
    int y2 = y + side_length * sin(angle2);

    for (int i = 0; i <= points_per_side; i++) {
        double t = static_cast<double>(i) / points_per_side;
        int px = static_cast<int>(x + t * (x1 - x));
        int py = static_cast<int>(y + t * (y1 - y));
        triangle_points[0].emplace_back(px, py);
    }

    for (int i = 0; i <= points_per_side; i++) {
        double t = static_cast<double>(i) / points_per_side;
        int px = static_cast<int>(x1 + t * (x2 - x1));
        int py = static_cast<int>(y1 + t * (y2 - y1));
        triangle_points[1].emplace_back(px, py);
    }

    for (int i = 0; i <= points_per_side; i++) {
        double t = static_cast<double>(i) / points_per_side;
        int px = static_cast<int>(x + t * (x2 - x));
        int py = static_cast<int>(y + t * (y2 - y));
        triangle_points[2].emplace_back(px, py);
    }

    return triangle_points;
}

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

void bmpDraw(const char *filename, uint8_t x, uint16_t y) {
  File     bmpFile;
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset = 0;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0;

  if((x >= tft.width()) || (y >= tft.height())) return;
  // Open requested file on SD card
  if (!(bmpFile = SD.open(filename))) {
    Serial.println(F("File not found"));
    return;
  }
  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) {     // BMP signature
    read32(bmpFile);
    (void)read32(bmpFile);    // Read & ignore creator bytes
    read32(bmpFile);          // Start of image data
    read32(bmpFile);          // Read DIB header
    w = read32(bmpFile);
    h = read32(bmpFile);
    if(read16(bmpFile) == 1) {    // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed
        goodBmp = true;              // Supported BMP format -- proceed!
        rowSize = (w * 3 + 3) & ~3;  // BMP rows are padded (if needed) to 4-byte boundary
        // If bmpHeight is negative, image is in top-down order.
        if(h < 0) {
          h    = -h;
          flip = false;
        }
        // Crop area to be loaded
        if((x+w-1) >= tft.width())  
          w = tft.width()  - x;
        if((y+h-1) >= tft.height()) 
          h = tft.height() - y;
        // Set TFT address window to clipped image bounds

        sprite.createSprite(w, h);
        for (row=0; row<h; row++) { // For each scanline...
          // Seek to start of scan line.  It might seem labor-intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (h - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) {
            tft.endWrite();
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }
          for (col=0; col<w; col++) { // For each pixel...
            if (buffidx >= sizeof(sdbuffer)) {
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0;
            }
            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            sprite.drawPixel(col, h - row - 1, sprite.color565(r, g, b));
          }
        }
        sprite.pushSprite(x, y);
      }
    }
  }
  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}