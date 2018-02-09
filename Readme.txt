Bass, Treble, and Volume USB Controller With Keyboard LEDs



Introduction: Bass, Treble, and Volume USB Controller With Keyboard LEDs


Picture of Bass, Treble, and Volume USB Controller With Keyboard LEDs

Picture of Bass, Treble, and Volume USB Controller With Keyboard LEDs

I have a Creative SoundBlaster Audigy in my main desktop computer and I needed a way to quickly adjust the bass and treble settings (as well as the volume) when listening to audio or video media.
I have adapted the code from two sources given in the code-listing, and also in the links at the end, which makes use of an Arduino Micro by the Arduino organisation, or any other ATmega32u4, as a USB Human Interface Device by using the Nico Hood HID-Project libraries.
I also use a wireless keyboard which only shows the state of the Caps-lock, Num-lock and Scroll-lock keys for a short time to increase its battery life. I therefore also included three LED's which show the status of these three keyboard functions.
Because the Micro can be easily reprogrammed even after putting it into an enclosure through the USB port, you can adapt the code to suit the characteristics of your own sound device and keyboard by substituting the media identification used in the code with your own selection. There is a vast list of all the functions that you can use available at the Nico Hood HID Github website - examine the file named ConsumerAPI.h for all the other HID functions.
Add TipAsk Question

Step 1: Parts List


Picture of Parts List

Arduino Micro or any other ATmega32u4 based Arduino board such as the Sparkfun Pro Micro. The Leonardo can be used but it is a very big board compared to the micro…
5 LEDs of different colour
5 x 470 ohm resistors
4 x pushbutton switches
Rotary encoder with A B C and a switch connection
Small enclosure, knob, strip board, connection wire etc.
Add TipAsk Question

Step 2: Construction


Picture of Construction

Picture of Construction

Details are given in the photos, schematic and the Fritzing stripboard layouts. I used two smaller strip boards - one to house 4 LEDs and the four switches, and the other for a blue volume level direction indicator LED, and a larger stripboard for the micro and the rotary encoder. They are connected by a 9 way and two way ribbon cable.

Attachments Final1.fzzFinal1.fzz Download

Add TipAsk Question

Step 3: Software

As mentioned this USB Volume, Bass, and Treble Control use a rotary encoder as the volume control with a mute/unmute switch, and the Bass and Treble controlled by four Up and Down buttons.
It also has PC Keyboard LED indicators namely Caps-lock, Scroll-lock, and Num-lock.
It uses an Arduino Micro (or other ATmega32u4-based) MCU and the code is based on the transition states ISR roto_sm.ino by boolrules at the Arduino forum, which gives clearly differentiated up and down triggers.
The code is also based on pleriche's USB Volume Control and Caps Lock LED instructable.
It requires the Nico Hood HID-Project library at github: download the library as a zip file and extract it, then copy the folder to your Arduino library folder underneath your documents folder.
Because the Micro can be easily reprogrammed via its USB port, you can adapt the code to suit the characteristics of your own sound device and keyboard by substituting the media instruction used in the code with your own selection.
There is a vast list of all the functions that you can use available at the Nico Hood HID Github website - examine the file named ConsumerAPI.h for all the other HID functions which can include system functions such as sleep or reboot key etc. I have included photos of the construction as it progressed.
