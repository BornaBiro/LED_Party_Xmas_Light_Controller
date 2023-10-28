PCB CAD files are made with EAGLE. I will add gerbers files later (if needed).

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
8. Added two 1206 10uF MLCC at the output of the DC-DC converter to reduce high frequency noise.
![HW_Mistake7](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/img11.jpg) 
10. Some of these DC-DC converters are noisy. If needed, add 470uF at the output of the DC-DC converter.

**NOTE** I did not fix any of these on PCB design, so like I mentioned, you will either need to do these fixes or you will need to make changes on the PCB before you order it.
