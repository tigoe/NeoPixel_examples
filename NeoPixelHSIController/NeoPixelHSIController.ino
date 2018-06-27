/*
  RGBW NeoPixel HSI controller

  This sketch changes of a neoPixel device based on three potentiometers,
  one for hue, one for saturation,and one for intensity

  It converts HSI values to a single RGB color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  Uses ColorConverter library: https://github.com/tigoe/ColorConverter

  created 29 Oct 2017
  modified 27 Jun 2018
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip
}

void loop() {
  int sensor1 = analogRead(A0);  // read potentiometers
  delay(1);                      // delay to stabilize ADC
  int sensor2 = analogRead(A1);
  delay(1);
  int sensor3 = analogRead(A2);

  int h = map(sensor1, 0, 1023, 0, 360);   // convert to hue
  int s = map(sensor2, 0, 1023, 0, 100);   // convert to saturation
  int i = map(sensor3, 0, 1023, 0, 100);   // convert to intensity

  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGBW(h, s, i);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color.red, color.green, color.blue, color.white ); // set the color for this pixel
    strip.show();   // update the strip
    delay(100);
  }
}

