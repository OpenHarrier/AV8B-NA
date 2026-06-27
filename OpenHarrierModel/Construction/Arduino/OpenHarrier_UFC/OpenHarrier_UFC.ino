/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
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
#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define PIXWIDTH 3
#define PIXSPACE 1

#define paddingLarge 2
#define paddingSmall 1
byte offsetLarge = PIXWIDTH + paddingLarge;
byte offsetSmall = PIXWIDTH + paddingSmall;

#define OFFSET   (PIXWIDTH + PIXSPACE)

// Issues: 
// 2026-05-30  COM2 rotary encoder is broken... 2 contacts on rotary encoder have separated.
// 2026-05-30  IFF button doesn't work or has a short.  I disabled code below for now.
// 2026-05-30  WOF button isn't working right.  It presses and then acts like it is pressed a second time.


const int pinMasterCautionLight = 11;
const int pinEncoderCom1A =  7;
const int pinEncoderCom1B =   8;
const int pinEncoderCom2A =   10;
const int pinEncoderCom2B =   9;

const int pinUfcTimer = 5;
const int pinUfcToo = 4;
const int pinUfcBtn1 = 3;
const int pinUfcBtn4 = 2;
const int pinUfcBtn7 = 22;
const int pinUfcEnt  = 23;
const int pinUfcIff  = 24;
const int pinUfcWof  = 25;
const int pinUfcBtn2 = 26;
const int pinUfcBtn5 = 27;
const int pinUfcBtn8 = 28;
const int pinUfcBtn0 = 29;
const int pinUfcTcn  = 30;
const int pinUfcBcn  = 31;
const int pinUfcBtn3 = 32;
const int pinUfcBtn6 = 33;
const int pinUfcBtn9 = 34;
const int pinUfcDot  = 35;
const int pinUfcAwl  = 36;
const int pinUfcAlt  = 37;
const int pinUfcClr  = 38;
const int pinUfcSve  = 39;
const int pinUfcDash = 40;
const int pinUfcOnOff = 41;
const int pinUfcWpn  = 42;
const int pinUfcEmCom = 43;
const int pinUfcCircle = 44;

const int pinUfcFuelLeft = 45;
const int pinUfcFuelRight = 46;
const int pinUfc15Sec = 47;
const int pinUfcMFS = 48;
const int pinUfcBingo = 49;
const int pinUfcH2o = 50;

const int pinUfcFire = 51;
const int pinUfcLaw = 52;
const int pinUfcFlaps = 53;
const int pinUfcLTank = 14;
const int pinUfcRTank = 15;
const int pinUfcHyd = 16;
const int pinUfcGear = 17;
const int pinUfcOT = 18;
const int pinUfcJPTL = 19;

// do not use 20 (SDA)
// do not use 21 (SDC)

const int pinUfcEFC = A2;
const int pinUfcGEN = A6;

//const int pinMasterCautionReset = 10;

// 10, 9 = rotary encoder com 1
// 7, 8 = rotary encoder com 2


const int pinMasterWarnLight = 12;
//const int pinMasterWarnResetButton = 9;

const int pinCom1Push = 6;
const int pinCom2Push = 5;

const int pinCom1Volume = A8;
const int pinCom2Volume = A9;
const int pinUfcBrightness = A7;

char segbuffer[17] =  { "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0" };

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

bool pong = false;
bool isHarrier = false;
unsigned long lastcheck = 0;


///////////////////////////////////////////////////////////////////
// SCREENSAVER to prevent DIMMING of OLED pixels over time
////////////////////////////////////////////////////////////////////////////////////
unsigned long lastframeupdate = 0;
bool flipflop1 = false;

void onUpdateCounterChange(unsigned int newValue) {
    lastframeupdate = millis();
}
DcsBios::IntegerBuffer UpdateCounterBuffer(0xfffe, 0x00ff, 0, onUpdateCounterChange);


