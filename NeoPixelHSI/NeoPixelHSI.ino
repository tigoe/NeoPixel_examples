/*
  RGBW NeoPixel HSI-to-RGB and RGBW conversion

  This sketch accepts a serial input of hue, saturation and intensity (HSI)
  (h=0-360 degrees, s=0-100%, i=0-100%) in the following format:
  Ch,s,i\n

  It converts these values to a single RGBW color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  Based on HSI conversion from Saiko LED, by Brian Neitner:
  http://blog.saikoled.com/post/44677718712/how-to-convert-from-hsi-to-rgb-white
  http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi

  created 31 Jan 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#include "HSI.h"

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();        // initialize pixel strip
  strip.clear();        // turn all LEDs off
  strip.show();         // update strip
  Serial.begin(9600);   // initialize serial
  Serial.setTimeout(10);// timeout for Serial.parseInt() function
}

void loop() {
  if (Serial.available() > 0) {   // if there's serial data,
    if (Serial.read() == 'C') {   // and the first byte is C,
      int h = Serial.parseInt();  // listen for an ASCII numeric string
      int s = Serial.parseInt();  // listen for another ASCII numeric string
      int i = Serial.parseInt();  // listen for another ASCII numeric string
      Serial.print(h);            // print what you got
      Serial.print("°,");
      Serial.print(s);
      Serial.print("%,");
      Serial.print(i);
      Serial.println("%");
      // convert the values to RGB:
      //      unsigned long color = hsiToRgb(h, s, i);
      //      Serial.print("RGB: 0x");
      unsigned long color = hsiToRgbw(h, s, i);   // use these lines for RGBW
      Serial.print("RGBW: 0x");
      Serial.println(color);
      // loop over all the pixels:
      for (int pixel = 0; pixel < pixelCount; pixel++) {
        strip.setPixelColor(pixel, color);    // set the color for this pixel
      }
      strip.show();   // update the strip
    }
  }
}
