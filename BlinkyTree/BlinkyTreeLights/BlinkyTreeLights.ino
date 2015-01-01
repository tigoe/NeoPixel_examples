/*
  BlinkyTree
  This version maintains:
  * candle flicker effect
  * the slow fade to reddish orange over several hours
  * twinkle effect
  * capacitive touch sensor to trigger the twinkle effect
  * an array to hold only the pixels visible from the front of the tree
  * twinkle lights are picked only from the visible pixel array
  * mailbox support for web interface
  * mailbox support for fade time and fastForward

  It adds:
   * various big fixes

  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel

  created 4 Dec 2014
  updated 31 Dec 2014
  by Tom Igoe
*/

#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>
#include <Bridge.h>
#include <Mailbox.h>

const int controlPin = 12;  // the I/O pin that the neopixel data signal is on
const int numPixels = 50;    // number of pixels in the strip
// initialize the strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, controlPin, NEO_RGB + NEO_KHZ800);

CapacitiveSensor bell = CapacitiveSensor(10, 7);   // initialize cap touch library
long touchTime = 0;                      // time of last touch
int touchState = 0;                      // if the bell is being touched
int threshold = 1000;                    // touch sensitivity threshold
int touchDelay = 500;                    // how long to delay before resetting touch sensor

// initial reference color range:
unsigned long referenceColors[] = {0xCB5D0F, 0xB4470C, 0x95310C, 0x854E0B};
// the final color of all pixels before they flicker out:
unsigned long finalColor = 0x1F0F02;
// changing range of keyframe colors for the pixels to flicker to:
unsigned long keyColors[] = {0xCB500F, 0xB4410C, 0x95230C, 0x853E0B};

unsigned long targetColor[numPixels];    // next target for each pixel
unsigned long pixelColor[numPixels];     // current color for each pixel
int visiblePixels[] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 40}; // pixels visible on the front of the tree
int visibleCount = sizeof(visiblePixels) / 2;


// count of keyframe colors:
int numColors = sizeof(keyColors) / 4;
unsigned long slowFadeInterval =  169400;   // in millis, the time between fade changes
unsigned long lastFade = 0;                 // timestamp of the last call to fadeToRed()
int flickerInterval = 30;                   // in millis, the delay between flicker steps

boolean running = true;    // whether or not your're running
boolean finished = false;  // whether or not the slow fade is finished

void setup() {
  Bridge.begin();         // initialize Bridge
  Mailbox.begin();        // initialize Mailbox
  strip.begin();          // initialize pixel strip
  resetStrip();           // start the long fade
}

void loop() {
  String message;
  // read the next message present in the queue
  while (Mailbox.messageAvailable()) {
    Mailbox.readMessage(message);
    if (message == "off" ) {
      running = turnOff();
    }

    if (message == "on") {
      running = resetStrip();
    }

    if (message == "twinkle") {
      twinkle();
    }

    if (message.startsWith("fade/")) {
      String time = message.substring(5);        // get the substring after the /
      long minutes = time.toFloat();             // convert to a number
      slowFadeInterval = calcInterval(minutes);  // calculate the interval for slow fade
    }

    if (message.startsWith("ff/")) {
      String completion =  message.substring(3);    // get the substring after the /
      int completionPercent = completion.toFloat(); // convert to a number
      fastForward(completionPercent);               // fast forward to the right %
    }

    if (message == "hny") {
      happyNewYear();
    }
  }


  if (running) {      // continue the long fade
    // create the flicker effect:
    if (millis() % flickerInterval < 2) {
      flickerPixels();
    }

    // gradually fade the keyframe colors lower and more toward the red:
    if (millis() - lastFade >= slowFadeInterval) {
      finished = fadeToRed();
    }
  }
  // when you reach the final colors, turn off:
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

  // update the strip:
  strip.show();
}

