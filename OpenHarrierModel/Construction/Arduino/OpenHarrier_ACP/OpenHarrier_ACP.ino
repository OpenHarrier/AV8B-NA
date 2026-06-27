/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
//FreeSans12pt7b.h
#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans18pt7b.h>
#include <Adafruit_SSD1306.h>


#define PIXWIDTH 2
#define PIXSPACE 1

#define paddingLarge 2
#define paddingSmall 1
byte offsetLarge = PIXWIDTH + paddingLarge;
byte offsetSmall = PIXWIDTH + paddingSmall;

#define OFFSET   (PIXWIDTH + PIXSPACE)
#define YOFFSET 26
#define XOFFSET 38


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define PIN_FUZE_UP   9
#define PIN_FUZE_DOWN 10
#define PIN_QTY1_DOWN 7
#define PIN_QTY1_UP   8
#define PIN_QTY2_DOWN 6
#define PIN_QTY2_UP   5    
#define PIN_MULT_UP   4
#define PIN_MULT_DOWN 3  

#define PIN_INTV0_UP  2
#define PIN_INTV0_DOWN 1

#define PIN_INTV1_UP  0
#define PIN_INTV1_DOWN 14

#define PIN_INTV2_UP  15
#define PIN_INTV2_DOWN 16

// pins 22 23 24 25 26 are being used by the Jettison mode 5-position switch
// pins 27 is the jettison stores button

#define PIN_HUD_AG_LED        28
#define PIN_HUD_AG_BUTTON     29

#define PIN_HUD_NAV_LED       30
#define PIN_HUD_NAV_BUTTON    31

#define PIN_HUD_VSTOL_LED     32
#define PIN_HUD_VSTOL_BUTTON  33

#define PIN_MASTER_ARM_SWITCH  34

#define PIN_FLARE_SALVO        35

#define PIN_NAV_COURSE1        36
#define PIN_NAV_COURSE2        37

#define PIN_MASTER_CAUTION_RESET 38
#define PIN_MASTER_WARN_RESET    39


#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


unsigned long lastframeupdate = 0;
bool flipflop1 = false;

void onUpdateCounterChange(unsigned int newValue) {
    lastframeupdate = millis();
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);


bool isDcsBiosSleeping()
{
  return (millis() > (lastframeupdate + 300000)); // 5 minutes
}

int startx = 5;
int currx = startx;
void updateScreenSaver(int i)
{
  static float rate = 1.2f;

  display.clearDisplay();
  display.setCursor(currx, 44);
  if (flipflop1)
    display.print(("     "));
  else
    display.print(("X X X"));
  //display.fillCircle((int)ballx[i], (int)bally[i], 4, SSD1306_WHITE);
  display.display();
  return;

}





int interval = 250;





int stab[3] = { 0, 0, 0 };
bool stabchange = false;

int duct[3] = { 0, 0, 0 };
bool ductchange = false;

int ff[3] = { 0, 0, 0 };
bool ffchange = false;

int rpm[4] = { 0, 0, 0, 0 };
bool rpmchange = false;


int jpt[3] = { 0, 0, 0 };
bool jptchange = false;

int fueltot[5] = { 0, 0, 0, 0, 0 };
bool fueltotchange = false;

int fuelbingo[4] = { 0, 0, 0, 0 };
bool fuelbingochange = false;


//==============================

int qty[2] = { 0, 0 };
bool qtychange = false;

int mult[1] = { 0 };
bool multchange = false;

int intv[3] = { 0, 0, 0 };
bool intvchange = false;

int mode[1] = { 0 };
bool modechange = false;

int fuz[2] = { 0, 0 };
bool fuzchange = false;




// int mapDigit(int rawvalue)
// {
//   return map(rawvalue, 0,65536, 0, 9);
// }


void setDrumDigit(unsigned int newValue, int *drumarray, int index, bool *changed)
{
  int setvalue = computeDigit(newValue);
  if (setvalue != drumarray[index])
  {
    drumarray[index] = setvalue;
    *changed = true;
  }
}



// FUZE drum
//////////////////////
void onFuz1Change(unsigned int newValue) {
    /* your code here */
    // 0     = -
    //       = N
    //       = T
    //       = NT
    //       = PR
    //       = OP
    // 65535 = SA
  setDrumDigit(newValue, fuz, 1, &fuzchange);

}
DcsBios::IntegerBuffer fuz1Buffer(0x78d6, 0xffff, 0, onFuz1Change);

void onFuz2Change(unsigned int newValue) {
    /* your code here */
    // 0     = -              0
    // 6553  = IN             1
    // 13107 = D1             2
    // 19660 = D2             3
    //       = V 26213
    //       = V1
    //       = V2
    // 65535 = FE             9
    setDrumDigit(newValue, fuz, 0, &fuzchange);
}
DcsBios::IntegerBuffer fuz2Buffer(0x78d8, 0xffff, 0, onFuz2Change);

