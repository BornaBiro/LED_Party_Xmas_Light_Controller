# LED Party And Xmas Light Controller

ATMGEA328P based project for ccontrolling WS2801 RGB LEDs (addressable LEDs with dedicated IC controller, not the ones with built-in controller). It's primarlly used for controlling LED lights for Xmass, but with some code modification can be used for smart ambient light or even a party light (it already has dedicated mode for that, but it does not work properly, more about that later).

![finished_device1](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img1.jpg) ![finished_device1](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img4.jpg)

### Features
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

### Usage
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

### Hadrware issues
Yeah, this things happen, it doesn't work from the first try, so there are some fixes you need to make in order to your PCB work properly.

1. microUSB connector does not fit. I use wrong footprint. Used microUSB connector is [920-E52A2021S10100](https://www.lcsc.com/product-detail/USB-Connectors_Jing-Extension-of-the-Electronic-Co-920-E52A2021S10100_C10418.html)
2. Missing SMD 10uF MLCC at DC input
![HW_Mistake1](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img8.jpg)
3. Missing debounce capacitor on rotary enncoder pushbutton from pin to GND (100nF)
![HW_Mistake2](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img5.jpg)
4. Analog audio part... Ohh boy... AVCC and AREF sholud be seperated (AVCC should be on 5V but AREF should be on 3V3), bias curcuit is missing filtering, MIC and bias must be supplied from 3V3.
![HW_Mistake3](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img6.jpg)
5. No GND at 3V3 VREG... This one hurts the most...
![HW_Mistake4](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img10.jpg)
6. Swapped CLK_OUT and CLK_OE... and this one...
![HW_Mistake5](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img7.jpg)
7. In order to make MIC breakout to work properly (so it does not capture noise from the power supply), cut the MIC supply trace and add 10uF 1206 MLCC.
![HW_Mistake6](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img9.jpg)
8. Some of these DC-DC converters are noisy. If needed, add 470uF at the output of the DC-DC converter.

**NOTE** I did not fix any of these on PCB design, so like I mentioned, you will either need to do these fixes or you will need to make changes on the PCB before you order it.

### Software issues
Software is still in the progress of make it. Since I do not have much free time, do not have high hopes on fixing these. I'll try my best to fix these before Xmas and New Year.
Whole code is written to be non-blocking.

Known issues:
1. Xmas mode and Party Mode are not seperated.
2. Only xmas mode is working properly
3. Be careful while setting up current Xmas mode, because you can force it into party mode by incrementing it to the Moon and beyond. Not sure if this is a bug or feature.
4. No day selecton on automatic on or off (no date setting either)

## Menu / How to use it

## Contributions
[ladyM9](https://github.com/ladyM9) - Little help for the timer code (got few ideas on how to approach the problem). Thank you!
