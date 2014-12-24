/*
  BlinkyTree
  This version maintains:
  * candle flicker effect
  * the slow fade to reddish orange over several hours
  * twinkle effect
  * capacitive touch sensor to trigger the twinkle effect
  * an array to hold only the pixels visible from the front of the tree
  * twinkle lights are picked only from the visible pixel array
  * serial input changed to serial from linux on Yun (Serial1)
  * turn on and off using serial commands '~1' and '~0'
  * bug fix in reset command

  It adds:
  * major code changes for stability
  * fixed bug at end of cycle

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 4 Dec 2014
  updated 25 Dec 2014
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
#include <Bridge.h>

#define PIN 12  // the I/O pin that the neopixel data signal is on

CapacitiveSensor bell = CapacitiveSensor(10, 7);   // initialize cap touch library
long touchTime = 0;                      // time of last touch
int touchState = 0;                      // if the bell is being touched
int threshold = 1000;                    // touch sensitivity threshold
int touchDelay = 500;                    // how long to delay before resetting touch sensor

const int numPixels = 50;    // number of pixels in the strip
// initialize the strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_RGB + NEO_KHZ800);

unsigned long targetColor[numPixels];    // next target for each pixel
unsigned long pixelColor[numPixels];     // current color for each pixel

// range of keyframe colors for the pixels to flicker to:
unsigned long keyColors[] = {0xCB500F, 0xB4410C, 0x95230C, 0x853E0B};
// initial reference color range:
unsigned long referenceColors[] = {0xCB5D0F, 0xB4470C, 0x95310C, 0x854E0B};

int visiblePixels[] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 40}; // pixels visible on the front of the tree
int visibleCount = sizeof(visiblePixels) / 2;

// count of keyframe colors:
int numColors = sizeof(keyColors) / 4;
unsigned long slowFadeInterval =  169400;   // in millis, this makes an 8-hour fade
unsigned long lastFade = 0;           // timestamp of the last call to fadeToRed()

boolean running = true;    // whether or not the lights are running
boolean finished = false;

void setup() {
  Serial1.begin(250000);  // initialize serial communication with linux
  Serial1.setTimeout(10);
  Console.begin();

  bell.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off touch sensor autocalibrate
  strip.begin();          // initialize pixel strip
  resetStrip();           // reset the strip
}

void loop() {
  // read serial input:
  while (Serial1.available() > 0) {
    if (Serial1.find("~")) {    // looking for ~ first allows for filtering out the dmesg
      char input = Serial1.read();
      if (input == '0' ) {    // 0 signals shutdown
        running = turnOff();
      }

      if (input == '1') {
        running = resetStrip();
      }
    }
  }

  if (running) {
    // create the flicker effect every 30ms:
    if (millis() % 30 < 2) {
      flickerPixels();
    }

    // gradually fade the keyframe colors lower and more toward the red:
    if (millis() - lastFade >= slowFadeInterval) {
      finished = fadeToRed();
    }
  }

  // if you reached the end of the long fade, turn off:
  if (finished) running = turnOff();


  // read touch sensor:
  long bellTouch =  bell.capacitiveSensor(30);
  // if the bell is touched and it wasn't touched
  // on the last check:
  if (bellTouch > threshold && touchState == 0) {
    touchState = 1;
    twinkle();
    touchTime = millis();
  }

  // if the touch delay has passed, reset touchState:
  if (millis() - touchTime > touchDelay) {
    touchState = 0;
  }
}

/*
  this function creates the twinkle effect:
*/
void twinkle() {
  int whichPixel = random(visibleCount);
  int thisPixel = visiblePixels[whichPixel]; // pick a random pixel from the visible list
  pixelColor[thisPixel] = 0xFFDDDD;          // set its color to white
  Serial1.println("ding");
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

  // refresh the strip:
  strip.show();
}

/*
  This function fades all the key colors toward a low reddish orange
*/
boolean fadeToRed() {
  boolean result = true;
  // iterate over all pixels:
  for (int thisColor = 0; thisColor < numColors; thisColor++) {
    // separate the  color:
    byte r = keyColors[thisColor] >> 16;
    byte g = keyColors[thisColor] >>  8;
    byte b = keyColors[thisColor];

    // the reddish-orange glow you're aiming for is 0x1F0F02
    // fade the first color toward the second color:
    if (r > 0x1F) r--;
    if (g > 0x0F) g--;
    if (b > 0x02) b--;

    // combine the values to get the new color:
    keyColors[thisColor] = ((unsigned long)r << 16) | ((unsigned long)g << 8) | b;
    // if all the keyColors == the final color then you're finished:
    if (keyColors[thisColor] != 0x1F0F02) {
      result = false;
    }
  }
  lastFade = millis();

  return result;
}

/*
  This function resets all the pixels to their base colors:
*/
boolean resetStrip() {
  // iterate over the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {
    // pick two random colors from the keyframe colors:
    int nextColor = random(numColors);
    int lastColor = random(numColors);
    targetColor[pixel] = referenceColors[nextColor];
    pixelColor[pixel] = referenceColors[lastColor];
    // set the pixel color:
    strip.setPixelColor(pixel, pixelColor[pixel]);// set the color for this pixel
  }

  // reset the keyColor array with the reference color array:
  for (int p = 0; p < numColors; p++) {
    keyColors[p] = referenceColors[p];
  }

  // update the strip:
  strip.show();
  return true;
}

boolean turnOff() {
  // iterate over the pixels:
  for (int pixel = 0; pixel < numPixels; pixel++) {

    // set the pixel color:
    strip.setPixelColor(pixel, 0);// set the color for this pixel
    strip.show();
    delay(300);
  }
  // update the strip:

  return false;
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
