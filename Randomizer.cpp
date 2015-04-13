#include <IRremote.h>
#include <IRremoteInt.h>
#include <Adafruit_NeoPixel.h>
#include "Randomizer.h"
#include "Arduino.h"
#define filterVal 581859881
//#include <Time.h>

Randomizer::Randomizer(Adafruit_NeoPixel& strip) { // Setting everything up.
  // Change to the appropriate percentage of the original brightness of pixels.
  // This is the value that determines how dim the pixels get dimmed when/if they are selected for such functionality.
  dimPerc = 0.4;
  strip.begin();
  strip.show(); // clear the strip (set all pixels to 'off')
  stateFlag = 0;

  // Instantiate all arrays.
  // Daylight
  dayP[0] = (int)(dimPerc * 209); dayP[1] = (int)(dimPerc * 100); dayP[2] = (int)(dimPerc * 39); // This is the primary day color. The preceding colors are for random selection.
  day1[0] = 220; day1[1] = 97; day1[2] = 34;
  day2[0] = 250; day2[1] = 120; day2[2] = 34;
  day3[0] = 250; day3[1] = 150; day3[2] = 34;
  day4[0] = 250; day4[1] = 115; day4[2] = 30;
  //day1 = {209, 100, 39};
  //day2 = {220, 97, 34};
  //day3 = {250, 120, 34};
  //day4 = {250, 150, 34};
  //day5 = {250, 115, 30};

  // Sunset
  /*
  ss1 = {245, 90, 20};
  ss2 = {245, 60, 20};
  ss3 = {245, 55, 15};
  // These last two are extremely red.
  // Neither are to be used as a "main" color.
  ss4 = {200, 40, 5};
  ss5 = {245, 25, 5};
  */
  // Set the time for the random seed.
  // Any time will do.
  //setTime(9, 24, 00, 11, 59, 2015);

  // Set the random seed.
  randomSeed(analogRead(0));
}

bool Randomizer::powerOn(Adafruit_NeoPixel& strip, int wait, IRrecv& irrecv, decode_results& results) {
  int r = 209,
      g = 100,
      b = 39;
  stateFlag = 1;
  int lim = 100;
  int rnew, gnew, bnew;
  for (int a = 0; a <= 100; a++) {
    if (irrecv.decode(&results)) {
      if (results.value == filterVal) {
        delay(500);
        irrecv.resume();
        int retVal = changeState(stateFlag, strip);
      }
    }
    double percentage = (double)a / 100;
    int red_ = percentage * r;
    int green_ = percentage * g;
    int blue_ = percentage * b;

    for (int i = 0; i <= strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(red_, green_, blue_));
    }
    strip.show();
    delay(wait);
  }

  delay(500);

  for (int i = 0; i <= lim; i++) {
    rnew = r + (dayP[0] - r) * i / lim;
    gnew = g + (dayP[1] - g) * i / lim;
    bnew = b + (dayP[2] - b) * i / lim;
    for (int n = 0; n < strip.numPixels(); n++) {
      strip.setPixelColor(n, strip.Color(rnew, gnew, bnew));
      strip.show();
    }
    if (irrecv.decode(&results)) {
      if (results.value == filterVal) {
        irrecv.resume();
        delay(500);
        powerOff(strip);
        return true;
      }
    }
    delay(30);
  }
  return false;
}

bool Randomizer::randomize(Adafruit_NeoPixel& strip, IRrecv& irrecv, decode_results& results) {
  int numPixels = random(15) + 11; // max of 20 pixels can "flare" at a time (min of 10).
  int colors [numPixels][3];
  int pixels [numPixels];
  for (int a = 0; a < numPixels; a++) {
    int changeOrDim = random(2); // 0 or 1
    double dimmer = 1;
    if (changeOrDim == 1) {
      //dimmer = dimPerc;
    }

    int randCol = rand() % 4 + 1; // select a color [1, 4]
    int red, green, blue;
    switch (randCol) {
      case 1:
        red = (int)(dimmer * day1[0]); green = (int)(dimmer * day1[1]); blue = (int)(dimmer * day1[2]);
        break;

      case 2:
        red = (int)(dimmer * day2[0]); green = (int)(dimmer * day2[1]); blue = (int)(dimmer * day2[2]);
        break;

      case 3:
        red = (int)(dimmer * day3[0]); green = (int)(dimmer * day3[1]); blue = (int)(dimmer * day3[2]);
        break;

      case 4:
        red = (int)(dimmer * day4[0]); green = (int)(dimmer * day4[1]); blue = (int)(dimmer * day4[2]);
        break;

      default:
        break;
    }
    colors[a][0] = red; colors[a][1] = green; colors[a][2] = blue;
    // call flarePix() with the color we just determined.
  }
  bool isDistinct = false;
  while (!isDistinct) {
    isDistinct = true;
    for (int i = 0; i < numPixels; i++) {
      pixels[i] = random(45) + 1;
    }
    for (int i = 0; i < numPixels; i++) {
      for (int k = numPixels - 1; k > i; k--) {
        if (pixels[i] == pixels[k])
          isDistinct = false;
      }
    }
  }
  
  int switchOff = 0;
  for (int a = 0; a < numPixels; a++) {
    switchOff = fadeToColor(dayP, colors[a], pixels[a], strip, irrecv, results);
    if(switchOff == 1) {
      Serial.println("here");
      changeState(3, strip);
    }
    //changeState(stateFlag, strip);
    delay(100);
  }
  delay(10);
  for (int a = 0; a < numPixels; a++) {
    switchOff = fadeToColor(colors[a], dayP, pixels[a], strip, irrecv, results);
    if(switchOff == 1) {
      Serial.println("here");
      changeState(3, strip);
    }
    //changeState(stateFlag, strip);
    delay(100);
  }
  return false;
}