/*
  this function creates the twinkle effect:
*/
void twinkle() {
  int whichPixel = random(visibleCount);
  int thisPixel = visiblePixels[whichPixel]; // pick a random pixel from the visible list
  pixelColor[thisPixel] = 0xFFDDDD;          // set its color to white
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
  This function fades all the key colors toward a low reddish orange
*/
boolean fadeToRed() {
  boolean result = true;
  // iterate over all pixels:
  for (int thisColor = 0; thisColor < numColors; thisColor++) {
    // calculate a new keyColor closer to the final color:
    keyColors[thisColor] = compare(keyColors[thisColor], finalColor);

    // if all the keyColors == the final color then you're finished:
    if (keyColors[thisColor] != finalColor) {
      result = false;
    }
  }
  lastFade = millis();
  return result;
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
  finished = false;
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
  return false;
}

/*
  This function calculates the interval for the fadeToRed() steps
*/
unsigned long calcInterval(long fadeMinutes) {
  if (fadeMinutes <= 0) return 0;
  unsigned long result = 0;
  long steps = calcFadeSteps();

  // return total fade time / numSteps
  result = (fadeMinutes * 60 * 1000) / steps;
  return result;
}

/*
  This function calculates the fadeToRed() steps
*/
long calcFadeSteps() {
  // separate the final color:
  byte targetR = finalColor >> 16;
  byte targetG = finalColor >>  8;
  byte targetB = finalColor;

  long numSteps = 0;
  // loop over the reference colors
  for (int thisColor = 0; thisColor < numColors; thisColor++) {
    // separate into R, G, B
    byte r = referenceColors[thisColor] >> 16;
    byte g = referenceColors[thisColor] >>  8;
    byte b = referenceColors[thisColor];

    // calculate max of reference color - final color:
    int redDifference = abs(r - targetR);
    numSteps = max(redDifference, numSteps);
    int greenDifference = abs(g - targetG);
    numSteps = max(greenDifference, numSteps);
    int blueDifference = abs(b - targetB);
    numSteps = max(blueDifference, numSteps);
  }
  return numSteps;
}

/*
  This function fastforwards to a given percentage
  of completion of the fadeToRed cycle
*/
void fastForward(int targetPercent) {
  if (targetPercent <= 0) return;    // if targetPercent isn't valid, return

  resetStrip();                      // reset the color values
  long steps = calcFadeSteps();      // calculate how many steps to take

  // calculate how many steps to take:
  long targetStep = (steps * (100 - targetPercent)) / 100;
  // step through that many steps:
  while (steps > targetStep) {
    fadeToRed();        // fade the colors
    steps--;            // decrement steps
  }
}

/*
  Added as a surprise effect and triggered with a cron job
  for New Year's Eve celebration 2014. This was all written hastily
  and never tested until it ran on New Year's.

*/
void happyNewYear() {
  // make all the pixels white:
  for (int times = 0; times < 5; times++) {
    // iterate over the pixels:
    for (int pixel = 0; pixel < numPixels; pixel++) {
      // set the pixel color:
      strip.setPixelColor(pixel, 0xFFDDDD);// set the color for this pixel
      strip.show();
      delay(300);
    }
    // change the reference colors to ones in the blue/purple range:
    referenceColors[0] = 0xA262E3;
    referenceColors[1] = 0xB4075C;
    referenceColors[2] = 0xB03E18;
    referenceColors[3] = 0xBA1530;

    // Change the final color to blue:
    finalColor = 0x0407DB;

    // set all the pixels to the new reference colors,
    // one at a time:
    for (int pixel = 0; pixel < numPixels; pixel++) {
      // set the pixel color:
      int d = random(numColors);
      unsigned long thisColor = referenceColors[d];
      strip.setPixelColor(pixel, thisColor);// set the color for this pixel
      strip.show();
      delay(300);
    }
    // reset the strip:
    resetStrip();
    
    // for 500 times, make a random twinkle; didn't really work
    int c = 500;
    while (c > 0) {
      int pause = random(1000) + 300;
      if (millis() % pause < 2) {
        twinkle();
        c--;
      }
      flickerPixels();
      strip.show();
    }
  }
}
