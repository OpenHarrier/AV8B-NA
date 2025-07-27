/**************************************************************************
  OPEN HARRIER
  Fuel Panel

  GNU GENERAL PUBLIC LICENSE
  Version 3, 29 June 2007
  (Refer to the official documenation of GNU Version 3, 29 June 2007 for the license)

  This software is provided AS-IS as per the GNU General Public License.

  Description:
    This code emulates the electromagnetic display drums used on the Harrier by displaying the current value on the number drum 
    in the center of the display window, and partially rendering the numbers of the drum that would appear above and below
    the currently displayed value.  There is no animation logic as the Arduino is far too slow to be able to do that animation
    in real-time.  And yes, I did experiment with trying that, but it only worked for 1 or 2 displays and became a laggy mess 
    when trying to animate more than 1 display.  

    NOTE:  There is a possiblity of increasing the i2c speed to 400Khz.  I have done no testing with that as of yet, but it could
    improve the update speed.  I still don't believe it will allow for meaningful animation.

  Requirements:

  Qty Item
  4   SSD1306 128x32 OLED displays
  1   PCA9548A (i2c Multiplexer)
  1   TODO: LED for fuel on/off indicator
  1   TODO: resistor for LED for fuel on/off indicator (adjust value as needed for LED chosen)
  1   TODO: Rotary encoder for BINGO knob
  1   TODO: 7 position rotary switch

  Wiring instructions:
    PCA9548A
      Connect +5V to VIN
      Connect GND to ground
      Connect SDA to SDA pin on Arduino
      Connect SCL to SCL pin on Arduino
      Connect SD0 to SSD1306 #0 SDA
      Connect SC0 to SSD1306 #0 SCL
      Connect SD1 to SSD1306 #1 SDA
      Connect SC1 to SSD1306 #1 SCL
      Connect SD2 to SSD1306 #2 SDA
      Connect SC2 to SSD1306 #2 SCL
      Connect SD3 to SSD1306 #3 SDA
      Connect SC3 to SSD1306 #3 SCL

    SSD1306 (x4)
      Connect +5V to VIN
      Connect GND to Ground
      Connect SDA and SCL pins as shown on the PCA9548A table above
 **************************************************************************/

#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define NUM_DISPLAYS   5 // Indicates how many SSD1306 displays this sketch will use. 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




#define TCAADDR 0x70

// Helper function to select the SSD1306 we want to send data to.
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}





int fueltot[5] = { 0, 0, 0, 0, 0 };
bool fueltotchange = false;

int fuelbingo[4] = { 0, 0, 0, 0 };
bool fuelbingochange = false;

int lbsleft[4] = { 0, 0, 0, 0 };
bool lbsleftchange = false;

int lbsright[4] = { 0, 0, 0, 0 };
bool lbsrightchange = false;


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

// Fuel TOTAL
//////////////////////////////////////////////////////////////
void onTotFuel1Change(unsigned int newValue) {
  setDrumDigit(newValue, fueltot, 0, &fueltotchange);
}
DcsBios::IntegerBuffer totFuel1Buffer(0x78b2, 0xffff, 0, onTotFuel1Change);

void onTotFuel2Change(unsigned int newValue) {
  setDrumDigit(newValue, fueltot, 1, &fueltotchange);
}
DcsBios::IntegerBuffer totFuel2Buffer(0x78b4, 0xffff, 0, onTotFuel2Change);

void onTotFuel3Change(unsigned int newValue) {
  setDrumDigit(newValue, fueltot, 2, &fueltotchange);
}
DcsBios::IntegerBuffer totFuel3Buffer(0x78b6, 0xffff, 0, onTotFuel3Change);

void onTotFuel4Change(unsigned int newValue) {
  setDrumDigit(newValue, fueltot, 3, &fueltotchange);
}
DcsBios::IntegerBuffer totFuel4Buffer(0x78b8, 0xffff, 0, onTotFuel4Change);

void onTotFuel5Change(unsigned int newValue) {
  setDrumDigit(newValue, fueltot, 4, &fueltotchange);
}
DcsBios::IntegerBuffer totFuel5Buffer(0x78ba, 0xffff, 0, onTotFuel5Change);


// BINGO display
///////////////////////////
void onBingo1Change(unsigned int newValue) {
  setDrumDigit(newValue, fuelbingo, 0, &fuelbingochange);
}
DcsBios::IntegerBuffer bingo1Buffer(0x78cc, 0xffff, 0, onBingo1Change);