DcsBios::Switch3Pos fuzeControl("FUZE_CONTROL", PIN_FUZE_UP, PIN_FUZE_DOWN);


// InVT x 10 1. Drum Digit
//////////////////////////////////
void onInvt1Change(unsigned int newValue) {
  setDrumDigit(newValue, intv, 0, &intvchange);
}
DcsBios::IntegerBuffer invt1Buffer(0x78da, 0xffff, 0, onInvt1Change);

void onInvt2Change(unsigned int newValue) {
  setDrumDigit(newValue, intv, 1, &intvchange);
}
DcsBios::IntegerBuffer invt2Buffer(0x78dc, 0xffff, 0, onInvt2Change);

void onInvt3Change(unsigned int newValue) {
  setDrumDigit(newValue, intv, 2, &intvchange);
}
DcsBios::IntegerBuffer invt3Buffer(0x78de, 0xffff, 0, onInvt3Change);

DcsBios::Switch3Pos releaseInt100("RELEASE_INT_100", PIN_INTV0_DOWN, PIN_INTV0_UP);
DcsBios::Switch3Pos releaseInt010("RELEASE_INT_010", PIN_INTV1_UP, PIN_INTV1_DOWN);
DcsBios::Switch3Pos releaseInt001("RELEASE_INT_001", PIN_INTV2_DOWN, PIN_INTV2_UP);


// ACP Mode
//////////////////////
void onModeGaugeChange(unsigned int newValue) {
    /* your code here */
    // 0     = -        0
    // 13107 = AUT      2
    // 26214 = CIP      4
    // 39321 = DSL      6
    // 52428 = DIR      8
    // 65535 = AGM      9
    setDrumDigit(newValue, mode, 0, &modechange);
}
DcsBios::IntegerBuffer modeGaugeBuffer(0x78d4, 0xffff, 0, onModeGaugeChange);

DcsBios::Switch3Pos armControl("ARM_CONTROL", 12, 11);


// MULT drum digit
//////////////////////
void onMultGaugeChange(unsigned int newValue) {
  setDrumDigit(newValue, mult, 0, &multchange);
}
DcsBios::IntegerBuffer multGaugeBuffer(0x78e0, 0xffff, 0, onMultGaugeChange);

DcsBios::Switch3Pos multiReleaseControl("MULTI_RELEASE_CONTROL", PIN_MULT_DOWN, PIN_MULT_UP);

// QTY drum digits
///////////////////////////////////////////////
void onQty1Change(unsigned int newValue) {
    /* your code here */
    setDrumDigit(newValue, qty, 0, &qtychange);
}
DcsBios::IntegerBuffer qty1Buffer(0x78e2, 0xffff, 0, onQty1Change);

void onQty2Change(unsigned int newValue) {
    /* your code here */
    setDrumDigit(newValue, qty, 1, &qtychange);
}
DcsBios::IntegerBuffer qty2Buffer(0x78e4, 0xffff, 0, onQty2Change);


DcsBios::Switch3Pos quantSel01("QUANT_SEL_01", PIN_QTY2_DOWN, PIN_QTY2_UP);
DcsBios::Switch3Pos quantSel10("QUANT_SEL_10", PIN_QTY1_DOWN, PIN_QTY1_UP);



// Jettison Mode switch
/////////////////////////////
const byte jettModePins[5] = {22, 23, 24, 25, 26};
DcsBios::SwitchMultiPos jettMode("JETT_MODE", jettModePins, 5);

// Jettison button
////////////////////
DcsBios::Switch2Pos jettStores("JETT_STORES", 27);


// HUD Master Modes (from Master Arm Panel)
/////////////////////////////////////////////////

// Master Mode VSTOL
DcsBios::ActionButton vstolSelToggle("VSTOL_SEL", "TOGGLE", PIN_HUD_VSTOL_BUTTON);
DcsBios::LED vstolLight(0x7880, 0x0400, PIN_HUD_VSTOL_LED);

// Master Mode A/G
DcsBios::ActionButton agSelToggle("AG_SEL", "TOGGLE", PIN_HUD_AG_BUTTON);
DcsBios::LED agLight(0x7880, 0x0800, PIN_HUD_AG_LED);

// Master Mode NAV
DcsBios::ActionButton navSelToggle("NAV_SEL", "TOGGLE", PIN_HUD_NAV_BUTTON);
DcsBios::LED navLight(0x7880, 0x0200, PIN_HUD_NAV_LED);

