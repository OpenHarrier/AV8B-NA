

#define DCSBIOS_DEFAULT_SERIAL
//#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>



#define PIN_FLOOD_INPUT         A0
#define PIN_LIGHT_CONSOLE       A1
#define PIN_LIGHT_INST          A2
#define PIN_LIGHT_WARN          A3

#define PIN_FLOOD_LEFT          7
#define PIN_BATT_ALERT          2
#define PIN_BATT_ON             3
#define PIN_GEN_ON              4
#define PIN_GEN_TEST            5
#define PIN_ENG_START           6
// What's 7???
#define PIN_APU_RESET           9
#define PIN_APU_OFF             8
#define PIN_DCTEST_MAIN         11
#define PIN_DCTEST_STDBY        10

#define PIN_SAM                 28
#define PIN_CW                  29
#define PIN_AI                  30
#define PIN_AAA                 31

#define PIN_LIGHTTEST_1         32
#define PIN_LIGHTTEST_2         33

void setup(void) {
  



  DcsBios::setup();

}


void loop() {
  DcsBios::loop();

}


DcsBios::LED samLight(0x7880, 0x0020, PIN_SAM);
DcsBios::LED cwLight(0x7880, 0x0040, PIN_CW);
DcsBios::LED aiLight(0x7880, 0x0080, PIN_AI);
DcsBios::LED aaaLight(0x7880, 0x0100, PIN_AAA);


// Compass / Test light switch:
DcsBios::Switch3Pos compLight("COMP_LIGHT", PIN_LIGHTTEST_1, PIN_LIGHTTEST_2);


// Flood Lamps
void onFloodLightsDialChange(unsigned int newValue) {

  int linearValue = map(newValue, 0, 65535, 0, 255); // not expoential like I thought!

  analogWrite(PIN_FLOOD_LEFT, linearValue);
}
DcsBios::IntegerBuffer floodLightsDialBuffer(0x7862, 0xffff, 0, onFloodLightsDialChange);

DcsBios::Potentiometer floodLightsDial("FLOOD_LIGHTS_DIAL", PIN_FLOOD_INPUT,true);


DcsBios::Potentiometer consoleLights("CONSOLE_LIGHTS", PIN_LIGHT_CONSOLE, true);
DcsBios::Potentiometer instLights("INST_LIGHTS", PIN_LIGHT_INST, true);
DcsBios::Potentiometer annunciatorLights("ANNUNCIATOR_LIGHTS", PIN_LIGHT_WARN, true);



DcsBios::Switch3Pos battSw("BATT_SW", PIN_BATT_ALERT, PIN_BATT_ON);
DcsBios::Switch3Pos genSw("GEN_SW", PIN_GEN_ON, PIN_GEN_TEST);
DcsBios::Switch2Pos engStartSw("ENG_START_SW", PIN_ENG_START);
DcsBios::Switch3Pos apuGenSw("APU_GEN_SW", PIN_APU_OFF, PIN_APU_RESET);
DcsBios::Switch3Pos dcTestSw("DC_TEST_SW", PIN_DCTEST_MAIN, PIN_DCTEST_STDBY);
