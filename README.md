# 🎄LED Party And Xmas Light Controller🎄

ATMGEA328P based project for ccontrolling WS2801 RGB LEDs (addressable LEDs with dedicated IC controller, not the ones with built-in controller). It's primarlly used for controlling LED lights for Xmass, but with some code modification can be used for smart ambient light or even a party light (it already has dedicated mode for that, but it does not work properly, more about that later).

<p align="center">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img1.jpg>
</p>

<p align="center">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img4.jpg>
</p>

## Features 
- microUSB supply
- DC Jack supply (microUSB and DC Jack **CAN'T** be used at the same time!)
- Terminal block for power supply (connected in parallel with DC Jack)
- Analog audio input for MAX4466 microphone breakout
- microSD card reader
- 16x2 I2C LCD
- 2 (3) x LED Channels
- built-in buck regulator for 5V power supply
- RTC for automatic power on and off
- relay output (**for non-mains use only**)
- rotary encoder wuth pushbutton for navigating menu
- buzzer for playing old school Xmas melodies
- CH340 USB to UART IC for easy programming and debugging
- Reset pushbutton
- Addidional pushbutton
- easyC / STEMMA / Qwiic I2C connector for additional I2C devices

There is very neat little feature with this device: it creates same melodies as these old school incandescent Xmas lights. Check that out [here](https://github.com/BornaBiro/LED_Party_Xmas_Light_Controller/blob/dev/Codes/Melody%20Extract/README.md).

## Getting started 🚀
1. Get the board!
2. Get the parts!
3. Solder everything together (except DC-DC converter, **you must set the output voltage to 5V-5.2V first**)
4. Connect DC-DC converter to the power supply and set the output voltage to the 5V-5.2V
5. Solder it to the PCB using the headers
6. Burn the bootloader to the ATMEGA328P (using the test pads at bottom side of the PCB). You can use Arduino as ISP. If you don't know how to use it, check it out [here](https://docs.arduino.cc/built-in-examples/arduino-isp/ArduinoISP).
7. Copy / install provided WS2801 library to the Arduino
8. Open the sketch
9. Set Board as Arduino Nano (Arduino Nano with ATMEPGA328P, old bootloader).
10. Upload the sketch
11. Have fun!

## Contributions to this project
[ladyM9](https://github.com/ladyM9) - Little help for the timer code (got few ideas on how to approach the problem). Thank you!

## Hadrware issues
⚠️**NOTE**⚠️

Before making the CB you have to know that current version of the PCB has _some_ hardware issues. All issues are listed [here](https://github.com/BornaBiro/LED_Party_Xmas_Light_Controller/tree/dev/PCB#hadrware-issues).

⚠️**NOTE**⚠️

I **did not** fix any of these on PCB design, so like I mentioned, you will either need to do these fixes or you will need to make changes on the PCB before you order it.

## Software issues
Software is still in the progress of make it. Since I do not have much free time, do not have high hopes on fixing these. I'll try my best to fix these before Xmas and New Year.
Whole code is written to be non-blocking.

Known issues:
1. Xmas mode and Party Mode are not seperated. Actually on new and updated version only Xmas mode exists!
2. Only xmas mode is working properly
3. Be careful while setting up current Xmas mode, because you can force it into party mode by incrementing it to the Moon and beyond. Not sure if this is a bug or feature. 😆
4. No day selecton on automatic on or off (no date setting either)
5. 3D printed case is still missing.
6. For longer strands with total number of LEDs > 40 one Step Down is not enough.

## To-Do list
1. Try create two different flash images fir different modes that can be loaded into ATMEGA328P from the microSD card
2. Variable/Selectable length of LEDs per output
3. Enable or disable some effects or put them in the list
4. More effects!
5. More timer intervals
6. UART control support

## How to use it
The device has two/three WS2802 LEDs outputs. Number of LEDs per output can be configured in the [code](Codes/Arduino%20Code/led_party_xmas_light_controller/defines.h), STRAND1, STRAND2 and STRAND3. Third channel can be used for relay output (selected by default) or for one more channel for LEDs. On first start up it will set all setting to default ones. Every settting is stored in ATMEGA328P EEPROM. To enter menu, press Rotary Enc. To go thorugh menu, rotate rotary encoder. To enter settings, press the encoder again. Rotate it to set parameters. To go on the next parameter press the encoder. To exit setting, wait or simply keep pressing until `>` is not visable anymore. `>` shows what parametes is currently being set. To exit menu, wait or go to `Exit`. The menu will timeout as well as the backlight.

### Menu
  - Current mode: Shows current mode - Only Xmas for now
  - Current setting: Shows what light mode is currently active (same on the main menu along with the clock)
  - Frame speed: To slow or speed up the current animation (in percent)
  - Animation dur.: How long to keep current light effect (in seconds)
  - Auto Change: If enabled it will go though all light modes automatically.
  - Melody: If enabled it will play the melody if light mode has melody.
  - Auto Start: If set on `Auto`, light will power up automaticaly accordingly on `Activation Time` intervals. Relay and LEDs are seperated. `On` will keeps LEDs or relay on all the time overriding timer intervals.
  - Set Time: Set the time on RTC.
  - Activation Time: Set the times when the device will be powered on. First time is when device will be enabled, second is when the deivce disable LEDs/Relay. Y/N is if this timeslot it even enabled or not.
  - Exit: Exit menu.

## Few more images

<p align="center">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img2.jpg>
</p>

<p align="center">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img3.jpg>
</p>

## Licence
This project uses MIT licence. It is provided as is - no warranty is given. In other words - use it at your own risk.
You can use this project, change it, modify it, but source must be listed (this github repo) as well as the author name.
