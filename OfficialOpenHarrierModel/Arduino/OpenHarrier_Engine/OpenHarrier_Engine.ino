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

#include <Servo.h>
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
//FreeSans12pt7b.h
#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans18pt7b.h>
#include <Adafruit_SSD1306.h>




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


unsigned long lastframeupdate = 0;

void onUpdateCounterChange(unsigned int newValue) {
    lastframeupdate = millis();
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);


bool isDcsBiosSleeping()
{
  return (millis() > (lastframeupdate + 300000)); // 300,000 is 5 minutes
}

float ballx[4] = {5, 10, 15, 20};
float bally[4] = {0, 3, 10, 23};
float ballvx[4] = {1.0f, -1.0f, 1.0f, -1.0f};
float ballvy[4] = {1.0f, 1.0f, -1.0f, -1.0f};

void randomBallPos()
{
  randomSeed(analogRead(0));
  for (int i=0; i < 4; i++)
  {
    ballx[i] = random(0, 127);
    bally[i] = random(0, 31);
    ballvx[i] = ((float)random(15,30) * .05f);
    ballvy[i] = ((float)random(15,30) * .05f);
  }
}

bool flipflop1 = false;

void updateBouncingBall(int i)
{

  static float rate = 1.2f;
 
  display.clearDisplay();
    display.setCursor(5, 15);
  if (flipflop1)
  {
    display.print("      ");
  }
  else
  {
    display.print(" X X X");
  }
    


  //display.fillCircle((int)ballx[i], (int)bally[i], 4, SSD1306_WHITE);
  display.display();
  //display.fillCircle((int)ballx[i], (int)bally[i], 4, SSD1306_WHITE);
  //display.display();

  ballx[i] += (ballvx[i] * rate);
  bally[i] += (ballvy[i] * rate);

  if (ballx[i] < 0.0f)
  {
    ballvx[i] = -ballvx[i];
    ballx[i] = 0.0f;
  }

  if (ballx[i] > 127.0f)
  {
    ballvx[i] = -ballvx[i];
    ballx[i] = 127.0f;
  }

  if (bally[i] < 0)
  {
    bally[i] = 0;
    ballvy[i] = -ballvy[i];
  }

  if (bally[i] > 31)
  {
    bally[i] = 31;
    ballvy[i] = -ballvy[i];
  }

}





#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


unsigned long reset_trigger_ts = 0;
bool devices_need_reset = false;       // default to false so we don't immediate run a reset.
#define RESET_AFTER 3000               // Set this value to when you want the reset to occur, in milliseconds.  1000 = 1 second.
bool continuousReset = true;          // Set this to true if you want to continously reset the data every X milliseconds

void resetDevicesToClearBadData()
{
  if (devices_need_reset && millis() >= reset_trigger_ts)
  {
    unsigned long current_ts = millis();
    unsigned int state = 0;
    char buf[6];
    utoa(state, buf, 10);

    //send messages to force the canopy flood rings to an in-range position.
    DcsBios::tryToSendDcsBiosMessage("FLOOD_L_CAN_B", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_L_CAN_T", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_R_CAN_B", buf);
    DcsBios::tryToSendDcsBiosMessage("FLOOD_R_CAN_T", buf);

    // send message to force the whiskey compass north south heading to an in-range position.
    DcsBios::tryToSendDcsBiosMessage("COMPASS_NS", buf);

    if (!continuousReset) devices_need_reset = false;
    reset_trigger_ts = current_ts + RESET_AFTER;
  }
}

void onAcftNameChange(char* newValue) {
  ////// Uncomment the commented out lines below if only want this to run when AV8BNA module first loads.
  //if (strcmp(newValue, "AV8BNA"))
  //{
    reset_trigger_ts = millis() + RESET_AFTER;  // 1-second delay
    devices_need_reset = true;
  //}
}
DcsBios::StringBuffer<24> AcftNameBuffer(0x0000, onAcftNameChange);





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

int h2o[2] = { 0, 0};
bool h2ochange = false;



