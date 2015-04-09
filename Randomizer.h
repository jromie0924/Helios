#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include "Arduino.h"

//using namespace std;

class Randomizer {
  private:
    // Daylight Colors
    //Dynamically-allocated Arrays
    int day1 [3];
    int day2 [3];
    int day3 [3];
    int day4 [3];
    int day5 [3];
    
    // Sunset Colors
    // Dynamically-allocated Arrays
    int ss1 [3];
    int ss2 [3];
    int ss3 [3];
    int ss4 [3];
    int ss5 [3];
    
    void flarePix(int, int); // for one pixel
    void flarePix(int, int, int); // for two pixels
    void flarePix(int, int, int, int); // for three pixels
    
  public:
    Randomizer();
    void randomize(int, int, int);
};

#endif
