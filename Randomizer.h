#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <IRremote.h>
#include <IRremoteInt.h>
#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

//using namespace std; 

class Randomizer {
  private:
    double dimPerc;
    
    //Adafruit_NeoPixel strip;
    
    // Daylight Colors
    //Arrays
    int dayP [3];
    int day1 [3];
    int day2 [3];
    int day3 [3];
    int day4 [3];
    
    // Indicates the current state of the light.
    // 0 = Off
    // 1 = Daylight
    // 2 = Sunset
    // 3 = Night
    int stateFlag;
    
    // Sunset Colors
    // Arrays
    /*
    int ss1 [3];
    int ss2 [3];
    int ss3 [3];
    int ss4 [3];
    int ss5 [3];
    */
    void flarePix(int[3], int, Adafruit_NeoPixel&); // for one pixel
    //void flarePix(int, int, int); // for two pixels
    //void flarePix(int, int, int, int); // for three pixels
    uint8_t splitColor(uint32_t, char);
    
  public:
    Randomizer(Adafruit_NeoPixel&);
    bool randomize(Adafruit_NeoPixel&, IRrecv&, decode_results&);
    void fadeToBaseColor(int[3], int[3], int, Adafruit_NeoPixel&);
    int fadeToColor(int[3], int[3], int, Adafruit_NeoPixel&, IRrecv&, decode_results&);
    void fadeAllToColor(int[3], int[3], Adafruit_NeoPixel&);
    bool powerOn(Adafruit_NeoPixel&, int, IRrecv&, decode_results&);
    bool powerOff(Adafruit_NeoPixel&);
    int changeState(int, Adafruit_NeoPixel&);
    int getStateFlag();
    void transition(Adafruit_NeoPixel&);
};

#endif
