/*
  RGBW NeoPixel fade control

  This sketch fades the white LED up and down
  on an RGBW neoPixel strip.

  In the setPixelColor command, the colors are ordered: red, green, blue, white.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 30 Jan 2017
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 5;  // control pin
const int numPixels = 7;    // number of pixels
int level = 255;            // the white LED color for the whole strip
int difference = 1;         // the fading difference in each loop

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_RGBW + NEO_KHZ800);

void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  pinMode(0, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // loop over all the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.setPixelColor(pixel, 0, 0, 0, level);// set the color for this pixel
    strip.show();    // refresh the strip

    delay(500);
    if (level < 0) level = 255;
    level = level -20;
    Serial.print(pixel);
    Serial.print("\t");
    Serial.println(level);
  }
  //   if ((level >= 255) || (level <= 0)) { // if level's at the top or bottom
  //    difference = -difference;           // invert the difference value
  //  }
  //  level += difference;                  // add the difference to the level
  //  delay(10);
  //           analogWrite(0, level);
}


