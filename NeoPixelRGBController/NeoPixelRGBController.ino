/*
  RGBW NeoPixel controller

  This sketch changes of a neoPixel device based on four potentiometers,
  one for red, green, blue, and white

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 29 Oct 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
}

void loop() {
  int red = analogRead(A0) / 4;  // read potentiometers
  delay(1);                      // delay to stabilize ADC
  int green = analogRead(A1) / 4;
  delay(1);
  int blue = analogRead(A2) / 4;
  delay(1);
  int white = analogRead(A3) / 4;

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, red, green, blue, white);    // set the color for this pixel
    strip.show();   // update the strip
    delay(100);
  }
}

