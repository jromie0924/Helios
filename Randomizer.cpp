#include <Adafruit_NeoPixel.h>
#include "Randomizer.h"
#include "Arduino.h"
#include <Time.h>

Randomizer::Randomizer(Adafruit_NeoPixel& strip) { // Setting everything up.
  PIN = 6;
  dimPerc = 0.5;
  strip.begin();
  strip.show(); // clear the strip (set all pixels to 'off')
  
  // Instantiate all arrays.
  // Daylight
  day1[0] = 209; day1[1] = 100; day1[2] = 39;
  day2[0] = 220; day2[1] = 97; day2[2] = 34;
  day3[0] = 250; day3[1] = 120; day3[2] = 34;
  day4[0] = 250; day4[1] = 150; day4[2] = 34;
  day5[0] = 250; day5[1] = 115; day5[2] = 30;
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
  setTime(9, 24, 00, 11, 59, 2015);
  
  // Set the random seed.
  srand(now());
}

void Randomizer::randomize(int curR, int curG, int curB, Adafruit_NeoPixel& strip) {
  int numPixels = rand() % 3 + 1; //max of 3 pixels can "flare" at a time (min of 1).
  int colors [numPixels][3];
  for(int a = 0; a < numPixels; a++) {
    int changeOrDim = rand() % 2; // 0 or 1
    if(changeOrDim == 0) { // Dim and change
      int randCol = rand() % 5 + 1; // select a color
      int red, green, blue;
      switch (randCol) {
        case 1:
          red = (int)(dimPerc * day1[0]); green = (int)(dimPerc * day1[1]); blue = (int)(dimPerc * day1[2]);
          break;
          
        case 2:
          red = (int)(dimPerc * day2[0]); green = (int)(dimPerc * day2[1]); blue = (int)(dimPerc * day2[2]);
          break;
          
        case 3:
          red = (int)(dimPerc * day3[0]); green = (int)(dimPerc * day3[1]); blue = (int)(dimPerc * day3[2]);
          break;
          
        case 4:
          red = (int)(dimPerc * day4[0]); green = (int)(dimPerc * day4[1]); blue = (int)(dimPerc * day4[2]);
          break;
          
        case 5:
          red = (int)(dimPerc * day5[0]); green = (int)(dimPerc * day5[1]); blue = (int)(dimPerc * day5[2]);
          break;
          
        default:
          return;
      }
      // call flarePix() with the color we just determined.
    }
  }
}

void Randomizer::flarePix(int rCol[3], int pix, Adafruit_NeoPixel& strip) {
  int randColor = rand() % 3 + 1;
  int howManyPixels = rand() % 2 + 1;
  if(howManyPixels == 1) {
    int pix = rand() % strip.numPixels();
    int randColor = rand() % 3 + 1;
    int *col = new int[3];
    switch(randColor) {
      case 1:
        col[0] = day1[0];
        col[1] = day1[1];
        col[2] = day1[2];
    }
    int r = splitColor(strip.getPixelColor(pix), 'r');
    int g = splitColor(strip.getPixelColor(pix), 'g');
    int b = splitColor(strip.getPixelColor(pix), 'b');
    if(!(r == col[0] && g == col[1] && b == col[2])) {
      int *nextCol = new int[3];
      nextCol[0] = r;
      nextCol[1] = g;
      nextCol[2] = b;
      fadeToColor(col, nextCol, pix, strip);
    }
  }
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
  int n = 100;
  int rnew = 0, gnew = 0, bnew = 0;
  for(int i = 0; i <= n; i++) {
    rnew = start[0] + (end_[0] - start[0]) * i / n;
    gnew = start[1] + (end_[1] - start[1]) * i / n;
    bnew = start[2] + (end_[2] - start[2]) * i / n;
    strip.setPixelColor(pix, strip.Color(rnew, gnew, bnew));
    strip.show();
    delay(20);
  }
}



