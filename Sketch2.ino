/////////////////////////////////////////////////////////////////////////////////////////
// PC USB Volume Bass Treble Control:
// Volume control by rotary encoder
// Bass and Treble by two rotary encoders
// Bass Boost 20+ and Treble Boost 20+ controlled by switches on Bass and Treble Volume controls
//
// PC Keyboard LED indicators for wireless keyboards:
// Caps-lock Scroll-lock Num-lock
//
// Requires Arduino Micro (or Sparkfun Pro Micro or another ATmega32u4-based) MCU
//
// Code based on valid transition states ISR roto_sm.ino by boolrules at:
// https://forum.arduino.cc/index.php?topic=242356.15
// This seems to give clearly differentiated up and down triggers
//
// Code also based on p_leriche USB Volume Control and Caps Lock LED:
// https://www.instructables.com/id/USB-Volume-Control-and-Caps-Lock-LED-Simple-Cheap-/
//
// Requires the Nico Hood HID-Project library:
// https://github.com/NicoHood/HID/wiki
//
// Code sources above adapted by Tobias van Dyk February 2018
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <HID.h>
#include <HID-Project.h>  

#define DIR_NONE 0x00           // No complete step yet.
#define DIR_CW   0x10           // Clockwise step.
#define DIR_CCW  0x20           // Anti-clockwise step.

// Volume
unsigned int state;
int count = 0;         // count each indent
int old_count = 0;     // check for count changed

// Bass
unsigned int bstate;
int bcount = 0;         // count each indent
int old_bcount = 0;     // check for count changed

// Treble
unsigned int tstate;
int tcount = 0;         // count each indent
int old_tcount = 0;     // check for count changed

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */

// State definitions state table (emits a code at 00 only)
// states are: (NAB) N = 0: clockwise;  N = 1: counterclockwiswe
#define R_START     0x3
#define R_CW_BEGIN  0x1
#define R_CW_NEXT   0x0
#define R_CW_FINAL  0x2
#define R_CCW_BEGIN 0x6
#define R_CCW_NEXT  0x4
#define R_CCW_FINAL 0x5

const unsigned char ttable[8][4] = {
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},                // R_CW_NEXT
    {R_CW_NEXT,  R_CW_BEGIN,  R_CW_BEGIN,  R_START},                // R_CW_BEGIN
    {R_CW_NEXT,  R_CW_FINAL,  R_CW_FINAL,  R_START | DIR_CW},       // R_CW_FINAL
    {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},                // R_START
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},                // R_CCW_NEXT
    {R_CCW_NEXT, R_CCW_FINAL, R_CCW_FINAL, R_START | DIR_CCW},      // R_CCW_FINAL
    {R_CCW_NEXT, R_CCW_BEGIN, R_CCW_BEGIN, R_START},                // R_CCW_BEGIN
    {R_START,    R_START,     R_START,     R_START}                 // ILLEGAL
};

// Define Arduino pin numbers for buttons and LEDs
#define NumLockLED 4
#define CapsLockLED 5 
#define ScrLockLED 6 

#define ActivityLED 10      // 16 Show Activity

#define RotaryA 2 
#define RotaryB 3 
#define RotaryC 7        // Volume Mute = Button on Volume

#define BassUp A3        // A0 Bass Increase
#define BassDwn A2       // A1 Bass Decrease

#define BassBoost 8      // Balance Right Increase = Button on Bass
#define TrebleBoost 9    // Balance Left Increase = Button on Treble

#define TrebleUp A1      // A2 Treble Increase
#define TrebleDwn A0     // A3 Treble Decrease

boolean AnyBtnPress = false;
byte n = 255;

