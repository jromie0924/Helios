#include <IRremote.h>
#include <IRremoteInt.h>
#include <Adafruit_NeoPixel.h>
#include "Randomizer.h"
#include <avr/power.h>
//#include <Time.h>

#define PIN 6
#define RECV_PIN 11
#define POWER_PIN 7

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
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  Serial.begin(9600);
  //Randomizer rand_(strip);
  //randomizer.powerOn(strip, 10);
  randomizer = new Randomizer(strip);
  //randomizer->powerOn(strip, 30);  
  //delay(1000);
}

void loop() {
  while(randomizer->stateFlag == 0) {
    if(irrecv.decode(&results)) {
      Serial.println("turning on");
      if(results.value == filterVal) {
        irrecv.resume();
        randomizer->powerOn(strip, 30, irrecv, results);
        Serial.println("Power on sequence finished.");
      }
      //irrecv.resume();
    }
  }
  randomizer->randomize(strip, irrecv, results);
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
