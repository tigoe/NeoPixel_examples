/*
  Simple NeoPixel control
  This sketch allows serial control over RGBW NeoPixels.
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  updated 30 Dec 2020
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
}

void loop() {
  if (Serial.available()) {
    int r = Serial.parseInt();
    int g = Serial.parseInt();
    int b = Serial.parseInt();
    int w = Serial.parseInt();
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.println(b);

    unsigned long color = (w << 24) + (r << 16) + (g<<8) + b;
    strip.fill(color);
  }
  strip.show();
}