////////////////////////////////////////////////////////
// Valid transition states ISR roto_sm.ino by boolrules
////////////////////////////////////////////////////////
void AB_isr( ) 
{
    // Grab state of input pins.
    unsigned char pinstate = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );

    // Determine new state from the pins and state table.
    state = ttable[state & 0x07][pinstate];

    if( state & DIR_CW )    
      { count++;                            // count up for clockwise
        Consumer.write(MEDIA_VOL_UP);
      }
    if( state & DIR_CCW )   
      { count--;                           // count down for counterclockwise
        Consumer.write(MEDIA_VOL_DOWN);
      }
    
}
void CheckBass() 
{
    // Grab state of input pins.
    unsigned char pinstate = (digitalRead( BassUp ) << 1) | digitalRead( BassDwn );


    // Determine new state from the pins and state table.
    bstate = ttable[bstate & 0x07][pinstate];

    if( bstate & DIR_CW )    
      { bcount++;                            // count up for clockwise
        Consumer.write(HID_CONSUMER_BASS_INCREMENT);
      }
    if( bstate & DIR_CCW )   
      { bcount--;                           // count down for counterclockwise
        Consumer.write(HID_CONSUMER_BASS_DECREMENT);
      }
    
}
void CheckTreble() 
{
    // Grab state of input pins.
    unsigned char pinstate = (digitalRead( TrebleUp ) << 1) | digitalRead( TrebleDwn );

    // Determine new state from the pins and state table.
    tstate = ttable[tstate & 0x07][pinstate];

    if( tstate & DIR_CW )    
      { tcount++;                            // count up for clockwise
        Consumer.write(HID_CONSUMER_TREBLE_INCREMENT);
      }
    if( tstate & DIR_CCW )   
      { tcount--;                           // count down for counterclockwise
        Consumer.write(HID_CONSUMER_TREBLE_DECREMENT);
      }
    
}
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
void setup() 
{
  pinMode (TrebleDwn, INPUT_PULLUP);
  pinMode (TrebleUp, INPUT_PULLUP);
  pinMode (BassDwn, INPUT_PULLUP); 
  pinMode (BassUp, INPUT_PULLUP); 

  pinMode (BassBoost, INPUT_PULLUP); 
  pinMode (TrebleBoost, INPUT_PULLUP); 

  pinMode (ActivityLED, OUTPUT);
 
  pinMode(RotaryA, INPUT_PULLUP);
  pinMode(RotaryB, INPUT_PULLUP);
  pinMode(RotaryC, INPUT_PULLUP);

  pinMode(CapsLockLED, OUTPUT);
  pinMode(ScrLockLED, OUTPUT);
  pinMode(NumLockLED, OUTPUT);

  AnyBtnPress = false;

  bstate = (digitalRead( BassUp ) << 1) | digitalRead( BassDwn );     // Initialise state.
  old_bcount = 0;

  tstate = (digitalRead( TrebleUp ) << 1) | digitalRead( TrebleDwn );     // Initialise state.
  old_tcount = 0;

  state = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );     // Initialise state.
  old_count = 0;

  attachInterrupt( 0, AB_isr, CHANGE );   // pin-change interrupts: 
  attachInterrupt( 1, AB_isr, CHANGE );

  Consumer.begin();          // Start Media Keys
  BootKeyboard.begin();      // Start Keybrd LEds

  // Flash the LEDs just to show we're in business
  digitalWrite(CapsLockLED, HIGH); delay(100);
  digitalWrite(ScrLockLED, HIGH); delay(100);
  digitalWrite(NumLockLED, HIGH); delay(100);
  digitalWrite(ActivityLED, HIGH); delay(100);
  digitalWrite(CapsLockLED, LOW); delay(100);
  digitalWrite(ScrLockLED, LOW); delay(100);
  digitalWrite(NumLockLED, LOW); delay(100);
  digitalWrite(ActivityLED, LOW); delay(100);

}

//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
void loop() 
{
    //check if the buttons are being pressed
    chkBtn(); if (AnyBtnPress) readBtn(); 

    if (n == 0)
       { if (BootKeyboard.getLeds() & LED_CAPS_LOCK) digitalWrite(CapsLockLED, HIGH);  // Capslock
                                                else digitalWrite(CapsLockLED, LOW);
         if (BootKeyboard.getLeds() & LED_SCROLL_LOCK) digitalWrite(ScrLockLED, HIGH); // Scrolllock
                                                  else digitalWrite(ScrLockLED, LOW);
         if (BootKeyboard.getLeds() & LED_NUM_LOCK) digitalWrite(NumLockLED, HIGH);
                                              else  digitalWrite(NumLockLED, LOW);
         //check if the buttons are being pressed
         chkBtn(); if (AnyBtnPress) readBtn(); 
         CheckBass();
         CheckTreble();
       }
    
    //analogWrite (VolumeLED, VolumeLevel);
    n--;
}

////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
void chkBtn()
{
  int btnRotary = digitalRead(RotaryC);
  int btnBassBoost = digitalRead(BassBoost);
  int btnTrebleBoost = digitalRead(TrebleBoost);

  AnyBtnPress = false;
  if (!btnBassBoost||!btnTrebleBoost||!btnRotary)  AnyBtnPress = true;
}
////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
void readBtn()
{ 
  //Check if buttons are pressed
  int i;
  int btnRotary = digitalRead(RotaryC);
  int btnBassBoost = digitalRead(BassBoost);
  int btnTrebleBoost = digitalRead(TrebleBoost);

  if (!btnBassBoost)  
     { digitalWrite (ActivityLED, HIGH);
       delay(100) ;
       for (i=0; i<20; i++) Consumer.write(HID_CONSUMER_BASS_INCREMENT);
     }

  if (!btnTrebleBoost)  
     { digitalWrite (ActivityLED, HIGH);
       delay(100) ;
       for (i=0; i<20; i++) Consumer.write(HID_CONSUMER_TREBLE_INCREMENT);
     }

  if (!btnRotary)  
     { digitalWrite (ActivityLED, HIGH);
       delay(100) ;
       Consumer.write(MEDIA_VOL_MUTE);
       delay(100) ;
     }

  delay(100);
  digitalWrite (ActivityLED, LOW);
     
}
