/*
  NeoPixel RGB  fade

  This sketch  fades the RGB of a neoPixel device

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  Uses Interval library: https://github.com/tigoe/Interval
  
  created 31 Jan 2017
  modified 31 Jan 2017
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <Interval.h>

const int neoPixelPin = 5;
const int numPixels = 7;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRBW + NEO_KHZ800);

// changing range of keyframe colors for the pixels to flicker to:
unsigned long keyColors[] = {0xCB500F, 0xB4410C, 0x95230C, 0x853E0B};

unsigned long targetColor[numPixels];    // next target for each pixel
unsigned long pixelColor[numPixels];     // current color for each pixel

// count of keyframe colors:
int numColors = sizeof(keyColors) / 4;
// interval for flickering:
Interval flickerInterval;

void setup()  {
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn off pixel
  strip.show();   // refresh the strip
  flickerInterval.setInterval(flickerPixels, 30);
}

void loop() {
  flickerInterval.check();
   // update the strip:
  strip.show();
}


/*
  this function creates the flicker effect:
*/
void flickerPixels() {
  // iterate over all pixels:
  for (int thisPixel = 0; thisPixel < numPixels; thisPixel++) {
    // if the target color matches the current color for this pixel,
    // then pick a new target color randomly:
    if (targetColor[thisPixel] == pixelColor[thisPixel]) {
      int nextColor = random(numColors);
      targetColor[thisPixel] = keyColors[nextColor];
    }
    // fade the pixel one step from the current color to the target color:
    pixelColor[thisPixel] = compare(pixelColor[thisPixel], targetColor[thisPixel]);
    // set the pixel color in the strip:
    strip.setPixelColor(thisPixel, pixelColor[thisPixel]);// set the color for this pixel
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

