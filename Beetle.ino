/////////////////////////////////////////////////////////////////////////////////////////
// PC USB Volume Bass Treble Control:
// Volume control by rotary encoder
// Bass and Treble controlled by Up and Down buttons
//
// PC Keyboard LED indicators for wireless keyboards:
// Caps-lock Scroll-lock Num-lock
//
// Requires DFRobot Beetle (or other ATmega32u4-based) MCU
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
// Code sources above adapted by Tobias van Dyk February 2020
//
/////////////////////////////////////////////////////////////////////////////////////////
// DFRobot Beetle
// Silkscreen Digital-Pin PWM-Channel Analog-Channel UART     I2C 
//--------------------------------------------------------------------------------------
// Pads on bottom PCB
//
// RX         0                                      Serial 1                   
// TX         1     
// SDA        2                                               SDA 
// SCL        3           3                                   SCL 
//--------------------------------------------------------------------------------------
// Pads with holes on sides and top and bottom PCB
//
// 9          9           9           A9   
// 10         10          10          A10   
// 11         11          11    
// A0         A0                      A0   
// A1         A1                      A1   
// A2         A2                      A2 
/////////////////////////////////////////////////////////////////////////////////////////

#include <HID.h>
#include <HID-Project.h>  

#define DIR_NONE 0x00           // No complete step yet.
#define DIR_CW   0x10           // Clockwise step.
#define DIR_CCW  0x20           // Anti-clockwise step.

unsigned int state;
int count = 0;         // count each indent
int old_count = 0;     // check for count changed

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
#define NumLockLED 9
#define CapsLockLED 10 
#define ScrLockLED 11 

#define ActivityLED A2     // A2 Show Activity

#define RotaryA 3          // SCL Bottom pad on Beetle D3 
#define RotaryB 2          // SDA Bottom pad on Beetle D2 
#define RotaryC A1         // A1 Mute switch

boolean AnyBtnPress = false;
byte n = 255;
byte VolumeLevel = 16;

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
        VolumeLevel = 255;
      }
    if( state & DIR_CCW )   
      { count--;                           // count down for counterclockwise
        Consumer.write(MEDIA_VOL_DOWN);
        VolumeLevel = 32; 
      }
    
}

//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
void setup() 
{
  pinMode (ActivityLED, OUTPUT);
 
  pinMode(RotaryA, INPUT_PULLUP);
  pinMode(RotaryB, INPUT_PULLUP);
  pinMode(RotaryC, INPUT_PULLUP);

  pinMode(CapsLockLED, OUTPUT);
  pinMode(ScrLockLED, OUTPUT);
  pinMode(NumLockLED, OUTPUT);

  AnyBtnPress = false;

  state = (digitalRead( RotaryA ) << 1) | digitalRead( RotaryB );     // Initialise state.
  old_count = 0;

  attachInterrupt( 0, AB_isr, CHANGE );   // pin-change interrupts: 
  attachInterrupt( 1, AB_isr, CHANGE );

  Consumer.begin();          // Start Media Keys
  BootKeyboard.begin();      // Start Keybrd LEds
  //  System.begin();        // For System functions
  //  Gamepad.begin();       // For gamepad functions
  //  Mouse.begin();         // For mouse functions
  //  AbsoluteMouse.begin(); // For the Absolute Mouse

  // Flash the LEDs just to show we're in business
  digitalWrite(CapsLockLED, HIGH); delay(500);
  digitalWrite(ScrLockLED, HIGH); delay(500);
  digitalWrite(NumLockLED, HIGH); delay(500);
  digitalWrite(ActivityLED, HIGH); delay(500);
  delay(2000);
  digitalWrite(CapsLockLED, LOW); delay(500);
  digitalWrite(ScrLockLED, LOW); delay(500);
  digitalWrite(NumLockLED, LOW); delay(500);
  digitalWrite(ActivityLED, LOW); delay(500);

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
       }
    
    n--;
}

////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
void chkBtn()
{
  int btnRotary = digitalRead(RotaryC);

  AnyBtnPress = false;
  if (!btnRotary)  AnyBtnPress = true;
}
////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
void readBtn()
{ 
  //Check if buttons are pressed
  int btnRotary = digitalRead(RotaryC);

  if (!btnRotary)  
     { digitalWrite (ActivityLED, HIGH);
       delay(100) ;
       Consumer.write(MEDIA_VOL_MUTE);
       delay(100) ;
     }

  delay(200);
  digitalWrite (ActivityLED, LOW);
     
}
