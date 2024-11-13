#include "TFT_eff.h"

void draw_equilateral_triangle(int x, int y, int side_length, double angle_shift) {
    double angle_rad = angle_shift * PI / 180;

    double angle1 = angle_rad;
    double angle2 = PI / 3 + angle_rad;

    int x1 = x + side_length * cos(angle1);
    int y1 = y + side_length * sin(angle1);
    int x2 = x + side_length * cos(angle2);
    int y2 = y + side_length * sin(angle2);

    sprite.createSprite(240, 240);
    sprite.setRotation(1);

    sprite.drawLine(x, y, x1, y1, tft.color565(255, 255, 255));
    sprite.drawLine(x, y, x2, y2, tft.color565(255, 255, 255));
    sprite.drawLine(x1, y1, x2, y2, tft.color565(255, 255, 255));
    sprite.pushSprite(0, 0);
}

void tft_set_brightnes(uint8_t Value) {
  if ((Value < 0) || (Value > 100)) analogWrite(TFT_BL, 50);
  else                              analogWrite(TFT_BL, Value * 2.55);
}

void triangulos_javi() {
  sprite.createSprite(240, 240);
  sprite.setRotation(1);
  sprite.fillScreen(TFT_BLACK);
  sprite.pushSprite(0, 0);

  static int current_iteration = 0;
  /* Mirando el Kale de frente, con el triangulo en esta orientacion: |> el origen (0,0) esta arriba a la izquierda. 
      Eje Y aumenta hacia abajo, Eje X aumenta a la derecha
      El lado del triangulo es aprox 84 pixels
      El centro esta en (138,120)
      Los vertices del triangulo son: 
      (55,17)
                (220,120)
      (55,218) 
      El diametro del circulo es aproximadamente 194 pixels
      El tamaño de pixel es de 135um x 135um
      */
  int XC = 55;
  int YC = 17;
  int CIRCLE_DIAMETER = 97*2;
  //int x_ana_remap = map(analogRead(POTE), 0, 4095, 0, 90);
  //sprite.drawLine(0, x_ana_remap,240 , x_ana_remap, tft.color565(0, 255, 0));
  //sprite.pushSprite(0, 0);
  //Serial.println(x_ana_remap);
  const int n_points = 10;
  std::vector<std::vector<Point>> points = generate_triangle_side_points(XC, YC, CIRCLE_DIAMETER, n_points, 30);

  int j1 = current_iteration % n_points;
  int j2 = (current_iteration + 1) % n_points;
  int j3 = (current_iteration + 2) % n_points;
  int j4 = (current_iteration + 3) % n_points;
  int j5 = (current_iteration + 4) % n_points;
  int j6 = (current_iteration + 5) % n_points;
  int j7 = (current_iteration + 6) % n_points;

  int indices[] = {j1, j2, j3, j4, j5, j6, j7};

  // Dibuja líneas entre los puntos en diferentes lados con diferentes colores
  for (int i = 0; i < 5; ++i) {
      sprite.drawLine(points[0][indices[i]].first, points[0][indices[i]].second,
                    points[1][indices[i]].first, points[1][indices[i]].second,
                    sprite.color565(255, 0, 0));
      sprite.drawLine(points[1][indices[i]].first, points[1][indices[i]].second,
                    points[2][n_points - indices[i]].first, points[2][n_points - indices[i]].second,
                    sprite.color565(0, 255, 0));
      sprite.drawLine(points[0][indices[i]].first, points[0][indices[i]].second,
                    points[2][n_points - indices[i]].first, points[2][n_points - indices[i]].second,
                    sprite.color565(0, 0, 255));
      sprite.pushSprite(0, 0);
      delay(50);
  }
  current_iteration++;
}

// Función que calcula los puntos de un polígono regular usando grados
void polygons(int centerX, int centerY, int radius, int vertices, int* xPoints, int* yPoints, float rotationAngleDegrees) {
  float angleStep = 360.0 / vertices;
  float rotationAngle = rotationAngleDegrees * (PI / 180.0); // Conversión de grados a radianes

  for (int i = 0; i < vertices; i++) {
    float angle = i * angleStep * (PI / 180.0) + rotationAngle; // Conversión de cada ángulo a radianes
    xPoints[i] = centerX + radius * cos(angle);
    yPoints[i] = centerY + radius * sin(angle);

    if (xPoints[i] < 0)    xPoints[i] = 0;
    if (xPoints[i] >= 240) xPoints[i] = 239;
    if (yPoints[i] < 0)    yPoints[i] = 0;
    if (yPoints[i] >= 240) yPoints[i] = 239;
  }
}