void onStabArrowChange(unsigned int newValue) {
    /* your code here */
    int setvalue = 0;

    if (newValue == 0)
    {
      setvalue = -1;
    }
    else if (newValue == 32767)
    {
      setvalue = 0;
    }
    else if (newValue > 65000)
    {
      setvalue = 1;
    }

    if (setvalue != stab[0])
    {
      stab[0] = setvalue;
      stabchange = true;
    }

    printStab(0);
}
DcsBios::IntegerBuffer stabArrowBuffer(AV8BNA_STAB_ARROW, onStabArrowChange);

void onStab1Change(unsigned int newValue) {
    /* your code here */
    //stab[1] = map(newValue, 0,65536, 0, 10);

    int setvalue = computeDigit(newValue);
    if (setvalue != stab[1])
    {
      stab[1] = setvalue;
      stabchange = true;
    }
}
DcsBios::IntegerBuffer stab1Buffer(AV8BNA_STAB1, onStab1Change);

void onStab2Change(unsigned int newValue) {
    /* your code here */
    //stab[2] = map(newValue, 0,65536, 0, 10);

    int setvalue = computeDigit(newValue);
    if (setvalue != stab[2])
    {
      stab[2] = setvalue;
      stabchange = true;
    }
}
DcsBios::IntegerBuffer stab2Buffer(AV8BNA_STAB2, onStab2Change);

int mapDigit(int rawvalue)
{
  return map(rawvalue, 0,65536, 0, 9);
}


void onDuct1Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != duct[0])
  {
    
    duct[0] = setvalue;
    ductchange = true;
  }
}
DcsBios::IntegerBuffer duct1Buffer(AV8BNA_DUCT1, onDuct1Change);

void onDuct2Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != duct[1])
  {
    duct[1] = setvalue;
    ductchange = true;
  }
}
DcsBios::IntegerBuffer duct2Buffer(AV8BNA_DUCT2, onDuct2Change);

void onDuct3Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != duct[2])
  {
    duct[2] = setvalue;
    ductchange = true;
  }
}
DcsBios::IntegerBuffer duct3Buffer(AV8BNA_DUCT3, onDuct3Change);




void onFf1Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != ff[0])
  {
    ff[0] = setvalue;
    ffchange = true;
  }
}
DcsBios::IntegerBuffer ff1Buffer(AV8BNA_FF1, onFf1Change);


void onFf2Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != ff[1])
  {
    ff[1] = setvalue;
    ffchange = true;
  }
}
DcsBios::IntegerBuffer ff2Buffer(AV8BNA_FF2, onFf2Change);

void onFf3Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != ff[2])
  {
    ff[2] = setvalue;
    ffchange = true;
  }
}
DcsBios::IntegerBuffer ff3Buffer(AV8BNA_FF3, onFf3Change);



void onRpm1Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != rpm[0])
  {
    if (setvalue > 1) setvalue = 0; // fix for garbled data
    rpm[0] = setvalue;
    rpmchange = true;
  }
}
DcsBios::IntegerBuffer rpm1Buffer(AV8BNA_RPM1, onRpm1Change);


void onRpm2Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != rpm[1])
  {
    rpm[1] = setvalue;
    rpmchange = true;
  }
}
DcsBios::IntegerBuffer rpm2Buffer(AV8BNA_RPM2, onRpm2Change);


void onRpm3Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != rpm[2])
  {
    rpm[2] = setvalue;
    rpmchange = true;
  }
}
DcsBios::IntegerBuffer rpm3Buffer(AV8BNA_RPM3, onRpm3Change);


void onRpm4Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != rpm[3])
  {
    rpm[3] = setvalue;
    rpmchange = true;
  }
}
DcsBios::IntegerBuffer rpm4Buffer(AV8BNA_RPM4, onRpm4Change);


void onJpt1Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != jpt[0])
  {
    jpt[0] = setvalue;
    jptchange = true;
  }
}
DcsBios::IntegerBuffer jpt1Buffer(AV8BNA_JPT1, onJpt1Change);

void onJpt2Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != jpt[1])
  {
    jpt[1] = setvalue;
    jptchange = true;
  }
}
DcsBios::IntegerBuffer jpt2Buffer(AV8BNA_JPT2, onJpt2Change);

