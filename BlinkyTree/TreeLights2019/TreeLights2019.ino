#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
#include <WiFi101.h>
#include <WiFiMDNSResponder.h>
#include <RTCZero.h>

#include "arduino_secrets.h"

char mdnsName[] = "tree";
WiFiServer server(80);        // make an instance of the server class
WiFiMDNSResponder mdnsResponder;

const int controlPin = 7;     // the I/O pin that the neopixel data signal is on
const int pixelCount = 100;   // number of pixels in the tree
const int colorCount = 3;     // number of different colors on the tree

long lastPixelOff = 0;      // last time im millis that a pixel was shut off
int shutdownDelay = 0;      // shutdown delay before next pixel
int pixelsShutdown = 0;     // how many pixels have been shut down

// initalize a color converter:
ColorConverter converter;

// initialize the tree:
Adafruit_NeoPixel tree = Adafruit_NeoPixel(pixelCount, controlPin,
                         NEO_RGB + NEO_KHZ800);
// make an array of RGB colors:
unsigned long baseColor[pixelCount];

RGBColor colors[] = {converter.HSItoRGB(7, 95, 100),
                     converter.HSItoRGB(168, 100, 100),
                     converter.HSItoRGB(285, 22, 40)
                    };

String status = "";

RTCZero rtc;
int lastSecond;     // last second value, for watching passing seconds
int timeZone = -5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //  while (!Serial);
  tree.begin();
  tree.clear();          // clear the tree
  resetLights();         // reset the lights
  // while you're not connected to a WiFi AP:
  //  WiFi.beginProvision();
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while ( WiFi.status() != WL_CONNECTED) {
    // blink the led to show an unconnected status
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
  }
  server.begin();

  // Setup the MDNS responder to listen to the configured name.
  // NOTE: You _must_ call this _after_ connecting to the WiFi network and
  // being assigned an IP address.
  if (!mdnsResponder.begin(mdnsName)) {
    if (Serial) Serial.println("Failed to start MDNS responder!");
    while (1);
  }

  if (WiFi.status() == WL_CONNECTED) {
    // print the SSID of the network you're attached to:
    if (Serial) Serial.print("Connected to: ");
    if (Serial) Serial.println(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    if (Serial) Serial.print("IP Address: ");
    if (Serial) Serial.println(ip);
  }

  rtc.begin();
  // set the time from the network:
  unsigned long epoch;
  do {
    Serial.println("Attempting to get network time");
    epoch = WiFi.getTime();
    delay(1000);
  } while (epoch == 0);

  rtc.setEpoch(epoch);
  if (rtc.getHours() >= abs(timeZone)) {
    rtc.setHours(rtc.getHours() + timeZone);
  } else {
    rtc.setHours(24 + rtc.getHours() + timeZone);
  }
  Serial.println(getTimeStamp());

  rtc.setAlarmTime(0, 0, 0);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(crontab);
}

void loop() {
  int h = 10;         // hue
  int s = 255;        // saturation
  int i = 255;        // intensity

  mdnsResponder.poll();
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    if (Serial) Serial.println("new client");           // print a message out the serial port
    if (Serial) Serial.println(client.remoteIP());
    String currentLine = "";                // make a String to hold incoming data from the client
    if (client.connected()) {            // loop while the client's connected
      while (client.available()) {             // if there's bytes to read from the client,
        currentLine = client.readStringUntil('\n');             // read a byte, then
        currentLine.trim();                 // trim any return characters
        if (Serial) Serial.println(currentLine);                    // print it out the serial monitor

        if (currentLine.startsWith("GET /h/")) {
          String hue = currentLine.substring(6); // get the substring after the /
          h = hue.toFloat();             // convert to a number
          changeString(h, s, i);
        }

        if (currentLine.startsWith("GET /s/")) {
          String sat = currentLine.substring(6); // get the substring after the /
          s = sat.toFloat();             // convert to a number
          changeString(h, s, i);
        }

        if (currentLine.startsWith("GET /i/")) {
          String intensity = currentLine.substring(6); // get the substring after the /
          i = intensity.toFloat();             // convert to a number
          changeString(h, s, i);
        }
        // A message of /on starts the tree:
        if (currentLine.startsWith("GET /on")) {
          status = "on";
          resetLights();
        }

        // A message of /off stops the tree:
        if (currentLine.startsWith("GET /off")) {
          treeShutdown();
        }
        // the twinkle routine fades to white:
        if (currentLine.startsWith("GET /twinkle")) {
          twinkle();
        }

        if (currentLine.length() == 0) {
          sendResponse(client);
          break;
        }
      }
    }
    // close the connection:
    client.stop();
    if (Serial) Serial.println("client disconnected");
  }
  // fade pixels back to base color one point per 30ms:
  if (status == "on" && (millis() % 30 < 2)) {
    fadePixels();
  }
  // random twinkle. It's very rare:
  if (status == "on" && (random(50) == millis() % 1000)) {
    //    twinkle();
  }
  // update the tree:
  if (status == "on") tree.show();

  // shut down the next pixel of the tree:
  if (status == "shutting down") {
    if ((millis() - lastPixelOff > shutdownDelay) &&
        (pixelsShutdown < pixelCount)) {
      tree.setPixelColor(pixelsShutdown, 0);

      shutdownDelay = random(300) + 200;
      lastPixelOff = millis();
      pixelsShutdown++;
      if (Serial) Serial.println(pixelsShutdown);
      tree.show();
    }
    if (pixelsShutdown == pixelCount) {
      status = "off";
    }
  }
}
void treeShutdown() {
  status = "shutting down";
  shutdownDelay = random(300) + 200;
}
/*
  this function creates the twinkle effect:
*/
void twinkle() {
  int thisPixel = random(pixelCount);
  unsigned long currentColor = tree.getPixelColor(thisPixel);
  tree.setPixelColor(thisPixel, 0xFFEEEE);
}

