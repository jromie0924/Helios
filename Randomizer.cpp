#include "Randomizer.h"
#include "Arduino.h"
#include <Time.h>

Randomizer::Randomizer() {
  // Instantiate all arrays.
  // Daylight
  day1 = {209, 100, 39};
  day2 = {220, 97, 34};
  day3 = {250, 120, 34};
  day4 = {250, 150, 34};
  day5 = {250, 115, 30};
  
  // Sunset
  ss1 = {245, 90, 20};
  ss2 = {245, 60, 20};
  ss3 = {245, 55, 15};
  // These last two are extremely red.
  // Neither are to be used as a "main" color.
  ss4 = {200, 40, 5};
  ss5 = {245, 25, 5};
  
  // Set the time for the random seed.
  // Any time will do.
  setTime(9, 24, 00, 11, 59, 2015);
  
  // Set the random seed.
  srand(now());
}

void Randomizer::randomize(int curR, int curG, int curB) {
  int numPixels = rand() % 3 + 1; //max of 3 pixels can "flare" at a time (min of 1).
  int *colors = new int[numPixels][3];
  for(int a = 0; a < numPixels; a++) {
    int changeOrDim = rand() % 2; // 0 or 1
    if(changeOrDim == 0) { // Dim and change
      int randCol = rand() % 5 + 1; // select a color
      int red, green, blue;
      switch (randCol) {
        case 1:
          red = day1[0]; green = day1[1]; blue = day1[2];
          break;
          
        case 2:
          red = day2[0]; green = day2[1]; blue = day2[2];
          break;
          
        case 3:
          red = day3[0]; green = day3[1]; blue = day3[2];
          break;
          
        case 4:
          red = day4[0]; green = day4[1]; blue = day4[2];
          break;
          
        case 5:
          red = day5[0]; green = day5[1]; blue = day5[2];
          break;
          
        default:
          return;
      }
    }
  }
}

void Randomizer::flarePix(int rCol, int pix) {
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
      fadeToColor(col, nextCol, pix);
    }
  }
}