void fillPolygon(int* xPoints, int* yPoints, int vertices, TFT_eSprite &sprite, uint32_t color) {
  for (int y = 0; y < 240; y++) { // Recorre cada línea horizontal de la pantalla
    int intersections[10]; // Máximo de 10 intersecciones por línea
    int count = 0;

    for (int i = 0; i < vertices; i++) {
      int next = (i + 1) % vertices;
      // Comprobar si la línea horizontal (y) cruza el segmento del polígono
      if ((yPoints[i] < y && yPoints[next] >= y) || (yPoints[next] < y && yPoints[i] >= y)) {
        int x = xPoints[i] + (y - yPoints[i]) * (xPoints[next] - xPoints[i]) / (yPoints[next] - yPoints[i]);// Calcula el punto de intersección en x
        intersections[count++] = x; // Guarda el punto de intersección
      }
    }
    // Dibuja el interior del polígono
    for (int j = 0; j < count; j += 2) {
      // Asegúrate de que las intersecciones están en orden
      if (j + 1 < count) {
        // Asegúrate de que se dibujen entre los puntos
        if (intersections[j] > intersections[j + 1]) {
          int temp = intersections[j];
          intersections[j] = intersections[j + 1];
          intersections[j + 1] = temp;
        }
        sprite.fillRect(intersections[j], y, intersections[j + 1] - intersections[j], 1, color); // Rellena entre las intersecciones
      }
    }
  }
}

void drawPolygon(int centerX, int centerY, int radius, int vertices, float rotationAngleDegrees, uint32_t color) {
  int* xPoints = new int[vertices]; // Arreglo para coordenadas x
  int* yPoints = new int[vertices]; // Arreglo para coordenadas y

  polygons(centerX, centerY, radius, vertices, xPoints, yPoints, rotationAngleDegrees);

  // Rellena el interior del polígono
  fillPolygon(xPoints, yPoints, vertices, sprite, color);

  for (int i = 0; i < vertices; i++) {
    int next = (i + 1) % vertices; // Conecta el último vértice con el primero
    sprite.drawLine(xPoints[i], yPoints[i], xPoints[next], yPoints[next], color); // Dibuja la línea en el sprite
  }
  delete[] xPoints; // Libera la memoria del arreglo x
  delete[] yPoints; // Libera la memoria del arreglo y
}

const int lb_window          = 30;
double    bass_energy_history[lb_window];
int       circ_current_index = 0;
int       circ_count         = 0;
bool      ya_tiene_30        = false;

double efectos_bajos(double dt_factor, double static_threshold) {
  sampling();
  processAudio();
  for (int i = 0; i < lb_window - 1; i++)
    bass_energy_history[i] = bass_energy_history[i + 1];
  bass_energy_history[lb_window - 1] = vReal[2]+vReal[3]+vReal[4]+vReal[5];
  circ_current_index = (circ_current_index + 1) % lb_window;
  if (circ_count < lb_window) {
    circ_count++;
    if (circ_count == lb_window) ya_tiene_30 = true;
  }

  double current_value = 0.0;
  double threshold     = 1.0;
  if (ya_tiene_30) {
    double rolling_avg = 0.0;
    for (int i = 0; i < lb_window; i++)
        rolling_avg += bass_energy_history[i];
    rolling_avg /= lb_window;

    double dynamic_threshold = rolling_avg * dt_factor;
    threshold = dynamic_threshold;
    if (static_threshold > threshold) 
      threshold = static_threshold;
    current_value  = bass_energy_history[lb_window - 1];
    double previous_value = bass_energy_history[lb_window - 2];
    if ((current_value > threshold) && (current_value > previous_value))
      return current_value/threshold;
    else 
      return 0.0;
  } else
      return 0.0;
}

float oscillate(int counter, float minValue, float maxValue) {
  float length = fabs(maxValue - minValue);
  if (length == 0) return minValue;  // minValue and maxValue are the same

  int   cycle       = 2 * length;
  int   counter_mod = counter % cycle;
  float delta       = length - fabs(length - counter_mod);
  return minValue + delta;
}

// Function to rotate a point (px, py) around a center (cx, cy) by angle in degrees
void rotatePoint(float px, float py, float cx, float cy, float angleDegrees, float &rx, float &ry) {
    float angleRadians = angleDegrees * (PI / 180.0); // Convert angle to radians
    float s = sin(angleRadians);
    float c = cos(angleRadians);

    // Translate point back to origin
    px -= cx;
    py -= cy;

    // Rotate point
    float xnew = px * c - py * s;
    float ynew = px * s + py * c;

    // Translate point back
    rx = xnew + cx;
    ry = ynew + cy;
}

