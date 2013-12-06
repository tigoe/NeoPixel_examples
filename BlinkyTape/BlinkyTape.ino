/*
  Simple BlinkyStrip control
  
  This sketch allows serial control over a BlinkyStrip.
  To set any pixel in the strip, send the following serial string:
  
  Cr,g,b
  
  r, g, and b are ASCII numbers for the red, green, and blue brightness
  levels, from 0 to 255.
  
  This will probably work for any string of NeoPixels from Adafruit as well.
  
  Uses Adafruit's NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
  
  created 4 Dec 2014
  by Tom Igoe

*/
#include <Adafruit_NeoPixel.h>

#define PIN 13

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

int pixel = 0;            // pixel number that you're changing
int red = 0;              // red value 
int green = 34;           // green value
int blue = 12;            // blue value

void setup() {
  Serial.begin(9600);     // initialize serial communication
  Serial.setTimeout(10);  // set serial timeout
  strip.begin();          // initialize pixel strip
  strip.show();           // Initialize all pixels to 'off'
}

void loop() {
  // listen for serial:
  if (Serial.available() > 0) {
    if (Serial.read() == 'C') {    // string should start with C
      pixel = Serial.parseInt();   // then an ASCII number for pixel number
      red = Serial.parseInt();     // then an ASCII number for red
      green = Serial.parseInt();   // then an ASCII number for green
      blue = Serial.parseInt();    // then an ASCII number for blue
    }
  }
  
  strip.setPixelColor(pixel, red, green, blue);// set the color for this pixel
  strip.show();                                // refresh the strip
}



