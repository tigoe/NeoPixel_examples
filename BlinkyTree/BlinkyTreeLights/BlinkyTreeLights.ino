/*
  BlinkyTree

  This sketch twinkles a pixel white in response to a serial command (x or X).
  It resets the whole strip to a base color in response to a serial command (z).
  Continually fades the previously twinkled pixels.

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 5 Dec 2014
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>
#define PIN 13

const int numPixels = 50;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_RGB + NEO_KHZ800);

int change[numPixels];                // whether or not a pixel is changing
unsigned long pixelColor[numPixels];  // color for each pixel
unsigned long baseColor = 0x002212;   // In RGB decimal, (0, 34, 18)

void setup() {
  Serial.begin(9600);     // initialize serial communication
  Serial.setTimeout(10);  // set serial timeout
  strip.begin();          // initialize pixel strip
  resetStrip();           // reset the strip
}

void loop() {
  // read serial input:
  while (Serial.available() > 0) {
    int input = Serial.read();
    if (input == 'x' || input == 'X') {    // x signals that you should twinkle a pixel
      int thisPixel = random(numPixels);   // pick a random pixel
      pixelColor[thisPixel] = 0xFFFFFF;    // set its color to white
      change[thisPixel] = 1;               // set its change value to true
    }
    if (input == 'z') {                    // z signals you should reset the whole strip
      resetStrip();
    }
  }

  // lopp over all the pixels:
  for (int star = 0; star < numPixels; star++) {
    // if a pixel's change flag is set:
    if (change[star] == 1) {
      // check to see if it matches the base color yet:
      if (pixelColor[star] == baseColor) {
        // if it's reached base color again, turn its change flag off
        change[star] = 0;
      } else {      // if the pixel hasn't reached base color again,
        //compare its current color to the base color and fade it closer:
        unsigned long newColor = compare(pixelColor[star], baseColor);
        pixelColor[star] = newColor;
      }
      strip.setPixelColor(star, pixelColor[star]);// set the color for this pixel
    }
  }
  // update the strip:
  strip.show();
}

/*
  This function resets all the pixels to the base color
   and turns off all their change flags:
*/
void resetStrip() {
  for (int pixel = 0; pixel < numPixels; pixel++) {
    pixelColor[pixel] = baseColor;
    change[pixel] = 0;
    strip.setPixelColor(pixel, pixelColor[pixel]);// set the color for this pixel
    strip.show();
  }
}
/*
  takes two colors and returns a color that's a point on each axis (r, g, b)
  closer to the target color
*/
unsigned long compare(unsigned long thisColor, unsigned long thatColor) {

  // separate the first color:
  byte r = thisColor >> 16;
  byte g = thisColor >>  8;
  byte b = thisColor;

  // separate the second color:
  byte targetR = thatColor >> 16;
  byte targetG = thatColor >>  8;
  byte targetB = thatColor;

  // fade the first color toward the second color:
  if (r > targetR) r--;
  if (g > targetG) g--;
  if (b > targetB) b--;

  if (r < targetR) r++;
  if (g < targetG) g++;
  if (b < targetB) b++;

  // combine the values to get the new color:
  unsigned long result = ((unsigned long)r << 16) | ((unsigned long)g << 8) | b;
  return result;
}
