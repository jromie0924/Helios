#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Time.h>

#define PIN 6
int day1 [3] = {209, 100, 39}; //jstart
int day2 [3] = {220, 0, 0}; //end
//int currentColor[3] = {day1[0], day1[1], day1[2]};
int currentColors [45][3];

/*
int red = 200;
int green = 40;
int blue = 5;
*/

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code
  Serial.begin(9600);
  initializePixels();
  Serial.println(sizeof(currentColors[6]));
//  Serial.println(strip.numPixels());
  setTime(4, 20, 00, 11, 59, 2015);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  powerOn(day1[0], day1[1], day1[2], 10);
  delay(1000);
  srand(now());
  delay(10);
  //strip.setPixelColor(15, strip.Color(196, 196, 48));
 // delay(1000);
 // powerOff();
}
/*
 * 25 + ( std::rand() % ( 63 - 25 + 1 ) ) --> random number from 25-63
 * use for random pixel "liveliness"
 */ 
void loop() {
  //srand(now());
  int randColor = rand() % 3 + 1;
  int randPix = rand() % strip.numPixels();
  //Serial.println(randColor);
  if(randColor == 1) {
  //  if(!(currentColors[randPix][0] == day2[0] && currentColors[randPix][1] == day2[1] && currentColors[randPix][2] == day2[2])) {
    //  Serial.print("Changing color on pixel ");
   //  Serial.println(randPix);
      fadeToColor(currentColors[randPix], day2, randPix);
      for(int i = 0; i < sizeof(currentColors[randPix]); i++)
        currentColors[randPix][i] = day1[i];
    //}
  }
  delay(2000);
}

uint8_t splitColor ( uint32_t c, char value )
{
  switch ( value ) {
    case 'r': return (uint8_t)(c >> 16);
    case 'g': return (uint8_t)(c >>  8);
    case 'b': return (uint8_t)(c >>  0);
    default:  return 0;
  }
}

void fadeToColor(int start[], int end_[], int pix) {
 // Serial.println("here");
  int n = 100;
  int rnew = 0, gnew = 0, bnew = 0;
  for(int i = 0; i <= n; i++) {
    rnew = start[0] + (end_[0] - start[0]) * i / n;
    gnew = start[1] + (end_[1] - start[1]) * i / n;
    bnew = start[2] + (end_[2] - start[2]) * i / n;
    strip.setPixelColor(pix, strip.Color(rnew, gnew, bnew));
    strip.show();
    delay(100);
  }
}

void powerOn(int r, int g, int b, int wait) {
  //red = r;
  //green = g;
  //blue = b;
  for(int a = 0; a <= 100; a++) {
    double percentage = (double)a/100;
    int red_ = percentage * r;
    int green_ = percentage * g;
    int blue_ = percentage * b;
    
    for(int i=0; i<=strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(red_, green_, blue_));
    }
    strip.show();
    delay(wait);
  }
}
/*
void powerOn(int r, int g, int b, int wait) {
  for(int a = 0; a <= strip.numPixels()/2; a++) {
    for(int i = 0; i <= 100; i+=5) {
      double percentage = (double)i/100;
      int red_ = percentage * r;
      int green_ = percentage * g;
      int blue_ = percentage * b;
      strip.setPixelColor(a, strip.Color(red,green,blue));
      strip.setPixelColor(((strip.numPixels() - 1) - a), strip.Color(red_,green_,blue_));
      strip.show();
      delay(5);
    }
 //   delay(wait);
  }
  */
  /*
  for(int k = 0; k < strip.numPixels(); k++) {
    for(int g = 10; g <= 100; g+=30) {
      double percentage = (double)g/100;
     // Serial.println(percentage);
      int red_ = percentage * red;
      int green_ = percentage * green;
      int blue_ = percentage * blue;
    //  delay(1000);
      strip.setPixelColor(((strip.numPixels()-1) - k), strip.Color(red_,green_,blue_));
      strip.show();
     // delay(5);
    }
    //delay(wait);
  }
}
*/

void powerOff() {
  for(int a = strip.numPixels()/2; a >=0; a--) {
    for(double k = 100; k >= 0; k--) {
      double percentage = k/100.0;
      uint32_t colorVal = strip.getPixelColor(a);
  //    Serial.println(colorVal);
      double r = splitColor(colorVal, 'r');
      double g = splitColor(colorVal, 'g');
      double b = splitColor(colorVal, 'b');
      r = (double)r * percentage;
      g = (double)g * percentage;
      b = (double)b * percentage;
     // Serial.println(r);
      strip.setPixelColor(a, strip.Color((int)r,(int)g,(int)b));
      strip.setPixelColor(((strip.numPixels() - 1) -a), strip.Color((int)r,(int)g,(int)b));
      //delay(10);
      strip.show();
    }
    delay(1);
  }
}

void initializePixels() {
  Serial.println(sizeof(currentColors));
  Serial.println();
  for(int a = 0; a < 45; a++) {
    for(int k = 0; k < 3; k++) {
      currentColors[a][k] = day1[k];
     // delay(500);
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
/*
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
*/
