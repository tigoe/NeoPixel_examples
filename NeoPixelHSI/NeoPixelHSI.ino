/*
  RGBW NeoPixel HSI-to-RGB and RGBW conversion

  This sketch accepts a serial input of hue, saturation and intensity (HSI)
  (h=0-360 degrees, s=0-100%, i=0-100%) in the following format:
  Ch,s,i\n

  It converts these values to a single RGBW color and sets a neoPixel string

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  Uses ColorConverter library: https://github.com/tigoe/ColorConverter

  created 31 Jan 2017
  modified 27 June 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;

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
      // convert the values to RGBW:
      RGBColor color = converter.HSItoRGBW(h, s, i);
      Serial.print("RGBW: 0x");
      Serial.println(color.red, HEX);
      Serial.print(color.green, HEX);
      Serial.print(color.blue, HEX);
      Serial.println(color.white, HEX);
      
      // loop over all the pixels:
      for (int pixel = 0; pixel < pixelCount; pixel++) {
        strip.setPixelColor(pixel, color.red, color.green, color.blue, color.white);    // set the color for this pixel
      }
      strip.show();   // update the strip
    }
  }
}
