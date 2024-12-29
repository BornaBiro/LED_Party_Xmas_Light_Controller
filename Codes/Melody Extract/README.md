# Code for melody extraction from old school Xmas light "automata"

## Intro
Remeber these things? We use to hate it back in the days with it's irritating melodies they produced. But nowdays I really miss these melodies. Maybe I'm little bit nostalgic ... or crazy, who knows. :D 
<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract4.jpg>
</p>

I wanted to add melodies from these little noise makers, but I colud not found them anywhere on the web in the form of the notes and their durations in miliseconds.
I could find the songs and then manually convert them into notes (and frequencies) and durations, but this takes way too much time.
Or I could use these two and record the sound the produce into .wav file, store it on the microSD card and reproduce it with ATMEGA328P, but that would use way too much memory and CPU cycles.
But I needed much simpler solution. I got an idea. What if I record the signal comming from the IC inside this automata and use that?

But how? Well, here is how I done it. By using cheap 24MHz logic analyzer and some C code, of course!
<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract9.jpg>
</p>

You could use alerady extracted files (provided here as [newerAutomata.csv](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/Codes/Melody%20Extract/newerAutomata.csv) and [oldestAutomata.csv](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/Codes/Melody%20Extract/oldestAutomata.csv)) or even cleaned up file [here](https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/main/extras/xmas_light_automata_songs.txt).
But, if you are having one with the different set of melodies, join me on this adventure.

## Hardware side of things
First question that you may ask yourself can be "How do I connect logic analyzer to that?!" and that would be great a question. First thing to note that, what ever you do

> :warning: **DO NOT CONNECTED THEM INTO MAINS WHILE DOING ANYTHING RELATED TO THIS, PLEASE!!!**.

The truth is, these little devices run on about 3-5VDC. There is simple resistive dropper circuit that will convert 230VAC (or 120VAC) to 3-5V needed IC inside this box to create the melody.
So, for the supply, we need lab. power supply set to 3V with current limit set to no more than 30mA. But, again, where to connect this supply?!?! Calm down, it's actually very easy, just find electrolitic capacitor rated at 16V.
Usually this is the main filter capacitor. Just be careful about the polarity. If there is two caps, one is usually for the main filtering and other one is for speaker. How to know which is which is *fairly easy*; on capacitor, both legs are connected to the main IC, the one for the speaker can only hove one connected to the IC or even none (if the device has transistor ampplifier circuit).

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract6.jpg>
</p>

Now, if you connect the supply, this little noisemaker should start producing sound. If not, either is dead or you connected it wrong. In this case, check the supply current - if needs more than 30mA check the polarity or it's wrong capacitor.

Ok, ok, but where do I connect logic analyzer? Easy there! Fisrt, you will need an oscilloscope. And this does not to be fancy one. Even kit oscilloscopres can be used for this. But for what? Well, we need to find the output pin that goes into speaker to be able to capture the signal.
To find it, power the this noisemaker (**AGAIN NOT FROM THE MAINS, BUT FROM THE LAB. POWER SUPPLY WITH 3-5VDC!**) and try to find pin that produces squarewave with the frequency of about 500Hz to 2kHz. Thats your melody! To be sure, you can attach a small speaker (like a headphones) to check if this is really a signal that we are looking for.

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract5.jpg>
</p>

I already soldered white wire to it (and again, **black nad red wires are 4VDC!**).

Here is the otherone where I'm probing it with the oscilloscope.

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract7.jpg>
</p>

Close-up shot.

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract8.jpg>
</p>

As you can se this pin generates signal with 50% duty and the frequency is chaning with the sound. That's our signal! Now, solder the wire and connect it to the CH0 of the logic analyzer.

## Software side of things
Ok, now we have the signal, but how to convert that into something useful. Yea sure we can do this manually (please don't) by recording a video and than writing down frequrncies and duration seen on oscilloscope or recording a melody and setting a scopoe on slow time base and by using cursors extract frequency and duration. Or capture everything with the logic analyzer and then manually analyze frequencies and their durations. But this won't be a *engineering* method right? Besides, where is fun in that?
It's much better to write the code that will extract these values. So I did exactly that! Script can be found [here](main.cpp). The code is written in C/C++. I run it by using [DevCpp](https://www.bloodshed.net) but you can run it with simple GCC Compiler.
Code is ment to parse Salea Logic 2 .csv output with all 8 channels active.

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract1.png>
</p>

Captured data template:
```
Time [s],Channel 0,Channel 1,Channel 2,Channel 3,Channel 4,Channel 5,Channel 6,Channel 7
```

Example of one data point.
```
0.000000000,1,1,1,1,1,1,1,1
```

For script to work properly, use only channel 0 of the logic analyzer!
For export data use this settings:
<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract2.png>
</p>

You can use [songData.csv](songData.csv) as filename, or you can use whatever name you want as long as you change it in [main.cpp](main.cpp). After running script, you will get [output.csv](output.csv) with frequencies in Hz (Hertz) on the left and durations in milliseconds on the right.

<p align="center">
  <img width="100" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract3.png>
</p>

What the script actually doing is carefully analyzing data and watches for the pin state change and when this happens it captures time from the input time data and calculates the frequency. If frequency is changed from prevoius calculated one (plus tollerance) it save is to the file and also calculates the duration. If not it keeps reading the file until the file is read completly. Now you can use this with any MCU (in my case Arduino and ATMEGA328P).