bool isDcsBiosSleeping()
{
  return (millis() > (lastframeupdate + 300000)); // 5 minutes
  //return (millis() > (lastframeupdate + 5000)); // 5 minutes
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

/////////////////////////////////////////////////////

DcsBios::Switch2Pos ufcCom1Pull("UFC_COM1_PULL", pinCom1Push);
//DcsBios::Switch2Pos ufcCom2Pull("UFC_COM2_PULL", pinCom2Push);
DcsBios::PotentiometerEWMA<5, 256, 5> ufcCom1Vol("UFC_COM1_VOL", pinCom1Volume);
DcsBios::PotentiometerEWMA<5, 256, 5> ufcCom2Vol("UFC_COM2_VOL", pinCom2Volume);
DcsBios::PotentiometerEWMA<5, 256, 5> ufcBright("UFC_BRIGHT", pinUfcBrightness);

typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> CusomRotaryEncoder; // A custom rotary encoder with four quadrature steps per physical detent.
CusomRotaryEncoder ufcCom1Sel("UFC_COM1_SEL", "-3200", "+3200", pinEncoderCom1A, pinEncoderCom1B);

CusomRotaryEncoder ufcCom2Sel("UFC_COM2_SEL", "-3200", "+3200", pinEncoderCom2A, pinEncoderCom2B);

DcsBios::Switch2Pos ufcTimer("UFC_TIMER", pinUfcTimer);
DcsBios::Switch2Pos ufcTarget("UFC_TARGET", pinUfcToo);
DcsBios::Switch2Pos ufcB1("UFC_B1", pinUfcBtn1);
DcsBios::Switch2Pos ufcB4("UFC_B4", pinUfcBtn4);
DcsBios::Switch2Pos ufcB7("UFC_B7", pinUfcBtn7);
DcsBios::Switch2Pos ufcEnter("UFC_ENTER", pinUfcEnt);
//DcsBios::Switch2Pos ufcIff("UFC_IFF", pinUfcIff);  // Not working... appears to be a short.
DcsBios::Switch2Pos ufcWay("UFC_WAY", pinUfcWof);
DcsBios::Switch2Pos ufcB2("UFC_B2", pinUfcBtn2);
DcsBios::Switch2Pos ufcB5("UFC_B5", pinUfcBtn5);
DcsBios::Switch2Pos ufcB8("UFC_B8", pinUfcBtn8);
DcsBios::Switch2Pos ufcB0("UFC_B0", pinUfcBtn0);
DcsBios::Switch2Pos ufcTacan("UFC_TACAN", pinUfcTcn);
DcsBios::Switch2Pos ufcBeacon("UFC_BEACON", pinUfcBcn);
DcsBios::Switch2Pos ufcB3("UFC_B3", pinUfcBtn3);
DcsBios::Switch2Pos ufcB6("UFC_B6", pinUfcBtn6);
DcsBios::Switch2Pos ufcB9("UFC_B9", pinUfcBtn9);
DcsBios::Switch2Pos ufcDot("UFC_DOT", pinUfcDot);
DcsBios::Switch2Pos ufcWeather("UFC_WEATHER", pinUfcAwl);
DcsBios::Switch2Pos ufcAlt("UFC_ALT", pinUfcAlt);
DcsBios::Switch2Pos ufcClear("UFC_CLEAR", pinUfcClr);
DcsBios::Switch2Pos ufcSave("UFC_SAVE", pinUfcSve);
DcsBios::Switch2Pos ufcDash("UFC_DASH", pinUfcDash);
DcsBios::Switch2Pos ufcOnoff("UFC_ONOFF", pinUfcOnOff);
DcsBios::Switch2Pos ufcWeapon("UFC_WEAPON", pinUfcWpn);
DcsBios::Switch2Pos ufcEmcom("UFC_EMCOM", pinUfcEmCom);
DcsBios::Switch2Pos ufcIp("UFC_IP", pinUfcCircle);

// Left UFC indicators
DcsBios::LED lfuelLight(0x787c, 0x0010, pinUfcFuelLeft);
DcsBios::LED rfuelLight(0x787c, 0x0020, pinUfcFuelRight);
DcsBios::LED fifteenSekLight(0x787c, 0x0040, pinUfc15Sec);
DcsBios::LED mfsLight(0x787c, 0x0080, pinUfcMFS);
DcsBios::LED bingoLight(0x787c, 0x0100, pinUfcBingo);
DcsBios::LED h2oLight(0x787c, 0x0200, pinUfcH2o);

// Right UFC indicators
DcsBios::LED fireLight(0x787c, 0x0400, pinUfcFire);
DcsBios::LED lawLight(0x787c, 0x0800, pinUfcLaw);
DcsBios::LED flapsLight(0x787c, 0x1000, pinUfcFlaps);
DcsBios::LED ltankLight(0x787c, 0x2000, pinUfcLTank);
DcsBios::LED rtankLight(0x787c, 0x4000, pinUfcRTank);
DcsBios::LED hhydLight(0x787c, 0x8000, pinUfcHyd);
DcsBios::LED gearLight(0x7880, 0x0001, pinUfcGear);
DcsBios::LED otLight(0x7880, 0x0002, pinUfcOT);
DcsBios::LED jptlLight(0x7880, 0x0004, pinUfcJPTL);
DcsBios::LED efcLight(0x7880, 0x0008, pinUfcEFC);
DcsBios::LED genLight(0x7880, 0x0010, pinUfcGEN);

void setup() {

 // Serial.begin(115200);
    //Serial.println("Hello World!");
  Wire.begin();


  pinMode(pinMasterCautionLight, OUTPUT);
  pinMode(pinMasterWarnLight, OUTPUT);

 
  

   //SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally

  /*TCA displays should be wired as follows:
  0 = COMM 1
  1 = COMM 2
  2 = right scratchpad
  3 = middle scratchpad
  4 = left scratchpad
  */

  for (int i=0; i < 5; i++)
  {
    tcaselect(i);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    delay(100);
    
    display.setTextSize(0); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();
    char test = ' ';
    switch(i)
    {
      case 0:
        test = '0';
        display.setFont(&FreeSans12pt7b);
        break;
      
      case 1:
        test = '1';
        break;

      case 2:
        test = '2';
        break;

      case 3:
        test = '3';
        break;

      case 4:
        test = '4';
        break;        
    }
    drawSmallChar(test,20,  20, offsetSmall);
    display.display();

    delay(100);

  }

  

  //digitalWrite(pinMasterCautionLight, HIGH);
  //digitalWrite(pinMasterWarnLight, HIGH );

  // for (int i=0; i < 5; i++)
  // {
  //   tcaselect(i);
  //   testdrawrect();
  // }


  


   delay(500);
   
   
   
   DcsBios::setup();
   
   
 
}


void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}




// typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> FourStepRotaryEncoder;
// FourStepRotaryEncoder ufcCom1Sel("UFC_COM1_SEL", "-3200", "3200", 11, 12);



           
char textScratchpad[13] = { "OPENHARRIERXX" };
char textCom1[3] = { "12" };
char textCom2[3] = { "34" };

unsigned int rate = 70;
unsigned long nextupdate = 0;
short maxupdate = 2;

bool wasAsleep = false;
void loop() {
  DcsBios::loop();
  unsigned long timestamp = millis();

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


      nextupdate = timestamp + 2000;
      wasAsleep = true;
      return;      
    }

    if (wasAsleep)
    {
      // Restore whatever was displayed before we pasued
      renderScratchPad(textScratchpad);
      renderCom1(textCom1);
      renderCom2(textCom2);
      // for redraw
      //qtychange = true;
      //intvchange = true;
      //modechange = true;
      //fuzchange = true;
      //multchange = true;

      wasAsleep = false;
    }
  }
}



void renderScratchPad(char* newValue) {
  tcaselect(4);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  drawComChar(newValue[0],0,20, offsetSmall);
  drawComChar(newValue[1],43,20, offsetSmall);
  drawComChar(newValue[2],86,20, offsetSmall);

  //drawSmallChar(newValue[0],0,20, offsetSmall);
  //drawSmallChar(newValue[1],43,20, offsetSmall);
  //drawSmallChar(newValue[2],86,20, offsetSmall);

  // display.setTextSize(0);
    // display.setCursor(10, 60);
    // display.setTextColor(SSD1306_WHITE);
    // display.print("\"");
    // display.print(newValue);
    // display.print("\"");

  display.display();

  tcaselect(3);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawSmallChar(newValue[3],0,  20, offsetSmall);
  drawSmallChar(newValue[4],27, 20, offsetSmall);
  drawSmallChar(newValue[5],55,  20, offsetSmall);
  drawSmallChar(newValue[6],83,  20, offsetSmall);
  drawSmallChar(newValue[7],111,  20, offsetSmall);
  display.display();

  tcaselect(2);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawSmallChar(newValue[8],0,  20, offsetSmall);
  drawSmallChar(newValue[9],27, 20, offsetSmall);
  drawSmallChar(newValue[10],55,  20, offsetSmall);
  drawSmallChar(newValue[11],83,  20, offsetSmall);
  drawSmallChar(newValue[12],111,  20, offsetSmall);
  display.display();  
}