void onJpt3Change(unsigned int newValue) {
  int setvalue = computeDigit(newValue);
  if (setvalue != jpt[2])
  {
    jpt[2] = setvalue;
    jptchange = true;
  }
}
DcsBios::IntegerBuffer jpt3Buffer(AV8BNA_JPT3, onJpt3Change);


// h2o water display
///////////////////////////////////
void onH2o1Change(unsigned int newValue) {
  setDrumDigit(newValue, h2o, 0, &h2ochange);
}
DcsBios::IntegerBuffer h2o1Buffer(0x78aa, 0xffff, 0, onH2o1Change);

void onH2o2Change(unsigned int newValue) {
  setDrumDigit(newValue, h2o, 1, &h2ochange);
}
DcsBios::IntegerBuffer h2o2Buffer(0x78ac, 0xffff, 0, onH2o2Change);


// Nozzle position indicator
DcsBios::ServoOutput nozzlePointer(0x78ae,6, 2300, 760);


// Helper function to make it easy to set the appropriate value in integer array that
// represents the drum values.  If the new value of the digit of that interger array is 
// different from the previous value, then we update the changed flag value (passed in as a pointer)
// to indicate that there has been a change to this display.  This will be used by the rendering
// routine to flag that a display should be rerendered.
void setDrumDigit(unsigned int newValue, int *drumarray, int index, bool *changed)
{
  int setvalue = computeDigit(newValue);
  if (setvalue != drumarray[index])
  {
    drumarray[index] = setvalue;
    *changed = true;
  }
}



int computeDigit(long rawvalue)
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
  if (rawvalue == 32767) rc = 5;

  //rc = map(rawvalue,0,58981,0,9);

  return rc;
}

void setup() {
  //Serial.begin(9600);

    randomBallPos();

  Wire.begin();
  Wire.setClock(400000); // Set I2C clock to 400kHz

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
      display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
      display.clearDisplay();
      display.display();

    }
  }

  for (int i=0; i < 6; i++)
    {
      tcaselect(i);
      display.clearDisplay();
      display.setCursor(20, 15);
      display.print("+");
      display.print(i);
      display.display();

      //delay(1000);
      
    }


  //delay(1000);
    for (int i=0; i < 6; i++)
    {
      tcaselect(i);
      display.clearDisplay();
      display.display();

    }

  printDuct(456);
  printRpm(123);
  printFuelFlow(789);
  printJpt(475);
  printH2o(50);
  printStab(0);

    delay(500);

    DcsBios::setup();
}

int x;
int drumoffset = 22; //28
int drumadjust = 2; //6
int drumval = drumoffset * -10;
int drumval2 = drumoffset * -10;
int drumval3 = drumoffset * -10;
int drumval4 = drumoffset * -10;
int modeinterval = 1; // milliseconds
unsigned long nextflop = 0;
bool flipflop = false;
unsigned long starttime = 0;
int numdisplays = 1;
int counter = 950;
int maxdrum = drumadjust;
int mindrum = (-drumoffset * 10)+drumadjust; // -244

int prevtarget[4][5] = { {1,1,1,1,1}, {2,2,2,2,2}, { 3,3,3,3,3}, {4,4,4,4,4} };
int targetvalue[4][5] = { {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0} };
int curdrumpos[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };
int targetdrumpos[4][4] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };

int modecounter = 0;
int prevmodecounter = 0;
int modeline = 0;

void printDuct(int value)
{
    tcaselect(0);
    display.clearDisplay();
    printNumber(34,3,3,value);
    display.display();
}

void printRpm(int value)
{
    tcaselect(5);
    display.clearDisplay();
    printNumber(0,3,4,value);
    display.display();
}

void printFuelFlow(int value)
{
    tcaselect(1);
    display.clearDisplay();
    printNumber(25,3,3,value);
    display.display();
}

void printJpt(int value)
{
    tcaselect(4);
    display.clearDisplay();
    printNumber(13,3,3,value);
    display.display();
}

