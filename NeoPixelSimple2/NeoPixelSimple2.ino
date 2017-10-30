/*
  Simple NeoPixel control

  This sketch allows serial control over RGBW NeoPixels.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 Oct 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 6;  // control pin
const int pixelCount = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // Initialize all pixels to 'off'
}

void loop() {
  /*
    Each byte of the color is a hue, in the sequence WWRRGGBB.
    0xFF000000 is white
    0x00FF0000 is red
    0x0000FF00 is green
    0x000000FF is blue

    Other colors are made by combining, e.g. 
    0x000077FF is teal
    0x003300FF is violet
    

  */
  unsigned long color = 0x00FF0000;


  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  strip.show();                                // refresh the strip
}


