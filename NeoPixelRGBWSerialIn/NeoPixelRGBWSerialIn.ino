/*
  RGBW NeoPixel RGBW control

  This sketch accepts a serial input of:
  C red, green, blue, white\n
  or H hue, intensity, value\n

  It converts these values to a single RGBW color and sets a neoPixel strip

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 30 Jan 2017
  modified 31 Jan 2023
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

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
    char incoming = Serial.read();
    if (incoming == 'C') {   // and the first byte is C,
      int r = Serial.parseInt();  // listen for an ASCII numeric string
      int g = Serial.parseInt();  // listen for another ASCII numeric string
      int b = Serial.parseInt();  // listen for another ASCII numeric string
      int w = Serial.parseInt();  // listen for another ASCII numeric string
      Serial.print(r);            // print what you got
      Serial.print(",");
      Serial.print(g);
      Serial.print(",");
      Serial.print(b);
      Serial.print(",");
      Serial.println(w);
      for (int p = 0; p < pixelCount; p++) {
        strip.setPixelColor(p, r, g, b, w); // set the color for this pixel
      }
    } else if (incoming == 'H') {
      unsigned long h = Serial.parseInt();  // listen for an ASCII numeric string
      int s = Serial.parseInt();  // listen for another ASCII numeric string
      int v = Serial.parseInt();  // listen for another ASCII numeric string
      unsigned long color = strip.ColorHSV(h, s, v);
      for (int p = 0; p < pixelCount; p++) {
        strip.setPixelColor(p, color);
      }
    }
    strip.show();   // update the strip
  }
}