// fade the pixels:
void fadePixels() {
  // iterate over all pixels:
  for (int thisPixel = 0; thisPixel < pixelCount; thisPixel++) {
    // fade the pixel one step from the current color to the target color:
    compare(thisPixel, tree.getPixelColor(thisPixel), baseColor[thisPixel]);
  }
}

/*
  takes two colors and returns a color that's a point on each axis (r, g, b)
  closer to the target color
*/
void compare(int thisPixel, unsigned long thisColor, unsigned long thatColor) {
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
  tree.setPixelColor(thisPixel, result);
}


// change the string to a single color:
void changeString(int hue, int sat, int intensity) {
  // create a single color from hue, sat, intensity:
  //  unsigned long color = tree.ColorHSV(hue, sat, intensity);

  RGBColor color = converter.HSItoRGB(hue, sat, intensity);

  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    tree.setPixelColor(pixel, color.red, color.green, color.blue);    // set the color for this pixel
  }
}

// reset the lights to a random array of the three colors:
void resetLights() {
  status = "on";
  // loop over all the pixels:
  for (int pixel = 0; pixel < pixelCount; pixel++) {
    int colorNumber = random(colorCount);
    RGBColor color = colors[colorNumber];
    baseColor[pixel]  = ((unsigned long)color.red << 16)
                        | ((unsigned long)color.green << 8)
                        | (color.blue);
    tree.setPixelColor(pixel, color.red, color.green, color.blue);    // set the color for this pixel
  }
  tree.show();
}


void sendResponse(WiFiClient client) {
  String response = "HTTP/1.1 200 OK\n";
  response += "Content-type:text/html\n";
  client.println();

  // the content of the HTTP response follows the header:
  client.print("<!DOCTYPE html><html lang=\"en\"><head>");
  client.print("</head><body>");
  client.print("<h1>" + status + "</h1>");
  client.print("Tree time: " + getTimeStamp() + "<br>");
  client.print("Click <a href=\"/on\">here</a> to turn on the tree on<br>");
  client.print("Click <a href=\"/off\">here</a> to turn the tree off<br>");
  client.print("</body></html>");

  // The HTTP response ends with another blank line:
  client.println();

}

// format the time as hh:mm:ss
String getTimeStamp() {
  String timestamp = "";
  if (rtc.getHours() <= 9) timestamp += "0";
  timestamp += rtc.getHours();
  timestamp += ":";
  if (rtc.getMinutes() <= 9) timestamp += "0";
  timestamp += rtc.getMinutes();
  timestamp += ":";
  if (rtc.getSeconds() <= 9) timestamp += "0";
  timestamp += rtc.getSeconds();
  timestamp += " GMT";
  return timestamp;
}

void crontab() {
  // at 6:15 AM, turn on
  if (rtc.getHours() == 6 &&
      rtc.getMinutes() == 15) {
    status == "on";
  }

  // at 8:45 AM, M-F turn off
  if (rtc.getHours() == 8 &&
      rtc.getMinutes() == 45) {
    status == "off";
  }
  // Except on Christmas day and Christmas eve
  if ((rtc.getDay() == 25 ||
       rtc.getDay() == 24) &&
      rtc.getMonth() == 12) {
    status == "on";
  }

  // at 4:45 PM, turn on
  if (rtc.getHours() == 16 &&
      rtc.getMinutes() == 45) {
    status == "on";
  }
  // at 11 PM, turn off
  if (rtc.getHours() == 22 &&
      rtc.getMinutes() == 45) {
    status == "off";
  }
  //# during weekend operating hours, reshuffle the lights:
  //30 6-7,17-22 * * 1-5 curl 'http://localhost/mailbox/on'
  //# reshuffle every hour on weekend:
  //30 7-22 * * 0,6 curl 'http://localhost/mailbox/on'
}
