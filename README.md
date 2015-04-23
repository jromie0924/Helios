Helios is controlled by an Arduino. Arduino is a microcontroller which allows one to write some code to tell it what to do, in laymen’s terms.
Arduino can handle both input values and output values. A trivial example is to set up a button (input) and an LED bulb (output); whenever the button is depressed, the LED turns on, and when the button is released, the LED shuts off.
Projects as simple as the previous example, and as complex as controlling an autonomous robot can be controlled by an Arduino.
For the tech savvy, the Arduino has two main core functions, and then the programmer writes other code (presumably) to add more functionality to whatever gizmo is being created:
-	Setup (core function 0)
o	Only run once, at power-on
o	Programmer might want to initialize values, tell the Arduino to be ready for the heart of the software to run
o	This is accomplished in setup.
-	Loop (core function 1)
o	Runs indefinitely until the Arduino is disconnected from power.
o	Inside here the programmer can insert commands for the Arduino to perform over and over again
o	These commands can consist of the following (plus more)
	Basic mathematical computations
	Calling of other functions, defined by the programmer
	Reading inputs
	Sending outputs
	Etc.
o	At this point you may be asking how Helios can act at total random when the loop function repeats the same functions indefinitely…

The Software
Helios works by selecting a random number of individual LEDs from the NeoPixel strip, and then, for each individual LED that has been selected, it selects a random color from a predefined pool of colors, and assigns that color to the LED. It is then faded into that color.
The loop function does not cause repetition because each time the Arduino enters the loop process, it calls other functions Jackson wrote that make sure to never make the same selection twice, except for out of coincidence.
Every time Helios enters a process that takes a notable amount of time, it repeatedly polls the infrared receiver for signals. If a signal is detected (user presses the remote control button), it halts the process, and based on the current state (off, daylight, sunset, night), it changes the pool of colors and resets the repeating process of randomization.

The LEDs
The LED strip used to implement Helios is unique. It is made by adaFruit, and is called NeoPixel. What makes it unique is that it is digitally addressed. This allows the microcontroller to talk individually to each LED and give it a single hue and brightness. For instance, LED 0 can have RGB values (10, 45, 30), while LED 1 can have RGB values (230, 130, 98). Each RGB value has an inclusive range from 0-255.
------------------------------------------------------------------------------------------------------------------------------------------
FOR THE GEEKS: If you really get this stuff, then you would understand that since RGB is usually 8-bit, the largest value is 255, which comes from (28 – 1). Computers use a number system called binary which is composed of 1 and 0, and in very basic terms, the largest number that can be accomplished with 8 digits (bits) is 255 (11111111). From right  left, the first bit representing 20, second bit representing 21, third representing 22, and so on, we multiply the result of those exponents by the value at that position (0 or 1) to get the decimal value (equal value in regular number system).
For example: 110 = 6
0 * 20 = 0
1 * 21 = 2
1 * 22 = 4
0 + 2 + 4 = 6

