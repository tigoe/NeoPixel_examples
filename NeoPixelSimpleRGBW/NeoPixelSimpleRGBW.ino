/*
  Simple NeoPixel control
  This sketch allows serial control over RGBW NeoPixels.
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  updated 28 Jun 2018
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
}

void loop() {
  byte red = 23;   // set colors
  byte green = 0;
  byte blue = 255;
  byte white = 0;

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, red, green, blue, white);// set the color for this pixel
    delay(250);
    strip.show();    // refresh the strip
  }
  delay(1000);
  strip.clear(); 
}
