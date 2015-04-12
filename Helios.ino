#include <IRremote.h>
#include <IRremoteInt.h>
#include <Adafruit_NeoPixel.h>
#include "Randomizer.h"
#include <avr/power.h>
//#include <Time.h>

#define PIN 6
#define RECV_PIN 11

// Initialize the IR receiver.
IRrecv irrecv(RECV_PIN);
decode_results results;
bool isOff = true;
#define filterVal 581859881

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(45, PIN, NEO_GRB + NEO_KHZ800);

// Initialize the randomizer object. 
Randomizer *randomizer;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  
  // Start the receiver.
  irrecv.enableIRIn();
  
  Serial.begin(9600);
  //Randomizer rand_(strip);
  //randomizer.powerOn(strip, 10);
  randomizer = new Randomizer(strip);
  //randomizer->powerOn(strip, 30);  
  delay(1000);
}



void loop() {
  while(isOff) {
    if(irrecv.decode(&results)) {
      if(results.value == filterVal) {
        irrecv.resume();
        delay(500);
        randomizer->powerOn(strip, 30, irrecv, results);
        isOff = false;
      }
      //irrecv.resume();
    }
  }
  //  delay(500);
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

/*
void fadeToColor(int *start, int *end_, int pix) {
 // Serial.println("here");
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
}*/

//void powerOn(int r, int g, int b, int wait) {
//  for(int a = 0; a <= 100; a++) {
//    double percentage = (double)a/100;
//    int red_ = percentage * r;
//    int green_ = percentage * g;
//    int blue_ = percentage * b;
//    
//    for(int i=0; i<=strip.numPixels(); i++) {
//      strip.setPixelColor(i, strip.Color(red_, green_, blue_));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
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
