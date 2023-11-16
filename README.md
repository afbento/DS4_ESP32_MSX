# DS4_ESP32_MSX
ESP32 adapter to use a DualShock4 game controller on a MSX computer via bluetooth connection

On the ESP32 code it was used the PS4-esp32 Library (https://github.com/aed3/PS4-esp32) to establish Bluetooth connection.


## Remapping buttons:
To enter on Remapping mode, press OPTIONS one time (DS4 LED will turn GREEN)

Choose the desirable buttons in the following sequence : UP, DOWN, RIGHT, LEFT, TRIGGER 1, TRIGGER 2 (DS4 LED will blink white when release each button)

Almost all of DS4 buttons can be chosen, except OPTIONS button.

Choose this sequence of buttons for Joystick 1 and 2, then it will return automatically to Operational mode and will save this configuration on ESP32.

You can press OPTIONS two times before the sequence ends, to return to Operational mode and save the modified buttons on ESP32.


## Adjusting "deadzone" parameters:

To enter on Deadzone mode, press OPTIONS 2 times (DS4 LED will turn YELLOW)

To choose the deadzone value for the Analog Sticks, press LSTICK to right or left (DS4 LED will from faint YELLOW to BLUE, according to analog stick move intensity),

and press CROSS to modify this deadzone parameter (DS4 LED will turn white when CROSS button is pressed).

To chosse the deadzone value for R2 and L2 buttons, press L2 (DS4 LED will from faint YELLOW to RED, according to L2 press intensity)

and press CIRCLE to modify this deadzone parameter (DS4 LED will turn white when CIRCLE button is pressed).

To save these configurations on ESP32 and return to Operational mode, press OPTIONS.


## Some schematic and code explanations:

There are two versions of the schematics to this project:

One that ignores PIN8(OUT) of the MSX joystick port. 

And one that ESP32 disables U2 or U3 and sets its output signals to Hi-Z, if the PIN8 signal of the corresponding Joystick is HIGH.

The ESP32 code can be used with these versions without any modification.

Unfortunately its not possible to pair more than one DS4 controller due code and/or PS4-ESP32 library inability.

In the future i'll try another solution to pair 2 or more game controllers.


