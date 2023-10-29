# Code for melody extraction from old school Xmas light "automata"

## Intro
Remeber these things? We use to hate it back in the days with it's irritating melodies they produced. But nowdays I really miss these melodies. Maybe I'm little bit nostalgic ... or crazy, who knows. :D 
<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract4.jpg>
</p>

I wanted to add melodies from these little noise makers, but I colud not found them anywhere on the web in the form of the notes and their durations in miliseconds.
I colud find the songs and then manually convert them into notes (and frequencies) and durations, but this takes way too much time.
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
Usually this is the main filter capacitor. Just be careful about the polarity. If there is two caps, one is usually for the main filtering and other one is for speaker.

<p align="left">
  <img width="600" src=https://raw.githubusercontent.com/BornaBiro/LED_Party_Xmas_Light_Controller/dev/extras/Images/melodyExtract6.jpg>
</p>

Now, if you connect the supply, this little noisemaker should start producing sound. If not, either is dead or you connected it wrong.

Ok, ok, but where do I connect logic analyzer? Easy there, fisrt, you will need an osciloscope. And this does not to be fancy one. Even kit osciloscopes can be used for this. But for what? Well, we need to find the output pin that goes into speaker.
To find it, power the this noisemaker (**AGAIN NOT FROM THE MAINS, BUT FROM THE LAB. POWER SUPPLY WITH 3-5VDC!**) and try to find pin that produces squarewave with the frequency of about 500Hz to 2kHz. Thats your melody!

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

Now, solder the wire and connect it to the CH0 of the logic analyzer.

## Software side of things
[Comming soon!]