void onUfcScratchpadChange(char* newValue) {
  memcpy(textScratchpad, newValue, 13);
  renderScratchPad(newValue);

  
}
DcsBios::StringBuffer<12> ufcScratchpadBuffer(0x7976, onUfcScratchpadChange);





// void updateComPreselect(char* newValue) {
//   display.clearDisplay();
//   char a = newValue[0];
//   char b = newValue[1];

//   drawComChar(a,32,10,offsetLarge);
//   drawComChar(b,85,10,offsetLarge);
//   display.display(); 
// }

void renderCom1(char* newValue) {
  tcaselect(0);
  display.clearDisplay();
  char a = newValue[0];
  char b = newValue[1];

  drawComChar(a,29,10,offsetLarge);     // was 32
  drawComChar(b,82,10,offsetLarge);     // was 85
  display.display(); 
}

void renderCom2(char* newValue) {
  tcaselect(1);
  display.clearDisplay();
  char a = newValue[0];
  char b = newValue[1];

  drawComChar(a,29,10,offsetLarge);
  drawComChar(b,82,10,offsetLarge);
  display.display(); 
}


void onUfcComm1DisplayChange(char* newValue) {
  memcpy(textCom1, newValue, 3);
  renderCom1(newValue);
}
DcsBios::StringBuffer<2> ufcComm1DisplayBuffer(0x7954, onUfcComm1DisplayChange);


void onUfcComm2DisplayChange(char* newValue) {
  memcpy(textCom2, newValue, 3);
  renderCom2(newValue);
}
DcsBios::StringBuffer<2> ufcComm2DisplayBuffer(0x7956, onUfcComm2DisplayChange);


//DcsBios::RotaryEncoder ufcCom1Sel("UFC_COM1_SEL", "-3200", "+3200", 8, 9);
//typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> CusomRotaryEncoder; // A custom rotary encoder with four quadrature steps per physical detent.
//CusomRotaryEncoder ufcCom1Sel("UFC_COM1_SEL", "-3200", "+3200", 8, 9);


/*
 *  -A-   -B-
 * | \  |  / |
 * H  M L K  C
 * |   \|/   |
 *  -I-  -J-
 * |   /|\   |
 * G N  O P  D
 * |/   |  \ |
 *  -F-  -E-
 * 
 */

void drawDot(short offx, short offy)
{
  display.drawLine(offx-1,  offy,   offx+1, offy,   SSD1306_WHITE);
  display.drawLine(offx,   offy-1,  offx,   offy+1, SSD1306_WHITE);  
}

void drawSegHorizontal(short offx, short offy, short spacing)
{
  for (int i=0; i < 3; i++)
  {
    drawDot(offx+(spacing*i), offy);
    //display.drawLine(offx-1+(spacing*i),  offy,   offx+1+(spacing*i), offy, SSD1306_WHITE);
    //display.drawLine(offx+(spacing*i),    offy-1, offx+(spacing*i),   offy+1, SSD1306_WHITE);  
  }
 
  //display.fillCircle(offx+0,offy+0,           1, SSD1306_INVERSE);
  //display.fillCircle(offx+spacing,offy+0,      1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*2),offy+0,  1, SSD1306_INVERSE);
  
}

void drawSegVertical(short offx, short offy, short spacing)
{
  for (int i=0; i < 3; i++)
  {
    drawDot(offx, offy+(spacing*i));
    //display.drawLine(offx-1,  offy+(spacing*i),   offx+1, offy+(spacing*i), SSD1306_WHITE);
    //display.drawLine(offx,    offy-1+(spacing*i), offx, offy+1+(spacing*i), SSD1306_WHITE);  
  }

  //display.fillCircle(offx+0,offy+0,           1, SSD1306_INVERSE);
  //display.fillCircle(offx+0,offy+spacing,      1, SSD1306_INVERSE);
  //display.fillCircle(offx+0,offy+(spacing*2),  1, SSD1306_INVERSE);
}

