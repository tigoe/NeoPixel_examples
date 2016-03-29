/*
  Brightness

  This sketch shows how to use setBrightness() with neoPixels

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int pinNumber = 6;    // Pin number of the DIN line to the neoPixels
const int pixelCount = 8;   // number of pixels

// declare the strip of pixels:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, pinNumber, NEO_RGB + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.show();     // Initialize all pixels to 'off'

  // turn on all the pixels first:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, 0x1500CC);
  }
  strip.show();
}

void loop() {
  // loop from brightest to dimmest:
  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int pixel = 0; pixel < pixelCount; pixel++) {
      strip.setBrightness(brightness);    // set the brightness

      // brightness just acts as a limit on whatever color you set, so
      // you still have to set color for all the pixels, but you'll get
      // a dimmed version of the color you set:
      // loop over all the pixels, turn on one at a time:
      strip.setPixelColor(pixel, 0x1500CC);
    }
    strip.show();                             // refresh the strip
    delay(10);                               // wait a hundredth of a second
  }
  delay(1000);
}