uint8_t Randomizer::splitColor( uint32_t c, char value )
{
  switch ( value ) {
    case 'r': return (uint8_t)(c >> 16);
    case 'g': return (uint8_t)(c >>  8);
    case 'b': return (uint8_t)(c >>  0);
    default:  return 0;
  }
}

int Randomizer::fadeToColor(int start[3], int end_[3], int pix, Adafruit_NeoPixel& strip, IRrecv& irrecv, decode_results& results) {
  int n = 70;
  int rnew = 0, gnew = 0, bnew = 0;
  for (int i = 0; i <= n; i++) {
    if (irrecv.decode(&results)) {
      if (results.value == filterVal) {
        irrecv.resume();
        delay(500);
        //Serial.println("changing state");
        int retVal = changeState(stateFlag, strip);
        //powerOff(strip);
        //return true;
      }
    }
    rnew = start[0] + (end_[0] - start[0]) * i / n;
    gnew = start[1] + (end_[1] - start[1]) * i / n;
    bnew = start[2] + (end_[2] - start[2]) * i / n;
    strip.setPixelColor(pix, strip.Color(rnew, gnew, bnew));
    strip.show();
    delay(5);
  }
  return 0;
}

bool Randomizer::powerOff(Adafruit_NeoPixel& strip) {
  int r, g, b;
  for (int a = 100; a >= 0; a--) {
    double percentage = (double)a / 100;
    for (int k = 0; k < strip.numPixels(); k++) {
      r = (int)(splitColor(strip.getPixelColor(k), 'r') * percentage);
      g = (int)(splitColor(strip.getPixelColor(k), 'g') * percentage);
      b = (int)(splitColor(strip.getPixelColor(k), 'b') * percentage);
      strip.setPixelColor(k, strip.Color(r, g, b));
    }
    strip.show();
  }
  return true;
}

int Randomizer::getStateFlag() {
  return stateFlag;
}

int Randomizer::changeState(int flag, Adafruit_NeoPixel& strip) {
  switch (flag) {
    case 0:
      dayP[0] = (int)(dimPerc * 209); dayP[1] = (int)(dimPerc * 100); dayP[2] = (int)(dimPerc * 39);
      day1[0] = 220; day1[1] = 97; day1[2] = 34;
      day2[0] = 250; day2[1] = 120; day2[2] = 34;
      day3[0] = 250; day3[1] = 150; day3[2] = 34;
      day4[0] = 250; day4[1] = 115; day4[2] = 30;
      stateFlag = 1;
      break;

    case 1:
      dayP[0] = (int)(dimPerc * 245); dayP[1] = (int)(dimPerc * 90); dayP[2] = (int)(dimPerc * 20);
      day1[0] = 245; day1[1] = 60; day1[2] = 20;
      day2[0] = 245; day2[1] = 55; day2[2] = 15;
      day3[0] = 200; day3[1] = 40; day3[2] = 5;
      day4[0] = 245; day4[1] = 25; day4[2] = 5;
      stateFlag = 2;
      break;

    case 2:
      dayP[0] = (int)(dimPerc * 209); dayP[1] = (int)(dimPerc * 150); dayP[2] = (int)(dimPerc * 150);
      day1[0] = 250; day1[1] = 115; day1[2] = 30;
      day2[0] = 245; day2[1] = 170; day2[2] = 100;
      day3[0] = 190; day3[1] = 170; day3[2] = 200;
      day4[0] = 135; day4[1] = 180; day4[2] = 245;
      stateFlag = 3;
      break;
      
    case 3:
      powerOff(strip);
      break;

    default:
      return -1;
      break;
  }
  return 1;
}

