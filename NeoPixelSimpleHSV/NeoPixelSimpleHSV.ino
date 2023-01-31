/*
   NeoPixel HSV control with Adafruit NeoPixel Library

  This sketch shows how to convert a hue, saturation, and value range
  to a single color value for controlling RGBW neopixels. Doesn't work on
  RGB addressable LEDs.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 7 Jun 2021
  modified 31 Jan 2023
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

unsigned long hue = 18000;
unsigned long color = 0;
int sat = 255;
int val = 255;
int hChange = 10;

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
  // hue fades through the color spectrum, starting from red.
  Serial.print(hue);
  Serial.print(",");
  Serial.print(sat);
  Serial.print(",");
  Serial.print(val);
  Serial.print("\t");
  Serial.println(color, HEX);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    color = strip.ColorHSV(hue, sat, val);
    strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  strip.show();                       // refresh the strip

  hue += hChange;
  // keep it in a range from blue to green:
  if (hue > 32767 || hue < 18000) {
    hChange = -hChange;
  }
  // add 10 to the hue and delay a little bit:
  if (hue > 32767) {
    hue = 18000;
  } else {
    hue += 10;
  }
  delay(5);
}
