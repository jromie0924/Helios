#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
int red = 245;
int green = 245;
int blue = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

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
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  powerOn(red, green, blue, 5);
  delay(1000);
 // powerOff();
}

void loop() {
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
  for(int a = 0; a < strip.numPixels()/2; a++) {
    for(int i = 1; i <= 100; i++) {
      double percentage = (double)i/100;
      int red = percentage * r;
      int green = percentage * g;
      int blue = percentage * b;
    //  Serial.println(red);
    //  Serial.println(green);
    //  Serial.println(blue);
      Serial.println(percentage);
      strip.setPixelColor(a, strip.Color(red,green,blue));
      strip.setPixelColor(((strip.numPixels() - 1) - a), strip.Color(red,green,blue));
      strip.show();
     // delay(1);
    }
    delay(wait);
  }
}

void powerOff() {
  for(int a = strip.numPixels()/2; a >=0; a--) {
    for(double k = 100; k >= 0; k--) {
      double percentage = k/100.0;
      uint32_t colorVal = strip.getPixelColor(a);
      Serial.println(colorVal);
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