void onBingo2Change(unsigned int newValue) {
  setDrumDigit(newValue, fuelbingo, 1, &fuelbingochange);
}
DcsBios::IntegerBuffer bingo2Buffer(0x78ce, 0xffff, 0, onBingo2Change);

void onBingo3Change(unsigned int newValue) {
  setDrumDigit(newValue, fuelbingo, 2, &fuelbingochange);
}
DcsBios::IntegerBuffer bingo3Buffer(0x78d0, 0xffff, 0, onBingo3Change);

void onBingo4Change(unsigned int newValue) {
  setDrumDigit(newValue, fuelbingo, 3, &fuelbingochange);
}
DcsBios::IntegerBuffer bingo4Buffer(0x78d2, 0xffff, 0, onBingo4Change);

// FUEL LBS LEFT display
///////////////////////////////
void onLbsL1Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsleft, 0, &lbsleftchange);
}
DcsBios::IntegerBuffer lbsL1Buffer(0x78bc, 0xffff, 0, onLbsL1Change);

void onLbsL2Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsleft, 1, &lbsleftchange);
}
DcsBios::IntegerBuffer lbsL2Buffer(0x78be, 0xffff, 0, onLbsL2Change);

void onLbsL3Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsleft, 2, &lbsleftchange);
}
DcsBios::IntegerBuffer lbsL3Buffer(0x78c0, 0xffff, 0, onLbsL3Change);

void onLbsL4Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsleft, 3, &lbsleftchange);
}
DcsBios::IntegerBuffer lbsL4Buffer(0x78c2, 0xffff, 0, onLbsL4Change);

// FUEL LBS RIGHT display
///////////////////////////////
void onLbsR1Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsright, 0, &lbsrightchange);
}
DcsBios::IntegerBuffer lbsR1Buffer(0x78c4, 0xffff, 0, onLbsR1Change);

void onLbsR2Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsright, 1, &lbsrightchange);
}
DcsBios::IntegerBuffer lbsR2Buffer(0x78c6, 0xffff, 0, onLbsR2Change);

void onLbsR3Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsright, 2, &lbsrightchange);
}
DcsBios::IntegerBuffer lbsR3Buffer(0x78c8, 0xffff, 0, onLbsR3Change);

void onLbsR4Change(unsigned int newValue) {
  setDrumDigit(newValue, lbsright, 3, &lbsrightchange);
}
DcsBios::IntegerBuffer lbsR4Buffer(0x78ca, 0xffff, 0, onLbsR4Change);



// DCS-BIOS returns an integer value (actually it should be an unsigned int but we use a long here because I got lazy.) that represents
// a rotational value between 0 and 65535 of the actual graphical drum for a single digit.  So DCS-BIOS will sometimes give values that 
// are inbetween 2 numbers on the number drum, and this is because that number drum is actually in the process of animating.  
// Our Arduino and usage of DCS-BIOS is unable to keep up with that animation, so we essentially use a switch statement to
// bind the ranges between digits to a fixed digit.  That's essentially what computeDigit is doing here.  It's translating the
// animation value of the number drum to a number between 0 and 9, and returning the value.
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
  if (rawvalue == 32767) rc = 5; // This line may not be needed.  I put this in to fix a weird problem before I realized that I was using the wrong datatype for value.

  //rc = map(rawvalue,0,58981,0,9);

  return rc;
}

void setup() {
  
  // The following 2 lines are for experimentation purposes in the future.
  Wire.begin();
  Wire.setClock(400000); // Set I2C clock to 400kHz

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setFont(&FreeSans12pt7b);
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

  delay(1000);

  // Initialize all of the displays
  for (int i=0; i < NUM_DISPLAYS; i++)
  {
    tcaselect(i);
    delay(100); // This delay helps address some timing issues that could occur on start.
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    delay(100); // This delay helps address some timing issues that could occur on start.
    display.clearDisplay();
    display.display();

  }

  // This code will number the displays. You can safely comment this out if you don't want it to do this on startup.
  for (int i=0; i < NUM_DISPLAYS; i++)
  {
    tcaselect(i);
    display.clearDisplay();
    display.setCursor(20, 15);
    display.print("+");
    display.print(i);
    display.display();      
  }
  //delay(1000);

  // Quickly clear all of the displays.
    for (int i=0; i < NUM_DISPLAYS; i++)
    {
      tcaselect(i);
      display.clearDisplay();
      display.display();

    }


  // Output test values so we can validate the positions of the drums.
  printBingo(8888);
  printFuelTotal(98765);
  printLbsLeft(1234);
  printLbsRight(4567);

  delay(1000);

  DcsBios::setup();
}



