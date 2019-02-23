# Bass, Treble, and Volume USB Controller With Keyboard LEDs

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/photo6.jpg" width="550" height="335" />

For more details please see the project at https://www.instructables.com/id/Bass-Treble-and-Volume-USB-Controller-With-Keyboar/

I have a Creative SoundBlaster Audigy in my main desktop computer and I needed a way to quickly adjust the bass and treble settings (as well as the volume) when listening to audio or video media. I have adapted the code from two sources given in the code-listing, and also in the links at the end, which makes use of an Arduino Micro by the Arduino organisation, or any other ATmega32u4, as a USB Human Interface Device by using the Nico Hood HID-Project libraries. I also use a wireless keyboard which only shows the state of the Caps-lock, Num-lock and Scroll-lock keys for a short time to increase its battery life. I therefore also included three LED's which show the status of these three keyboard functions. Because the Micro can be easily reprogrammed even after putting it into an enclosure through the USB port, you can adapt the code to suit the characteristics of your own sound device and keyboard by substituting the media identification used in the code with your own selection. There is a vast list of all the functions that you can use available at the Nico Hood HID Github website - examine the file named ConsumerAPI.h for all the other HID functions. 

### Step 1: Parts List

* Arduino Micro or any other ATmega32u4 based Arduino board such as the Sparkfun Pro Micro. The Leonardo can be used but it is a very big board compared to the micro… 
* 5 LEDs of different colour 
* 5 x 470 ohm resistors 
* 4 x pushbutton switches 
* Rotary encoder with A B C and a switch connection 
* Small enclosure, knob, strip board, connection wire etc. 

### Step 2: Construction

Details are given in the photos, schematic and the Fritzing stripboard layouts. I used two smaller strip boards - one to house 4 LEDs and the four switches, and the other for a blue volume level direction indicator LED, and a larger stripboard for the micro and the rotary encoder. They are connected by a 9 way and two way ribbon cable.

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/FinalSch.png" width="712" height="456" />

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/Final1.png" width="682" height="471" />

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/Photo1.jpg" width="793" height="676" />

### Step 3: Software
As mentioned this USB Volume, Bass, and Treble Control use a rotary encoder as the volume control with a mute/unmute switch, and the Bass and Treble controlled by four Up and Down buttons. It also has PC Keyboard LED indicators namely Caps-lock, Scroll-lock, and Num-lock. It uses an Arduino Micro (or other ATmega32u4-based) MCU and the code is based on the transition states ISR roto_sm.ino by boolrules at the Arduino forum, which gives clearly differentiated up and down triggers. The code is also based on pleriche's USB Volume Control and Caps Lock LED instructable. It requires the Nico Hood HID-Project library at github: download the library as a zip file and extract it, then copy the folder to your Arduino library folder underneath your documents folder. Because the Micro can be easily reprogrammed via its USB port, you can adapt the code to suit the characteristics of your own sound device and keyboard by substituting the media instruction used in the code with your own selection. There is a vast list of all the functions that you can use available at the Nico Hood HID Github website - examine the file named ConsumerAPI.h for all the other HID functions which can include system functions such as sleep or reboot key etc. I have included photos of the construction as it progressed. Source code is in Final.ino.


### Step 4: Links

* Arduino Micro https://store.arduino.cc/arduino-micro
* State machine ISR roto_sm.ino by boolrules https://forum.arduino.cc/index.php?topic=242356.15
* P LeRiche USB Volume Control and Caps Lock LED https://www.instructables.com/id/USB-Volume-Control-and-Caps-Lock-LED-Simple-Cheap-/
* Nico Hood HID-Project Github library https://github.com/NicoHood/HID/wiki

### Step 5: Adding Bass and Treble Rotary Controls

It is possible to replace the four pushbuttons that control the bass and treble with two rotary controls. Schematic2 gives the details and the Sketch is in Sketch2. The two pushbuttons on each control are used to give a bass boost and treble boost respectively. Source code is in Sketch2.ino.

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/Schematic2.png" width="687" height="462" />

### Step 6: Use a Sparkfun Pro Micro for a Smaller Case

Picture of Use a Sparkfun Pro Micro for a Smaller Case
Picture of Use a Sparkfun Pro Micro for a Smaller Case
You can use the smaller version of the Arduino Micro namely the Sparkfun Pro Micro to make a smaller enclosure for the Bass Treble Volume and Keyboard LED USB Controller. Photo5 and Photo6 gives the details of the build and you can use Sketch3.ino for the Pro Micro. Source code is in Sketch2.ino.

<img src="https://github.com/TobiasVanDyk/Bass-Treble-and-Volume-USB-Controller-with-Keyboard-LEDs/blob/master/photo5.jpg" width="636" height="525" />
