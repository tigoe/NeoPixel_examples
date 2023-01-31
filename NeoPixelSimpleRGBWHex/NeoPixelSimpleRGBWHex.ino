/*
  Simple NeoPixel control

  This sketch allows serial control over RGBW NeoPixels. It shows how to control
  them using a single long integer, written as a hexadecimal string, like
  web colors, (e.g. 00FF0000 is red)

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 Oct 2017
  updated 7 Jun 2021
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

unsigned long color = 0x0000FF00;
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



  // print the color in hex so you know what you're seeing:
  Serial.println(color, HEX);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  strip.show();                                // refresh the strip
  delay(1000);
  if (color >= 0xFF000000) {
    color = 0xFF;
  } else {
    color = color << 8;
  }
}
