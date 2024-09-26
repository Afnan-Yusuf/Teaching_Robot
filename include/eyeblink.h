#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Function prototypes
void drawEye(bool blink, float blinkProgress = 0);
void drawEllipse(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color);

void initeyedisplay() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}

void eyeblink() {
  // Open eye
  drawEye(false);
  delay(3000);

  // Blink
  for (int i = 0; i <= 10; i++) {
    drawEye(true, i * 0.1);
    delay(20);
  }
  for (int i = 10; i >= 0; i--) {
    drawEye(true, i * 0.1);
    delay(20);
  }

  // Open eye again
  drawEye(false);
  delay(3000);
}

void drawEye(bool blink, float blinkProgress) {
  display.clearDisplay();
  
  // Increase the size of the eye
  int centerX = 64;  // Center of the display (X axis)
  int centerY = 32;  // Center of the display (Y axis)
  
  // New larger dimensions
  int width = 90;    // Increased width of the eye
  int height = 45;   // Increased height of the eye
  
  if (blink) {
    height = height * (1 - blinkProgress);  // Shrink eye height during blink
  }
  
  // Draw thicker outer eye outline by drawing multiple ellipses
  for (int i = 0; i < 3; i++) { // Loop to draw multiple ellipses for thickness (change 3 to higher for thicker line)
    drawEllipse(display, centerX, centerY, (width/2) - i, (height/2) - i, WHITE);
  }
  
  // Draw iris and pupil if eye is open enough
  if (height > 10) {
    // Increased iris size, making sure it fits in the eye
    int irisSize = min(height - 4, 30);  // Increased from 20 to 30 for a larger iris
    display.fillCircle(centerX, centerY, irisSize/2, WHITE);  // Iris
    display.fillCircle(centerX, centerY, irisSize/4, BLACK);  // Pupil
  }
  
  display.display();
}




// Standalone function to draw an ellipse
void drawEllipse(Adafruit_SSD1306 &display, int16_t x0, int16_t y0, int16_t rx, int16_t ry, uint16_t color) {
  int16_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {
    display.drawPixel(x0 + x, y0 + y, color);
    display.drawPixel(x0 - x, y0 + y, color);
    display.drawPixel(x0 + x, y0 - y, color);
    display.drawPixel(x0 - x, y0 - y, color);
    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {
    display.drawPixel(x0 + x, y0 + y, color);
    display.drawPixel(x0 - x, y0 + y, color);
    display.drawPixel(x0 + x, y0 - y, color);
    display.drawPixel(x0 - x, y0 - y, color);
    if (s >= 0) {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }
}