void drawSegM(short offx, short offy, short spacing)
{

  drawDot(offx+(spacing*1), offy+(spacing*1));
  drawDot(offx+(spacing*2), offy+(spacing*2));
  drawDot(offx+(spacing*3), offy+(spacing*3));
  
  //display.fillCircle(offx+(spacing*1),       offy+(spacing*1),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*2),       offy+(spacing*2),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*3),       offy+(spacing*3),           1, SSD1306_INVERSE);
  

}
void drawSegK(short offx, short offy, short spacing)
{
  drawDot(offx+(spacing*7), offy+(spacing*1));
  drawDot(offx+(spacing*6), offy+(spacing*2));
  drawDot(offx+(spacing*5), offy+(spacing*3));

  //display.fillCircle(offx+(spacing*7),        offy+(spacing*1),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*6),        offy+(spacing*2),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*5),        offy+(spacing*3),          1, SSD1306_INVERSE);  
  
}

void drawSegN(short offx, short offy, short spacing)
{
  drawDot(offx+(spacing*1), offy+(spacing*7));
  drawDot(offx+(spacing*2), offy+(spacing*6));
  drawDot(offx+(spacing*3), offy+(spacing*5));

  //display.fillCircle(offx+(spacing*1),        offy+(spacing*7),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*2),        offy+(spacing*6),            1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*3),        offy+(spacing*5),           1, SSD1306_INVERSE);
  
}

void drawSegP(short offx, short offy, short spacing)
{
  drawDot(offx+(spacing*7), offy+(spacing*7));
  drawDot(offx+(spacing*6), offy+(spacing*6));
  drawDot(offx+(spacing*5), offy+(spacing*5));
  
  //display.fillCircle(offx+(spacing*7),        offy+(spacing*7),           1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*6),        offy+(spacing*6),             1, SSD1306_INVERSE);
  //display.fillCircle(offx+(spacing*5),        offy+(spacing*5),           1, SSD1306_INVERSE);

 
}

void drawSegColon(short offx, short offy, short spacing)
{

  drawDot(offx, offy+(spacing*1));
  drawDot(offx, offy+(spacing*2));
  drawDot(offx, offy+(spacing*5));
  drawDot(offx, offy+(spacing*6));

  
  //offy += (spacing*5);
  //display.fillCircle(offx+0,offy+spacing,           1, SSD1306_INVERSE);
  //display.fillCircle(offx+0,offy+(spacing*2),               1, SSD1306_INVERSE);
  
  
}

void drawSegA(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+spacing,  offy+0, spacing);              // A 
}

void drawSegB(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+(spacing*5), offy+0, spacing);  // B
}

void drawSegC(short offx, short offy, short spacing)
{
  drawSegVertical(offx+(spacing*8), offy+spacing, spacing);
}

void drawSegD(short offx, short offy, short spacing)
{
  drawSegVertical(offx+(spacing*8), offy+(spacing*5), spacing);
}

void drawSegE(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+(spacing*5), offy+(spacing*8), spacing);
}

void drawSegF(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+spacing,     offy+(spacing*8), spacing);
}

void drawSegG(short offx, short offy, short spacing)
{
  drawSegVertical(offx+0,  offy+(spacing*5), spacing);
}

void drawSegH(short offx, short offy, short spacing)
{
  drawSegVertical(offx+0,  offy+spacing, spacing); 
}

void drawSegI(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+spacing,     offy+(spacing*4), spacing);
}

void drawSegJ(short offx, short offy, short spacing)
{
  drawSegHorizontal(offx+(spacing*5), offy+(spacing*4), spacing);
}

void drawSegL(short offx, short offy, short spacing)
{
  drawSegVertical(offx+(spacing*4),  offy+spacing, spacing);
}

void drawSegO(short offx, short offy, short spacing)
{
   drawSegVertical(offx+(spacing*4),  offy+(spacing*5), spacing);
}
 


 
// ABCDEFGHIJKLMNOP
void drawSegments2(short segments, short x, short y, short spacing)
{
  byte low = lowByte(segments);
  byte high = highByte(segments);

            //ABCDEFGH IJKLMNOP
  if (high & B10000000) drawSegA(x,y,spacing);
  if (high & B01000000) drawSegB(x,y,spacing);
  if (high & B00100000) drawSegC(x,y,spacing);
  if (high & B00010000) drawSegD(x,y,spacing);
  if (high & B00001000) drawSegE(x,y,spacing);
  if (high & B00000100) drawSegF(x,y,spacing);
  if (high & B00000010) drawSegG(x,y,spacing);
  if (high & B00000001) drawSegH(x,y,spacing);

  if (low & B10000000) drawSegI(x,y,spacing);
  if (low & B01000000) drawSegJ(x,y,spacing);
  if (low & B00100000) drawSegK(x,y,spacing);
  if (low & B00010000) drawSegL(x,y,spacing);
  if (low & B00001000) drawSegM(x,y,spacing);
  if (low & B00000100) drawSegN(x,y,spacing);
  if (low & B00000010) drawSegO(x,y,spacing);
  if (low & B00000001) drawSegP(x,y,spacing);
  
}

