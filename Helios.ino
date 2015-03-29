#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Time.h>

#define PIN 6
int red = 245;
int green = 245;
int blue = 60;
int randRed, randGreen, randBlue, randPix;
int colorVals[11][3];
int pos=0;
int pos1=0;
int incNum = 1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(23, PIN, NEO_GRB + NEO_KHZ800);

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
//  Serial.println(strip.numPixels());
  setTime(4, 20, 00, 11, 59, 2015);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  powerOn(red, green, blue, 1);
  //strip.setPixelColor(15, strip.Color(196, 196, 48));
 // delay(1000);
 // powerOff();
}
/*
 * 25 + ( std::rand() % ( 63 - 25 + 1 ) ) --> random number from 25-63
 * use for random pixel "liveliness"
 */ 
void loop() {
  srand(now());
  randRed = rand() % 245;
  randGreen = rand() % 245;
  randBlue = rand() % 245;
  randPix = rand() % strip.numPixels();
  
  red = splitColor(strip.getPixelColor(randPix), 'r');
  green = splitColor(strip.getPixelColor(randPix), 'g');
  blue = splitColor(strip.getPixelColor(randPix), 'b');
  red = randRed;
  green = randGreen;
  blue = randBlue;
  
  for(int a = 0; a < strip.numPixels(); a++) {
   // strip.setPixelColor(randPix,strip.Color(red, green, blue));
  }
  strip.show();
  delay(500);
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

void powerOn(int r, int g, int b, int wait) {
  for(int a = 0; a < strip.numPixels(); a++) {
    for(int i = 0; i <= 9; i++) {
      double percentage = (double)i/100;
      int red_ = percentage * r;
      int green_ = percentage * g;
      int blue_ = percentage * b;
      //strip.setPixelColor(a, strip.Color(red,green,blue));
      if(a <=11) {
        colorVals[pos][0] = red_;
        colorVals[pos][1] = green_;
        colorVals[pos][2] = blue_;
      }
      else {
        strip.setPixelColor(((strip.numPixels() - 1) - a - 11), strip.Color((colorVals[pos1][0] + incNum), (colorVals[pos1][1] + incNum), (colorVals[pos1][2] + incNum)));
      }
      strip.setPixelColor(((strip.numPixels() - 1) - a), strip.Color(red_,green_,blue_));
      strip.show();
      delay(5);
    }
    pos++;
    pos1++;
 //   delay(wait);
  }
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
  */
}

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

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
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

