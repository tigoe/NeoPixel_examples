/*
  Dark Chaser

  This sketch turns off one neoPixel at a time in a strip.
  
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 28 March 2016
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int pinNumber = 6;    // Pin number of the DIN line to the neoPixels
const int pixelCount = 8;   // number of pixels
long pixelColor[pixelCount]; // array to hold the pixel colors

// declare the strip of pixels:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, pinNumber, NEO_RGB + NEO_KHZ800);

void setup() {
   strip.begin();   // initialize pixel strip
   strip.clear();   // turn all pixels off
  strip.show();     // Initialize all pixels to 'off'

// turn on all the pixels first:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    pixelColor[pixel] = 0x1500CC;  // set pixel color to blue
     strip.setPixelColor(pixel, pixelColor[pixel]);
  }
  strip.show();
}

void loop() {
  // loop over all the pixels, turn on one at a time:
  for (int pixel = 0; pixel < pixelCount; pixel++) { 
    long thisColor = pixelColor[pixel];       // get the color for this pixel from the array
    strip.setPixelColor(pixel, 0);            // set the color for this pixel
    if (pixel > 0) {                          // if pixel > 0, 
      strip.setPixelColor(pixel - 1, thisColor);      // then turn off pixel 0
    } else {                                  // if pixel == 0,
      strip.setPixelColor(pixelCount - 1, thisColor); // then turn off highest number pixel
    }
    strip.show();                             // refresh the strip
    delay(100);                               // wait a tenth of a second
  }
}