void drawSegments(char *segments, short x, short y, short spacing)
{
  //int max = segments.length();
  for (int i=0; i < 20; i++)           // was for (int i=0; i < 16; i++)
  {
    //char c = segments.charAt(i);
    char c = segments[i];
    if (c == 0x00) break;
    switch(c)
    {
      case 'A': drawSegA(x,y,spacing); break;
      case 'B': drawSegB(x,y,spacing); break;
      case 'C': drawSegC(x,y,spacing); break;
      case 'D': drawSegD(x,y,spacing); break;
      case 'E': drawSegE(x,y,spacing); break; 
      case 'F': drawSegF(x,y,spacing); break;
      case 'G': drawSegG(x,y,spacing); break;
      case 'H': drawSegH(x,y,spacing); break;
      case 'I': drawSegI(x,y,spacing); break;
      case 'J': drawSegJ(x,y,spacing); break;
      case 'K': drawSegK(x,y,spacing); break;
      case 'L': drawSegL(x,y,spacing); break;
      case 'M': drawSegM(x,y,spacing); break;
      case 'N': drawSegN(x,y,spacing); break;
      case 'O': drawSegO(x,y,spacing); break;
      case 'P': drawSegP(x,y,spacing); break;
    }
  }
}

void drawSmallChar(char c, short x, short y, short spacing)
{
  switch (c)
  {
    case '-':
    //drawSegments("I\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B0000000010000000
    drawSegments2(0x80,x,y,spacing);       
    break;
    
    case '@':
    //drawSegments("ALIH\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000000110010000
    drawSegments2(0x8190,x,y,spacing);      
    break;

    case '\'':
    //drawSegments("H\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B0000000000010000
    drawSegments2(0x10,x,y,spacing);     
    break;

    case '0':
    //drawSegments("ALOFGH\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000011100010010
    drawSegments2(0x8712,x,y,spacing);      
    break;

    case '1':
    //drawSegments("LO\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B0000000000010010
    drawSegments2(0x12,x,y,spacing);     
    break;

    case '2':
    //drawSegments("ALIGF\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000011010010000
    drawSegments2(0x8690,x,y,spacing);          
    break;

    case '3':
    //drawSegments("ALIOF\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000010010010010
    drawSegments2(0x8492,x,y,spacing);      
    break;

    case '4':
    //drawSegments("HILO\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B0000000110010010
    drawSegments2(0x192,x,y,spacing);       
    break;

    case '5':
    //drawSegments("AHIOF\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000010110000010
    drawSegments2(0x8582,x,y,spacing);    
    break;

    case '6':
    //drawSegments("AHGFOI\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000011110000010
    drawSegments2(0x8782,x,y,spacing);
    break;

    case '7':
    //drawSegments("ALO\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000000000010010
    drawSegments2(32786,x,y,spacing);
    break;

    case '8':
    //drawSegments("ALOFGHI\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000011110010010
    drawSegments2(34706,x,y,spacing);
    break;

    case '9':
    //drawSegments("AHLOI\0",x,y,spacing);
    //             ABCDEFGHIJKLMNOP
    //            B1000000110010010
    drawSegments2(0x8192,x,y,spacing);    
    break;


    case '.':
    drawDot(x+(spacing*2), y+(spacing*7));
    drawDot(x+(spacing*3), y+(spacing*7));
    break;

    default:
      // Added this code for DEBUGGING when getting bad data.
      //drawSegments2(0x80,x,y,spacing);
      break;
  }
}