// MASTER ARM Switch
DcsBios::Switch2Pos masterArm("MASTER_ARM", PIN_MASTER_ARM_SWITCH);

// Launch Flare Salvo button
DcsBios::Switch2Pos launchFlareSalvo("LAUNCH_FLARE_SALVO", PIN_FLARE_SALVO);

// Nav Course switch
//DcsBios::Switch3Pos navCrs("NAV_CRS", PIN_NAV_COURSE1, PIN_NAV_COURSE2);
DcsBios::RotaryEncoder navCrs("NAV_CRS", "DEC", "INC", PIN_NAV_COURSE1, PIN_NAV_COURSE2);

// Master Caution Reset Button
DcsBios::Switch2Pos mCaution("M_CAUTION", PIN_MASTER_CAUTION_RESET);

// Master Warn Reset Button
DcsBios::Switch2Pos mWarning("M_WARNING", PIN_MASTER_WARN_RESET);

int computeDigit(unsigned int rawvalue)
{
  int rc = 0;
  switch(rawvalue)
  {
    case 0 ... 6552:
      rc = 0;
      break;

    case 6553 ... 13106:
      rc = 1;
      break;

    case 13107 ... 19659:
      rc = 2;
      break;

    case 19660 ... 26213:
      rc = 3;
      break;

    case 26214 ... 32766:
      rc = 4;
      break;

    case 32767 ... 39320:
      rc = 5;
      break;

    case 39321 ... 45873:
      rc = 6;
      break;

    case 45874 ... 52427:
      rc = 7;
      break;

    case 52428 ... 58980:
      rc = 8;
      break;

    case 58981 ... 65535:
      rc = 9;
      break;

  }

  return rc;
}

void setup() {
  
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setFont(&FreeSans12pt7b);
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  delay(1000);

  // Clear the buffer
  for (int a=0; a < 1; a++)
  {
    for (int i=0; i < 6; i++)
    {
      tcaselect(i);
      delay(100);
      display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
      delay(100);
      display.clearDisplay();
      display.display();

    }
  }

  for (int i=0; i < 5; i++)
    {
      tcaselect(i);
      display.clearDisplay();
      display.setCursor(20, 15);
      display.print("+");
      display.print(i);
      display.display();
      
    }

    for (int i=0; i < 6; i++)
    {
      tcaselect(i);
      display.clearDisplay();
      display.display();

    }


  printMode(4);
  printFuz(23);
  printQty(11);
  printMult(2);
  printIntv(475);

  delay(1000);

  DcsBios::setup();
}


void printMode(int value)
{
  tcaselect(0);
  display.clearDisplay();

    // 0     = -        0
    // 13107 = AUT      2
    // 26214 = CIP      3
    // 39321 = DSL      6
    // 52428 = DIR      8
  int x = 40;
  int y = 24;

  switch(value)
  {
    case 0:
      printText(x,y,"AGM", "---", "   ");
      break;
    
    case 1:
       printText(x,y,"---", "   ", "AUT");
      break;

    case 2: // AUT
      printText(x,y,"   ", "AUT", "   ");
      break;

    case 3:
      printText(x,y,"AUT", "   ", "CIP");
      break;
    
    case 4: // CIP
      printText(x,y,"   ", "CIP", "   ");
      break;

    case 5:
      printText(x,y,"CIP", "   ", "DSL");
      break;

    case 6: // DSL
      printText(x,y,"   ", "DSL", "   ");
      break;

    case 7:
      printText(x,y,"DSL", "   ", "DIR");
      break;

    case 8:
      printText(x,y,"   ", "DIR", "AGM");
      break;

    case 9:
      printText(x,y, "DIR", "AGM", "---");
      break;

  }
  display.display();
}

