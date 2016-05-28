
/*
  This sketch makes neoPixels chase, fading each pixel as it goes.

  created 28 March 2016
  by Tom Igoe
*/
#include <Adafruit_NeoPixel.h>

const int neoPixelPin = 6;  // control pin
const int numPixels = 8;    // number of pixels

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_RGB + NEO_KHZ800);

// changing range of keyframe colors for the pixels to flicker to,
// in RGB hex:
unsigned long keyColors[] = {0x2300DE, 0x1512ED, 0x0034D9, 0x2315FF};

unsigned long targetColor[numPixels];    // next target for each pixel
unsigned long pixelColor[numPixels];     // current color for each pixel

// count of keyframe colors:
int numColors = sizeof(keyColors) / 4;
int flickerInterval = 100;     // in millis, the delay between flicker steps


void setup()  {
  strip.begin();                          // initialize pixel strip
  for (int pixel = 0; pixel < numPixels; pixel++) {
    strip.clear();  // turn off pixel
  }
  strip.show();                         // refresh the strip
}

void loop() {
  // create the flicker effect:
  if (millis() % flickerInterval < 2) {
    flickerPixels();
  }

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