void drawComChar(char c, short x, short y, short spacing)
{
  switch (c)
  {
    case '-':
      drawSegJ(x,y,spacing);
      break;
      
    case '@':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegL(x,y,spacing);
      break;

    case '\'':
      drawSegC(x,y,spacing);
      break;
    
    case '0':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegE(x,y,spacing);
      drawSegO(x,y,spacing);
      drawSegL(x,y,spacing);
      break;

    case '1':
      drawSegC(x,y,spacing);
      drawSegD(x,y,spacing);
      break;

    case '2':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegO(x,y,spacing);
      drawSegE(x,y,spacing);
      break;

    case '3':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegE(x,y,spacing);
      break;

    case '4':
      drawSegC(x,y,spacing);
      drawSegL(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegD(x,y,spacing);
      break;

    case '5':
      drawSegB(x,y,spacing);
      drawSegL(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegE(x,y,spacing);
      break;

    case '6':
      drawSegB(x,y,spacing);
      drawSegL(x,y,spacing);
      drawSegJ(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegE(x,y,spacing);
      drawSegO(x,y,spacing);
      break;

    case '7':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegD(x,y,spacing);
      break;

    case '8':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegE(x,y,spacing);
      drawSegO(x,y,spacing);
      drawSegL(x,y,spacing);
      drawSegJ(x,y,spacing);
      break;

    case '9':
      drawSegB(x,y,spacing);
      drawSegC(x,y,spacing);
      drawSegD(x,y,spacing);
      drawSegL(x,y,spacing);
      drawSegJ(x,y,spacing);
      break;
      
     

    case 'A':  drawSegments("GHABCDIJ\0",x,y,spacing); break;
    case 'B':  drawSegments("ABCDEFLOJ\0",x,y,spacing); break;
    case 'C':  drawSegments("ABHGFE\0",x,y,spacing); break;
    case 'D': drawSegments("ABCDEFLO\0",x,y,spacing); break;
    case 'E': drawSegments("ABHGFEI\0",x,y,spacing); break;
    case 'F': drawSegments("HGABI\0",x,y,spacing); break;
    case 'G': drawSegments("ABHGFEDJ\0",x,y,spacing);break;
    case 'H': drawSegments("HGIJCD\0",x,y,spacing); break;
    case 'I': drawSegments("ABLOFE\0",x,y,spacing); break;
    case 'J': drawSegments("ABLOFG\0",x,y,spacing); break; 
    case 'K': drawSegments("HGIKP\0",x,y,spacing); break;
    case 'L': drawSegments("HGFE\0",x,y,spacing); break;
    case 'M': drawSegments("HGMKCD\0",x,y,spacing); break;
    case 'N': drawSegments("GHMPDC\0",x,y,spacing); break;
    case 'O': 
      drawSegments("ABCDEFGH\0",x,y,spacing); break;
      // drawSegA(x,y,spacing);
      // drawSegB(x,y,spacing);
      // drawSegC(x,y,spacing);
      // drawSegD(x,y,spacing);
      // drawSegE(x,y,spacing);
      // drawSegF(x,y,spacing);
      // drawSegG(x,y,spacing);
      // drawSegH(x,y,spacing);
      // break;
    case 'P': drawSegments("IJCBAHG\0",x,y,spacing); break;
    case 'Q': drawSegments("ABCDEFGHP\0",x,y,spacing); break;
    case 'R': drawSegments("ABCJIHGP\0",x,y,spacing); break;
    case 'S': drawSegments("BAHIJDEF\0",x,y,spacing); break;
    case 'T': drawSegments("ABLO\0",x,y,spacing); break;
    case 'U': drawSegments("HGFEDC\0",x,y,spacing); break;
    case 'V': drawSegments("HGNK\0",x,y,spacing); break;
    case 'W': drawSegments("HGNPDC\0",x,y,spacing); break;
    case 'X': drawSegments("MKNP\0",x,y,spacing); break;
    case 'Y': drawSegments("MKO\0",x,y,spacing); break;
    case 'Z': drawSegments("ABKNFE\0",x,y,spacing); break;

    case '*': drawSegments("MLKJPONI\0",x,y,spacing); break;
    //default: drawSegments("MLKJPONI\0",x,y,spacing); break;
    
  }
}






#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2



// Master Caution

DcsBios::LED mcLight(0x787c, 0x0004, pinMasterCautionLight);

// // Master Warn
//DcsBios::Switch2Pos mWarning("M_WARNING", pinMasterWarnResetButton);
DcsBios::LED mwLight(0x787c, 0x0008, pinMasterWarnLight);


//DcsBios::Switch2Pos mCaution("M_CAUTION", pinMasterCautionReset);
