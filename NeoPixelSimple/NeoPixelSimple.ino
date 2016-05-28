/*
  Simple NeoPixel control

  This sketch allows serial control over RGB NeoPixels.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 6;  // control pin
const int numPixels = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_RGB + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // Initialize all pixels to 'off'
}

void loop() {
  int red = 12;   // set colors
  int green = 45;
  int blue = 255;

  
  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.setPixelColor(pixel, red, green, blue);// set the color for this pixel
  }
  strip.show();                                // refresh the strip
}


