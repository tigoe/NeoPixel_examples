/*
  Simple NeoPixel control

  This sketch allows serial control over RGBW NeoPixels.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int numPixels = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // Initialize all pixels to 'off'
}

void loop() {
  int red = 0xFF;   // set colors
  int green = 0x0;
  int blue = 0x00;
  int white = 0x10;

  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    unsigned long color = 0x1200FF00;
  //  strip.setPixelColor(pixel, red, green, blue, white);// set the color for this pixel
     strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  strip.show();                                // refresh the strip
}


