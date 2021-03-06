/*
  RGBW NeoPixel HSI-to-RGB conversion and fade

  This sketch fades the hue of a neoPixel device

  It converts HSI values to a single RGB color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  Uses ColorConverter library: https://github.com/tigoe/ColorConverter

  created 31 Jan 2017
  modified 27 Jun 2018
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

const int neoPixelPin = 5;   // control pin
const int pixelCount = 8;    // number of pixels
int change = 1;              // increment to change hue by

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRB + NEO_KHZ800);
ColorConverter converter;

int h = 10;         // hue
int s = 100;        // saturation
int i = 100;        // intensity

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
}

void loop() {
  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGB(h, s, i);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color.red, color.green, color.blue);    // set the color for this pixel
    strip.show();   // update the strip
    delay(50);
  }

  // increment hue to fade from red (0) to reddish orange (15) and back:
  h = h + change;
  if (h < 0 || h > 15) {
    change = -change;
  }
}