void printFuelTotal(long value)
{
    tcaselect(1);
    display.clearDisplay();
    printNumber(0,3,5,value);
    display.display();
}

void printBingo(int value)
{
    tcaselect(0);
    display.clearDisplay();
    printNumber(15,3,4,value);
    display.display();
}

void printLbsLeft(int value)
{
  tcaselect(3);
  display.clearDisplay();
  printNumber(15,3,4,value);
  display.display();
}

void printLbsRight(int value)
{
  tcaselect(4); // I used 4 because I messed up soldering on my #2 channel and wired SDA and SCL backwards.
  display.clearDisplay();
  printNumber(15,3,4,value);
  display.display();
}

// This function takes in an x,y coordinate on where to start rendering the number drums,
// a value of the number of number drum columns (numdigits) and a long integer value
// that is to be displayed.  Then, it processes the value 1 digit at a time, starting
// from the most significant digit (leftmost), strips that off and sends it to the printDigit
// function which will actually render that drum.
void printNumber(int x, int y, int numdigits, long value)
{
  int digit = 0;
  for (int i=0; i < numdigits; i++)
  {
    digit = value % 10; // digit will get a number between 0-9 of the most significant digit remaining of value.
    printDigit(numdigits-i-1,x,y,digit);
    value /= 10; // The most significant digit gets stripped off, leaving the remaining digits to process (if any).
  }
}


void printDigit(int column, int x, int y, int digit)
{

  // Print the number that should appear above the currently displayed number on the number drum.
  int value = digit - 1;
  if (value <= 0) value = 9;
  display.setCursor(x+(column*28), y);
  display.print(value);

  // Print the number that should be currently displayed on the number drum.
  display.setCursor(x+(column*28), y+21);
  display.print(digit);

  // Print the number that should appear below the currently displayed number on the number drum.
  value = digit + 1;
  if (value >= 9) value = 0; 
  display.setCursor(x+(column*28), y+42);
  display.print(value);
}

unsigned int rate = 70; // This sets the number of milliseconds that must past before the render routine will update any displays.
unsigned long nextupdate = 0; // This value is used to compute the next timestamp (in milliseconds) when rendering can occur again.
short maxupdate = 2; // This value is the maximum number of displays allowed to re-render at the current render loop.


void loop() {
  DcsBios::loop();
  unsigned long timestamp = millis();
  short count = 0;

  // This is the main render timing logic.
  // We only allow the displays to be updated periodically because they will stop responding if you try to 
  // update them too frequently.
  if (timestamp > nextupdate)
  {
    // If any changes have been made to the number drums, we will rerender the entire display that holds those numbers
    // only if we haven't reached the maximum number of display updates per render cycle.
    if (fueltotchange && count < maxupdate)
    {
      printFuelTotal((fueltot[0]*10000)+(fueltot[1]*1000)+(fueltot[2]*100)+(fueltot[3]*10)+fueltot[4]);
      fueltotchange = false;
      count += 1;
    }

    // If any changes have been made to the number drums, we will rerender the entire display that holds those numbers
    // only if we haven't reached the maximum number of display updates per render cycle.
    if (fuelbingochange && count < maxupdate)
    {
      printBingo((fuelbingo[0]*1000)+(fuelbingo[1]*100)+(fuelbingo[2]*10)+fuelbingo[3]);
      fuelbingochange = false;
      count +=1;
    }

    // If any changes have been made to the number drums, we will rerender the entire display that holds those numbers
    // only if we haven't reached the maximum number of display updates per render cycle.
    if (lbsleftchange && count < maxupdate )
    {
      lbsleftchange = false;
      printLbsLeft((lbsleft[0]*100)+(lbsleft[1]*100)+(lbsleft[2]*10)+lbsleft[3]);
      count += 1;
    }

    // If any changes have been made to the number drums, we will rerender the entire display that holds those numbers
    // only if we haven't reached the maximum number of display updates per render cycle.
    if (lbsrightchange && count < maxupdate )
    {
      lbsrightchange = false;
      printLbsRight((lbsright[0]*100)+(lbsright[1]*100)+(lbsright[2]*10)+lbsright[3]);
      count += 1;
    }


    nextupdate = timestamp + rate; // Calculate the next time the rendering code should run.
  }




}




