void printH2o(int value)
{
    tcaselect(3);
    display.clearDisplay();
    printNumber(6,3,2,value);
    display.display();
}

void printStab(int value)
{
    tcaselect(2);
    display.clearDisplay();
    printNumber(64,3,2,value);
    switch(stab[0])
    {
      case -1:
        printDownArrow(38,3);
      break;

      case 0:
        printDash(38,3);
      break;

      case 1:
        printUpArrow(38,3);
      break;
    }

    display.display();
}

void printUpArrow(int x, int y)
{
    display.fillTriangle(
    x+6, y,
    x,y+12,
    x+12, y+12, SSD1306_WHITE);
    display.fillRect(x+4, y+13, 5, 15, SSD1306_WHITE);
}

void printDownArrow(int x, int y)
{
    display.fillTriangle(
    x+6, y+28,
    x,y+16,
    x+12, y+16, SSD1306_WHITE);
    display.fillRect(x+4, y+1, 5, 15, SSD1306_WHITE);
}

void printDash(int x, int y)
{
   display.fillRect(x, y+12, 12, 4, SSD1306_WHITE);
}

void printNumber(int x, int y, int numdigits, int value)
{
  int digit = 0;
  for (int i=0; i < numdigits; i++)
  {
    digit = value % 10;
    printDigit(numdigits-i-1,x,y,digit);
    value /= 10;
  }
}


void printDigit(int column, int x, int y, int digit)
{
  // clear column
  //display.fillRect(x+(column*28), 0, 12, 32, 0);

  // value above
  int value = digit - 1;
  if (value <= 0) value = 9;
  display.setCursor(x+(column*28), y);
  display.print(value);

  // value
  display.setCursor(x+(column*28), y+21);
  display.print(digit);

  // value below
  value = digit + 1;
  if (value >= 9) value = 0; 
  display.setCursor(x+(column*28), y+42);
  display.print(value);
}

unsigned int rate = 50;
unsigned long nextupdate = 0;
short maxupdate = 3;

bool wasAsleep = false;
void loop() {
  DcsBios::loop();
  unsigned long timestamp = millis();
  short count = 0;

  resetDevicesToClearBadData(); // Add this to the main loop of your Arduino sketch

  if (timestamp > nextupdate)
  {
    if (isDcsBiosSleeping() == true)
    {
      tcaselect(0);
      updateBouncingBall(0);
      
      tcaselect(1);
      updateBouncingBall(1);

      tcaselect(2);
      updateBouncingBall(2);

      tcaselect(3);
      updateBouncingBall(3);

      tcaselect(4);
      updateBouncingBall(0);

      tcaselect(5);
      updateBouncingBall(1);
      flipflop1 = !flipflop1;

      nextupdate = timestamp + 2000;
      wasAsleep = true;
      return;  
    }

    if (wasAsleep)
    {
      stabchange = true;
      ductchange = true;
      ffchange = true;
      rpmchange = true;
      jptchange = true;
      h2ochange = true;
      wasAsleep = false;
    }

    if (stabchange & count < maxupdate)
    {
      printStab((stab[1]*10)+stab[2]);
      stabchange = false;
      count += 1;
    }

    if (ductchange & count < maxupdate)
    {
      printDuct((duct[0]*100)+(duct[1]*10)+duct[2]);
      ductchange = false;
      count +=1;
    }

    if (ffchange && count < maxupdate)
    {
      printFuelFlow((ff[0]*100)+(ff[1]*10)+ff[2]);
      ffchange = false;
      count +=1;
    }

    if (rpmchange && count < maxupdate)
    {
      printRpm((rpm[0]*1000)+(rpm[1]*100)+(rpm[2]*10)+rpm[3]);
      rpmchange = false;
      count +=1;
    }

    if (jptchange && count < maxupdate)
    {
      printJpt((jpt[0]*100)+(jpt[1]*10)+jpt[2]);
      jptchange = false;
      count +=1;
    }

    if (h2ochange && count < maxupdate)
    {
      printH2o((h2o[0]*10)+h2o[1]);
      h2ochange = false;
      count +=1;
    }

    nextupdate = timestamp + rate;
  }




}

















