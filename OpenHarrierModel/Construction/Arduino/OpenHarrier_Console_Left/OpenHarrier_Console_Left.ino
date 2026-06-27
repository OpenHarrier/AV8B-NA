

#define DCSBIOS_DEFAULT_SERIAL
//#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>


#define PIN_AFCA  8
#define PIN_AFCB  10
#define PIN_ALT   2
#define PIN_PITCH 11
#define PIN_ROLL  9
#define PIN_YAW   12
#define PIN_QFEEL 13

#define PIN_GEAR_NOSE_GREEN   3
#define PIN_GEAR_MAIN_GREEN   4
#define PIN_GEAR_LEFT_GREEN   5
#define PIN_GEAR_RIGHT_GREEN  6
#define PIN_GEAR_NOSE_YELLOW  24
#define PIN_GEAR_MAIN_YELLOW  25
#define PIN_GEAR_LEFT_YELLOW  26
#define PIN_GEAR_RIGHT_YELLOW 27

#define PIN_GEAR_HANDLE_LIGHT   52
#define PIN_GEAR_HANDLE_SWITCH  50
#define PIN_GEAR_FLAP_CRUISE    42
#define PIN_GEAR_FLAP_STOL      40
#define PIN_ANTISKID_TEST       44
#define PIN_ANTISKID_NWS        46
#define PIN_FLAP_RESET          38
#define PIN_FLAP_OFF            36

#define PIN_FLOOD_LEFT          7

void setup(void) {
  
  



  DcsBios::setup();

}


void loop() {
  DcsBios::loop();

}

DcsBios::Switch3Pos afcSw("AFC_SW", PIN_AFCA, PIN_AFCB);

DcsBios::Switch2Pos sasRoll("SAS_ROLL", PIN_ROLL);

DcsBios::Switch2Pos altHoldSw("ALT_HOLD_SW", PIN_ALT);

DcsBios::Switch2Pos sasPitch("SAS_PITCH", PIN_PITCH);

DcsBios::Switch2Pos sasYaw("SAS_YAW", PIN_YAW);

DcsBios::Switch2Pos qFeelSw("Q_FEEL_SW", PIN_QFEEL);

DcsBios::LED leftGearG(0x7888, 0x0040, PIN_GEAR_LEFT_GREEN);
DcsBios::LED leftGearY(0x7888, 0x0020, PIN_GEAR_LEFT_YELLOW);

DcsBios::LED mainGearG(0x7888, 0x0400, PIN_GEAR_MAIN_GREEN);
DcsBios::LED mainGearY(0x7888, 0x0200, PIN_GEAR_MAIN_YELLOW);

DcsBios::LED noseGearG(0x7888, 0x0010, PIN_GEAR_NOSE_GREEN);
DcsBios::LED noseGearY(0x7888, 0x0008, PIN_GEAR_NOSE_YELLOW);

DcsBios::LED rightGearG(0x7888, 0x0100, PIN_GEAR_RIGHT_GREEN);
DcsBios::LED rightGearY(0x7888, 0x0080, PIN_GEAR_RIGHT_YELLOW);

// Gear / Flap panel
//////////////////////////////
DcsBios::LED gearLeverLight(0x7882, 0x0004, PIN_GEAR_HANDLE_LIGHT);

DcsBios::Switch2Pos lgLever("LG_LEVER", PIN_GEAR_HANDLE_SWITCH, true);  // last parameter reverses the output.

DcsBios::Switch3Pos flapMode("FLAP_MODE", PIN_GEAR_FLAP_STOL, PIN_GEAR_FLAP_CRUISE);

DcsBios::Switch3Pos antiSkid("ANTI_SKID", PIN_ANTISKID_NWS, PIN_ANTISKID_TEST);

DcsBios::Switch3Pos flapPower("FLAP_POWER", PIN_FLAP_OFF, PIN_FLAP_RESET);

// Flood Lamps
void onFloodLightsDialChange(unsigned int newValue) {
    /* your code here */

// 16%  10771
  // 30%  19679
  // 43%  28483
  // 60%  39359
  // 72%  47231
  // 100% 65535

  //unsigned long temp = (newValue * newValue) / 65535;
  //  newValue = (newValue * newValue)/65535;
  //newValue = map(newValue,0,65535, 80,255);

  //float linearFloat = (log(newValue + 1.0) / log(65536.0)) * 255.0;
  
  //int linearValue = (int)linearFloat; // Final value 0-255

  //linearValue = map(linearValue, 0, 255, 0, 128);
  int linearValue = map(newValue, 0, 65535, 0, 255); // not expoential like I thought!


  analogWrite(PIN_FLOOD_LEFT, linearValue);
}
DcsBios::IntegerBuffer floodLightsDialBuffer(0x7862, 0xffff, 0, onFloodLightsDialChange);

