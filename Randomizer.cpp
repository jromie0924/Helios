#include <Adafruit_NeoPixel.h>
#include "Randomizer.h"
#include "Arduino.h"
//#include <Time.h>

Randomizer::Randomizer(Adafruit_NeoPixel& strip) { // Setting everything up.
  // Change to the appropriate percentage of the original brightness of pixels.
  // This is the value that determines how dim the pixels get dimmed when/if they are selected for such functionality.
  dimPerc = 0.4;
  strip.begin();
  strip.show(); // clear the strip (set all pixels to 'off')

  // Instantiate all arrays.
  // Daylight
  dayP[0] = (int)(dimPerc * 209); dayP[1] = (int)(dimPerc * 150); dayP[2] = (int)(dimPerc * 150); // This is the primary day color. The preceding colors are for random selection.
  day1[0] = 20; day1[1] = 90; day1[2] = 100;
  day2[0] = 245; day2[1] = 170; day2[2] = 100;
  day3[0] = 190; day3[1] = 170; day3[2] = 200;
  day4[0] = 145; day4[1] = 190; day4[2] = 245;
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

void Randomizer::powerOn(Adafruit_NeoPixel& strip, int wait) {
  int r = 209,
      g = 150,
      b = 150;
  //int r = dayP[0], g = dayP[1], b = dayP[2];
  int lim = 100;
  int rnew, gnew, bnew;
  for (int a = 0; a <= 100; a++) {
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
    delay(30);
  }

}

void Randomizer::randomize(Adafruit_NeoPixel& strip) {
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
        return;
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
  }/*
  for (int n = 0; n < numPixels; n++) {
    strip.setPixelColor(pixels[n], strip.Color(colors[n][0], colors[n][1], colors[n][2]));
    strip.show();
  }
  delay(500);
  for(int n = 0; n < numPixels; n++) {
    strip.setPixelColor(pixels[n], strip.Color(dayP[0], dayP[1], dayP[2]));
  }*/

  for (int a = 0; a < numPixels; a++) {
    fadeToColor(dayP, colors[a], pixels[a], strip);
    delay(100);
  }
  delay(10);
  for (int a = 0; a < numPixels; a++) {
    fadeToColor(colors[a], dayP, pixels[a], strip);
    delay(100);
  }
}

void Randomizer::flarePix(int rCol[3], int pix, Adafruit_NeoPixel& strip) {
  /*
    int randColor = rand() % 3 + 1;
    int howManyPixels = rand() % 2 + 1;
    if (howManyPixels == 1) {
      int pix = rand() % strip.numPixels();
      int randColor = rand() % 3 + 1;
      int *col = new int[3];
      switch (randColor) {
        case 1:
          col[0] = day1[0];
          col[1] = day1[1];
          col[2] = day1[2];
      }
      int r = splitColor(strip.getPixelColor(pix), 'r');
      int g = splitColor(strip.getPixelColor(pix), 'g');
      int b = splitColor(strip.getPixelColor(pix), 'b');
      if (!(r == col[0] && g == col[1] && b == col[2])) {
        int *nextCol = new int[3];
        nextCol[0] = r;
        nextCol[1] = g;
        nextCol[2] = b;
        fadeToColor(col, nextCol, pix, strip);
      }
    }*/
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

void Randomizer::fadeToColor(int start[3], int end_[3], int pix, Adafruit_NeoPixel& strip) {
  int n = 70;
  int rnew = 0, gnew = 0, bnew = 0;
  for (int i = 0; i <= n; i++) {
    rnew = start[0] + (end_[0] - start[0]) * i / n;
    gnew = start[1] + (end_[1] - start[1]) * i / n;
    bnew = start[2] + (end_[2] - start[2]) * i / n;
    strip.setPixelColor(pix, strip.Color(rnew, gnew, bnew));
    strip.show();
    delay(5);
  }
}



