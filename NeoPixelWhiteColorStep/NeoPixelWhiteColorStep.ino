/*
  RGBW NeoPixel control

  This sketch steps through the red, green, blue, and white colors
  on an RGBW neoPixel strip.

  Color on an RGBW pixel can be represented as a 4-byte value 
  in an unsigned long integer. For example: 
    0xFF000000 is white
    0x00FF0000 is green
    0x0000FF00 is red
    0x000000FF is blue

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 30 Jan 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int numPixels = 7;    // number of pixels
unsigned long color = 0xFF; // the color for the whole strip

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_RGBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.setPixelColor(pixel, color);// set the color for this pixel
  }
  strip.show();    // refresh the strip
  
  if (color < 0xFF000000) { // if the color is not red (e.g. 0xFF000000)
  color = color << 8;       // shift the color value by 8 bits
  } else {  
    color = 0xFF;           // if the color is red, reset to white
  }
  delay(1000);              // 1 second delay
}


