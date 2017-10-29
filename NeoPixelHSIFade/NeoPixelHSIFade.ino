/*
  RGBW NeoPixel HSI-to-RGB conversion and fade

  This sketch fades the hue of a neoPixel device

  It converts HSI values to a single RGB color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  Based on HSI conversion from Saiko LED, by Brian Neitner:
  http://blog.saikoled.com/post/44677718712/how-to-convert-from-hsi-to-rgb-white
  http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi

  created 31 Jan 2017
  modified 29 Oct 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#include "HSI.h"

const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels
int change = 1;              // increment to change hue by

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

int h = 0;          // hue
int s = 100;        // saturation
int i = 100;        // intensity

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
}

void loop() {
  // create a single color from hue, sat, intensity:
  unsigned long color = hsiToRgbw(h, s, i);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);    // set the color for this pixel
    strip.show();   // update the strip
    delay(100);
  }

  // increment hue to fade from red (0) to reddish orange (15) and back:
  h = h + change;
  if (h < 0 || h > 15) {
    change = -change;
  }
}

