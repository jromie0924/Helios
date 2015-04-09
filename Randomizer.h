#ifndef RANDOMIZER_H
#define RANDOMIZER_H


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
  public:
    Randomizer();
    void randomizePix(int, int); // for one pixel
    void randomizePix(int, int, int); // for two pixels
};

#endif