// Function to convert HSV to RGB
void HSVtoRGB(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
    if (s == 0) {  // Cuando la saturación es 0, el color es gris
        r = g = b = static_cast<uint8_t>(v * 255);
        return;
    }

    float c = v * s;
    float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
    float m = v - c;

    float r_, g_, b_;
    
    int segment = static_cast<int>(h / 60.0) % 6;
    switch (segment) {
        case 0: r_ = c; g_ = x; b_ = 0; break;
        case 1: r_ = x; g_ = c; b_ = 0; break;
        case 2: r_ = 0; g_ = c; b_ = x; break;
        case 3: r_ = 0; g_ = x; b_ = c; break;
        case 4: r_ = x; g_ = 0; b_ = c; break;
        case 5: r_ = c; g_ = 0; b_ = x; break;
    }

    r = static_cast<uint8_t>((r_ + m) * 255);
    g = static_cast<uint8_t>((g_ + m) * 255);
    b = static_cast<uint8_t>((b_ + m) * 255);
}

// Modify the drawFractal function to accept hue and depth
void drawFractal(TFT_eSprite &sprite, int cx, int cy, float side, float counter, float hue, int depth) {
    if (depth > 4) return; // Base case to stop recursion when the side is too small

    // Adjust hue based on recursion depth to create color variation
    float newHue = fmod(hue + depth * 30, 360); // Change 30 to adjust the hue shift per depth

    // Convert HSV to RGB
    uint8_t r, g, b;
    HSVtoRGB(newHue, 1.0, 1.0, r, g, b);
    uint16_t color = sprite.color565(r, g, b);

    // Draw the main triangle using drawPolygon function
    int vertices = 3;
    drawPolygon(cx, cy, side / 1.73, vertices, counter, color);

    // Calculate rotated positions for the smaller triangles
    float cx_r, cy_r;

    // Top triangle (rotated)
    //rotatePoint(cx + side / 2, cy + side / 3, cx, cy, counter, cx_r, cy_r);
    rotatePoint(cx + side / 3, cy + side / 2, cx, cy, counter, cx_r, cy_r);
    drawFractal(sprite, cx_r, cy_r, side / 2, counter, hue, depth + 1); // Recursively draw top triangle

    // Bottom-left triangle (rotated)
    //rotatePoint(cx - side / 2, cy + side / 3, cx, cy, counter, cx_r, cy_r);
    rotatePoint(cx + side / 3, cy - side / 2, cx, cy, counter, cx_r, cy_r);
    drawFractal(sprite, cx_r, cy_r, side / 2, counter, hue, depth + 1); // Recursively draw bottom-left triangle

    // Bottom-right triangle (rotated)
    //rotatePoint(cx, cy - side * 0.6, cx, cy, counter, cx_r, cy_r);
    rotatePoint(cx - side * 0.6, cy, cx, cy, counter, cx_r, cy_r);
    drawFractal(sprite, cx_r, cy_r, side / 2, counter, hue, depth + 1); // Recursively draw bottom-right triangle
}

// Prototipos que estan siendo trabajados
const float VISIBLE_ANGLE_MIN = -5.0;   // Ángulo mínimo para la visibilidad del cuadrado
const float VISIBLE_ANGLE_MAX = 5.0; 
const int CENTER_X = 120;          // Centro horizontal de la pantalla
const int CENTER_Y = 120;          // Centro vertical de la pantalla
const int SQUARE_SIZE = 80;        // Tamaño fijo del cuadrado
const int SCREEN_WIDTH = 240; 

void figura() {
  readIMU();  // Lee los ángulos actuales

  // Calcula la posición horizontal del cuadrado en función de angleZ con precisión de float
  float positionX = map(angleX, VISIBLE_ANGLE_MIN, VISIBLE_ANGLE_MAX, -SQUARE_SIZE, SCREEN_WIDTH);

  // Limpia la pantalla
  sprite.createSprite(240, 240);
  sprite.fillSprite(TFT_BLACK);

  // Si el ángulo está en la ventana visible, dibuja el cuadrado
  if (angleX >= VISIBLE_ANGLE_MIN && angleX <= VISIBLE_ANGLE_MAX) {
    int color = sprite.color565(255, 255, 255); // Color blanco (sin cambio de intensidad)
    
    // Dibuja el cuadrado en la posición calculada con suavidad
    sprite.fillRect(static_cast<int>(positionX), CENTER_Y - SQUARE_SIZE / 2, SQUARE_SIZE, SQUARE_SIZE, color);
  }

  // Muestra el sprite en la pantalla
  sprite.pushSprite(0, 0);
  delay(1);  // Reducción de delay para actualizaciones más frecuentes y movimiento más suave
}