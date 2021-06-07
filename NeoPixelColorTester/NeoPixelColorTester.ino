/*
  NeoPixel RGBW  Test

  This sketch  loops through all the pixels of a NeoPixel device
  one pixel at a time, and one color at a time, in the order:
  blue, green, red, white

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  
  created 31 Jan 2017
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int numPixels = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRBW + NEO_KHZ800);
unsigned long color = 0xFF;   // start with blue

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // refresh strip
}

void loop() {

  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.setPixelColor(pixel, color);// set the color for this pixel
    if (pixel > 0) {
      strip.setPixelColor(pixel - 1, 0); // turn off the last pixel
    }
    strip.show();    // refresh the strip
    delay(500);
  }
  color = color << 8;         // shift the FF (255) to the next color
  if (color >= 0xFF000000) {   // if the color is greater than white (0xFF000000)
    color = 0xFF;             // then set it back to blue
  }
  strip.clear();              // clear the strip at the end of a color
}
