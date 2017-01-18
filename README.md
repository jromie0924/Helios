Helios is a light fixture designed to mimic the Sun. It has a light strip mounted on a cylinder with a large round plate on top. The light bounces off the wall and appears to the viewer as a solar eclipse. The individual lights on the strip are digitally-addressed, meaning that they can be randomly selected to fade to other colors (thus further mimicing the movement of the real sun).

How the software works:

Helios is centered around a randomizer. Not only do we want to choose our differing colors in a way such that it appears to mimic the sun, but we also want to make it not repetitive. To do that, I seeded the randomizer based on the uptime of the unit. The process of randomly selecting the "rays" is as such:
  * Randomly select a number, call it n, between 10 and 25
    - As many times as n, do:
      -> Randomly select an LED, and make sure that the selected LED has not already been selected
      -> From a pool of colors, randomly select one, assign it to that LED, and fade it over to that color

IR receiver:
Helios has four modes, including "off." The reason why I included "off" in the list of modes is because when Helios is turned off (and still plugged in), the Arduino is still working. It is still listening for another signal from the IR receiver to turn it back on. The next setting is Daylight, which consists of warm whites, some orange, and a tad bit of green. The next setting is Sunset consisting of oranges, purples, and reds. The last setting is moonlight which consists of blue, purple, and blueish-white.

To avoid using interrupts, I opted for polling whenever possible. This means when Helios is in a loop, for each iteration of whatever task it is performing, it also checks for a signal from the IR remote control. If it gathers a signal, it breaks out of the loop, and goes through functionality of changing settings. When Helios is "off," it constantly sits in the Arduino loop() function polling for a signal that makes it through the filter.

Known issues:
IR response is weak. User needs to be mindful of exactly how Helios works or it could end up freezing and will need to be disconnected from power and reconnected. This is nothing more than a prototype.

Check out the video, and shoot me an email if you have any questions!

https://youtu.be/QKJVXafNn3E
