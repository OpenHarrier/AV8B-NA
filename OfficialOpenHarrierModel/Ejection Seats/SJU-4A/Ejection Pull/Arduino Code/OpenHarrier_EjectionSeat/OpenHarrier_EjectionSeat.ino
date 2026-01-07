/**************************************************************************
  OPEN HARRIER
  Ejection Seat Handle Pull

  GNU GENERAL PUBLIC LICENSE
  Version 3, 29 June 2007
  (Refer to the official documenation of GNU Version 3, 29 June 2007 for the license)

  This software is provided AS-IS as per the GNU General Public License.

  Description:
    This code emulates a keyboard and presses and releases LEFT_CTRL + "E" 3x times in rapid succession to initiate an ejection.  
    It has a cooldown reset that you can adjust so that it won't send the ejection sequece again for X milliseconds if you were to keep pulling on it. 

    The code also senses whether the ejection seat is armed or safe using DCS-BIOS, and lights an LED if you desire to indicate the seat is armed.  You can
    safely remove all of the DCS-BIOS code if you don't need that feature.  Note that this does not block the ejection sequence code from being sent if the seat is
    in "SAFE" mode, although that's easy to do just by setting a variable whenever the state changes of the arm handle, and adding a check on this state when the
    ejection handle is pulled.

  Requirements:
   You MUST use a controller that is capable of emulating a keyboard, such as an Arduino Micro.  In other words, it should be using the ATmega32U4, which is capable of emulating USB input devices.  An Arduino UNO will NOT work.

  Qty Item
  1   ATmega32U4-based controller or equivalent, such as Arduino Micro.
  1   switch of ejection pull detection.  Wire to pin of choice and adjust value of PIN_EJECT to correspond.
  1   optional:  LED and resistor to show seat armed state.  Adjust PIN_SEAT_ARMED_INDICATOR to correspond to pin chosen.

 **************************************************************************/


#include "Keyboard.h"
#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

#define PIN_SEAT_ARMED_SWITCH     9                 // Wire this to a switch that closes the contact when the seat is armed.
#define PIN_SEAT_ARMED_INDICATOR  LED_BUILTIN
#define PIN_EJECT                 10                // Wire this to a switch that closes when the ejection handle is pulled.
#define RESET_TIME                3000              // Amount of time in milliseconds that must pass before ejection handle can be pulled again.

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PIN_SEAT_ARMED_INDICATOR, OUTPUT);
  pinMode(PIN_EJECT, INPUT_PULLUP);
  pinMode(PIN_SEAT_ARMED_SWITCH, INPUT_PULLUP);
  DcsBios::setup();
}

//DcsBios::LED seatSafeLever(0x783c, 0x0080, PIN_SEAT_ARMED_INDICATOR);
DcsBios::Switch2Pos seatSafeLever("SEAT_SAFE_LEVER", PIN_SEAT_ARMED_SWITCH);

unsigned long lasttime = 0;

// the loop function runs over and over again forever
void loop() {
  DcsBios::loop();

  if (digitalRead(PIN_EJECT) == HIGH)
  {
    // Button NOT pressed.
    //digitalWrite(PIN_SEAT_ARMED_INDICATOR, LOW);  // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    // Button Pressed.
    if (millis() > lasttime + RESET_TIME)
    {
      //digitalWrite(PIN_SEAT_ARMED_INDICATOR, HIGH);   // turn the LED off by making the voltage LOW

      // The following code will rapidly press and release "CTRL+E" 3x times, which 
      // initiates the ejection sequence in DCS assuming the default keyboard mapping for eject.  
      for (int i=0; i < 3; i++)
      {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press('e');
        Keyboard.releaseAll();
        delay(20); // You can try reducing this value for faster ejection sequence initiation.
      }

      lasttime = millis();
    }
          
  }

}
