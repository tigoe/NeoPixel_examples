/*
   NeoPixel gamma32 control with Adafruit NeoPixel Library

  This sketch shows how to use the gamma32 command to color correct
  a color. Doesn't work on RGB addressable LEDs, only RGBW.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 7 Jun 2021
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
  strip.show();     // Initialize all pixels
}

void loop() {
  // put in any color here:
  unsigned long color = 0x7FFF0000;
  unsigned long corrected = strip.gamma32(color);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  Serial.print("color: ");
  Serial.println(color, HEX);
  strip.show();                       // refresh the strip
  delay(3000);
  
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, corrected);// set the color for this pixel
  }
  Serial.print("gamma32 corrected: ");
  Serial.println(corrected, HEX);
  strip.show();                       // refresh the strip

  delay(3000);

}
