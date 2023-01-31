#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;   // number of pixels
int change = 100;             // increment to change hue by

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

int h = 1000;  // hue, 0-65535
int s = 255;  // saturation 0-255
int i = 255;  // intensity 0-255

void setup() {
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn all LEDs off
  strip.show();   // update strip
}

void loop() {
  // create a single color from hue, sat, intensity:
  long color = strip.ColorHSV(h, s, i);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);
    strip.show();  // update the strip
    delay(100);
  }

  // increment hue to fade from red (0) to reddish orange (2400) and back:
  h = h + change;
  // when it hits the boundaries, reverse the direction of change:
  if (h <= 0 || h >= 2400) {
    change = -change;
  }
  // constrin hue to 0-2400:
  h = constrain(h, 0, 2400);
}