void printFuz(int value)
{
  int x = 30;
  int y = 24;
  tcaselect(1);
  display.clearDisplay();

  // 1st drum
  // 0     = -
  // 13107 = N
  //       = T
  //       = NT
  //       = PR
  //       = OP
  // 65535 = SA
  int digit = value % 10;
  switch(digit)
  {
    case 0:
      printText(x,y,"SA", "--", "  ");
      break;
    
    case 1:
      printText(x,y,"--", "  ", "N ");
      break;

    case 2:
      printText(x,y,"  ", "N ", "T ");
      break;

    case 3:
      printText(x,y,"N ", "T ", "NT");
      break;
    
    case 4:
      printText(x,y,"T ", "NT", "PR");
      break;

    case 5:
      printText(x,y,"NT", "PR", "OP");
      break;

    case 6:
      printText(x,y,"PR", "OP", "PR");
      break;

    case 7:
      printText(x,y,"OP", "  ", "  ");
      break;

    case 8:
      printText(x,y,"  ", "  ", "SA");
      break;

    case 9:
      printText(x,y, "  ", "SA", "--");
      break;

  }

  // 2nd drum
    // 0     = -              0
    // 6553  = IN             1
    // 13107 = D1             2
    // 19660 = D2             3
    // 26214 = V              4
    // 32767 = V1             5
    // 39321 = V2             6
    // 65535 = FE  
    value /= 10;
    digit = value;
    x += 40;
    switch(digit)
    {
      case 0:
        printText(x,y,"FE","--", "IN");
        break;

      case 1:
        printText(x,y,"--","IN", "D1");
        break;

      case 2:
        printText(x,y,"IN","D1", "D2");
        break;

      case 3:
        printText(x,y,"D1","D2", "V ");
        break;

      case 4:
        printText(x,y,"D2","V ", "V1");
        break;

      case 5:
        printText(x,y,"V ","V1", "V2");
        break;

      case 6:
        printText(x,y,"V1","V2", "  ");
        break;

      case 7:
        printText(x,y,"V2","  ", "  ");
        break;

      case 8:
        printText(x,y,"  ","  ", "FE");
        break;

      case 9:
        printText(x,y,"  ","FE", "--");
        break;
    }
   
  display.display();
}


void printQty(int value)
{
  tcaselect(2);
  display.clearDisplay();
  printNumber(40,24,2,value);
  display.display();
}

void printMult(int value)
{
  tcaselect(3);
  display.clearDisplay();
  printNumber(60,24,1,value);
  display.display();
}

void printIntv(int value)
{
    tcaselect(4);
    display.clearDisplay();
    printNumber(24,24,3,value);
    display.display();
}

void printDash(int x, int y)
{
   display.fillRect(x, y+12, 12, 4, SSD1306_WHITE);
}

void printNumber(int x, int y, int numdigits, long value)
{
  int digit = 0;
  for (int i=0; i < numdigits; i++)
  {
    digit = value % 10;
    printDigit(numdigits-i-1,x,y,digit);
    value /= 10;
  }
}

void printText(int x, int y, char *above, char *value, char *below)
{
  display.setCursor(x, y-2);
  display.print(above);

  display.setCursor(x, y+YOFFSET);
  display.print(value);

  display.setCursor(x, y+2+(YOFFSET*2));
  display.print(below);
}


void printDigit(int column, int x, int y, int digit)
{
  // clear column
  //display.fillRect(x+(column*28), 0, 12, 32, 0);

  // value above
  int value = digit - 1;
  if (value <= 0) value = 9;
  display.setCursor(x+(column*XOFFSET), y);
  display.print(value);

  // value
  display.setCursor(x+(column*XOFFSET), y+YOFFSET);
  display.print(digit);

  // value below
  value = digit + 1;
  if (value >= 9) value = 0; 
  display.setCursor(x+(column*XOFFSET), y+(YOFFSET*2));
  display.print(value);
}

unsigned int rate = 70;
unsigned long nextupdate = 0;
short maxupdate = 2;

bool wasAsleep = false;
void loop() {
  DcsBios::loop();
  unsigned long timestamp = millis();
  short count = 0;

  if (timestamp > nextupdate)
  {

    if (isDcsBiosSleeping() == true)
    {
      tcaselect(0);
      updateScreenSaver(0);

      tcaselect(1);
      updateScreenSaver(1);

      tcaselect(2);
      updateScreenSaver(2);

      tcaselect(3);
      updateScreenSaver(3);

      tcaselect(4);
      updateScreenSaver(4);

      flipflop1 = !flipflop1;
      currx++;
      if (currx > startx+25) currx = startx;

      nextupdate = timestamp + 2000;
      wasAsleep = true;
      return;      
    }

    if (wasAsleep)
    {
      qtychange = true;
      intvchange = true;
      modechange = true;
      fuzchange = true;
      multchange = true;

      wasAsleep = false;
    }




    if (qtychange && count < maxupdate)
    {
      printQty((qty[0]*10)+qty[1]);
      qtychange = false;
      count +=1;
    }

    if (intvchange && count < maxupdate)
    {
      printIntv((intv[0]*100)+(intv[1]*10)+intv[2]);
      intvchange = false;
      count +=1;
    }

    if (modechange && count < maxupdate)
    {
      printMode(mode[0]);
      modechange = false;
      count +=1;
    }

    if (fuzchange && count < maxupdate)
    {
      printFuz((fuz[0]*10)+fuz[1]);
      fuzchange = false;
      count += 1;
    }

    if (multchange && count < maxupdate)
    {
      printMult(mult[0]);
      multchange = false;
      count += 1;
    }

    nextupdate = timestamp + rate;
  }

}
