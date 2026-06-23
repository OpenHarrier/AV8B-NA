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

#define DCSBIOS_IRQ_SERIAL
#include "DcsBios.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define PIXWIDTH 2
#define PIXSPACE 1

#define paddingLarge 2
#define paddingSmall 1
byte offsetLarge = PIXWIDTH + paddingLarge;
byte offsetSmall = PIXWIDTH + paddingSmall;

#define OFFSET   (PIXWIDTH + PIXSPACE)


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


#define ODU_PIN_OPT1   12
#define ODU_PIN_OPT2   11
#define ODU_PIN_OPT3   10
#define ODU_PIN_OPT4   9
#define ODU_PIN_OPT5   8


// #define NUMFLAKES     10 // Number of snowflakes in the animation example

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}



int interval = 250;
bool oduselect1 = false;
bool oduselect2 = false;
bool oduselect3 = false;
bool oduselect4 = false;
bool oduselect5 = false;
char odu1[5] = { "1234" };
char odu2[5] = { "1234" };
char odu3[5] = { "1234" };
char odu4[5] = { "1234" };
char odu5[5] = { "1234" };



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
  //return (millis() > (lastframeupdate + 3000)); // 5 minutes
  return (millis() > (lastframeupdate + 300000)); // 5 minutes
}

int startx = 25;
int currx = startx;
void updateScreenSaver(int i)
{
 

  display.clearDisplay();
  display.setCursor(currx, 20);
  if (flipflop1)
    display.print(("     "));
  else
    display.print(("X X X"));
  //display.fillCircle((int)ballx[i], (int)bally[i], 4, SSD1306_WHITE);
  display.display();
  return;

}

/////////////////////////////////////////////////////










// ODU 1
////////////////////
void onAv8bnaOdu1SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect1 = true;
    else
      oduselect1 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu1, oduselect1, 4);      
}
DcsBios::StringBuffer<1> av8bnaOdu1SelectBuffer(0x7958, onAv8bnaOdu1SelectChange);

void onAv8bnaOdu1TextChange(char* newValue) {

  setMyDisplayChars(newValue, oduselect1, 4);

  for (int i=0; i < 4; i++)
  {
    odu1[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu1TextBuffer(0x795a, onAv8bnaOdu1TextChange);



// ODU 2
////////////////////
void onAv8bnaOdu2SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect2 = true;
    else
      oduselect2 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu2, oduselect2, 3);      
}
DcsBios::StringBuffer<1> av8bnaOdu2SelectBuffer(0x795e, onAv8bnaOdu2SelectChange);

void onAv8bnaOdu2TextChange(char* newValue) {

  setMyDisplayChars(newValue, oduselect2, 3);

  for (int i=0; i < 4; i++)
  {
    odu2[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu2TextBuffer(0x7960, onAv8bnaOdu2TextChange);



// ODU 3
////////////////////
void onAv8bnaOdu3SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect3 = true;
    else
      oduselect3 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu3, oduselect3, 2);      
}
DcsBios::StringBuffer<1> av8bnaOdu3SelectBuffer(0x7964, onAv8bnaOdu3SelectChange);

void onAv8bnaOdu3TextChange(char* newValue) {

  setMyDisplayChars(newValue, oduselect3, 2);

  for (int i=0; i < 4; i++)
  {
    odu3[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu3TextBuffer(0x7966, onAv8bnaOdu3TextChange);


// ODU 4
////////////////////
void onAv8bnaOdu4SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect4 = true;
    else
      oduselect4 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu4, oduselect4, 0);      
}
DcsBios::StringBuffer<1> av8bnaOdu4SelectBuffer(0x796a, onAv8bnaOdu4SelectChange);

void onAv8bnaOdu4TextChange(char* newValue) {

  setMyDisplayChars(newValue, oduselect4, 0);

  for (int i=0; i < 4; i++)
  {
    odu4[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu4TextBuffer(0x796c, onAv8bnaOdu4TextChange);


// ODU 5
////////////////////
void onAv8bnaOdu5SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect5 = true;
    else
      oduselect5 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu5, oduselect5, 1);      
}
DcsBios::StringBuffer<1> av8bnaOdu5SelectBuffer(0x7970, onAv8bnaOdu5SelectChange);

void onAv8bnaOdu5TextChange(char* newValue) {

  setMyDisplayChars(newValue, oduselect5, 1);

  for (int i=0; i < 4; i++)
  {
    odu5[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu5TextBuffer(0x7972, onAv8bnaOdu5TextChange);


// ODU
DcsBios::Switch2Pos oduOpt1("ODU_OPT1", ODU_PIN_OPT1);
DcsBios::Switch2Pos oduOpt2("ODU_OPT2", ODU_PIN_OPT2);
DcsBios::Switch2Pos oduOpt3("ODU_OPT3", ODU_PIN_OPT3);
DcsBios::Switch2Pos oduOpt4("ODU_OPT4", ODU_PIN_OPT4);
DcsBios::Switch2Pos oduOpt5("ODU_OPT5", ODU_PIN_OPT5);

void setup() {
  //Serial.begin(9600);

  // // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  // if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); // Don't proceed, loop forever
  // }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  tcaselect(0);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // drawSegColon(1,1,offsetSmall);
  drawComChar('O',10,1, offsetSmall);
  drawComChar('P',40,1, offsetSmall);
  drawComChar('E',70,1, offsetSmall);
  drawComChar('N',100,1, offsetSmall);
  display.display();
  delay(1000);

  tcaselect(1);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  drawSegColon(1,1,offsetSmall);
  drawComChar('H',10,1, offsetSmall);
  drawComChar('A',40,1, offsetSmall);
  drawComChar('R',70,1, offsetSmall);
  drawComChar('R',100,1, offsetSmall);
  display.display();
  delay(1000);


  tcaselect(2);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  drawSegColon(1,1,offsetSmall);
  drawComChar('I',10,1, offsetSmall);
  drawComChar('E',40,1, offsetSmall);
  drawComChar('R',70,1, offsetSmall);
  drawComChar('*',100,1, offsetSmall);
  display.display();
  delay(1000);

  tcaselect(3);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  drawSegColon(1,1,offsetSmall);
  drawComChar('3',10,1, offsetSmall);
  drawComChar('3',40,1, offsetSmall);
  drawComChar('3',70,1, offsetSmall);
  drawComChar('3',100,1, offsetSmall);
  display.display();
  delay(1000);

  tcaselect(4);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  drawSegColon(1,1,offsetSmall);
  drawComChar('4',10,1, offsetSmall);
  drawComChar('4',40,1, offsetSmall);
  drawComChar('4',70,1, offsetSmall);
  drawComChar('4',100,1, offsetSmall);
  display.display();
  delay(1000);


  tcaselect(0);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // drawSegColon(1,1,offsetSmall);
  drawComChar('O',10,1, offsetSmall);
  drawComChar('P',40,1, offsetSmall);
  drawComChar('E',70,1, offsetSmall);
  drawComChar('N',100,1, offsetSmall);
  display.display();
  delay(1000);
  
    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);

  DcsBios::setup();
}

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
      currx++;
      if (currx > startx+25) currx = startx;

      nextupdate = timestamp + 2000;
      wasAsleep = true;
      return;      
    }

    if (wasAsleep)
    {
      // Restore whatever was displayed before we pasued
      setMyDisplayChars(odu1, oduselect1, 4);
      delay(100);
      setMyDisplayChars(odu2, oduselect2, 3);
      delay(100);
      setMyDisplayChars(odu3, oduselect3, 2);
      delay(100);
      setMyDisplayChars(odu4, oduselect4, 0);
      delay(100);
      setMyDisplayChars(odu5, oduselect5, 1);
      delay(100);             
      //renderScratchPad(textScratchpad);
      //renderCom1(textCom1);
      //renderCom2(textCom2);


      wasAsleep = false;
    }
  }

}

void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
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

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}




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
  //display.drawLine(offx-1,  offy,   offx+1, offy,   SSD1306_WHITE);
  //display.drawLine(offx,   offy-1,  offx,   offy+1, SSD1306_WHITE);  

  display.drawLine(offx,  offy,   offx+1, offy,   SSD1306_WHITE);
  display.drawLine(offx,   offy+1,  offx+1,   offy+1, SSD1306_WHITE); 
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
 
//void testradio(short offx, short offy)
//{
//
//  //display.fillRect(0,0, 4,4, SSD1306_WHITE);
//
//  //display.fillRect(0,8, 4,4, SSD1306_WHITE);
//  short offset = PIXWIDTH+PIXSPACE;
//
//  drawSegHorizontal(offx+offset,  offy+0);              // A
//  drawSegHorizontal(offx+(offset*5), offy+0);  // B
//
//  drawSegVertical(offx+(offset*8), offy+offset);  // C
//  drawSegVertical(offx+(offset*8), offy+(offset*5)); // D
//
//  drawSegHorizontal(offx+(offset*5), offy+(offset*8));   // E
//  drawSegHorizontal(offx+offset,     offy+(offset*8));    // F
//  
//  drawSegVertical(offx+0,  offy+offset);   // H
//  drawSegVertical(offx+0,  offy+(offset*5));  // G
//
//  drawSegHorizontal(offx+offset,     offy+(offset*4));    // I
//  drawSegHorizontal(offx+(offset*5), offy+(offset*4));   // J
//
//
//  drawSegVertical(offx+(offset*4),  offy+offset);   // L
//  drawSegVertical(offx+(offset*4),  offy+(offset*5));  // O
//
//  drawSegM(offx,offy); // M
//  drawSegK(offx,offy); // K
//  drawSegN(offx,offy); // N
//  drawSegP(offx,offy); // P
//}

 
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

void drawSegments(String segments, short x, short y, short spacing)
{
  for (int i=0; i < 16; i++)
  {
    char c = segments.charAt(i);
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
    case 'O': drawSegments("ABCDEFGH\0",x,y,spacing); break;
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
    
  }
}





void setMyDisplayChars(char* fart, bool selected, int unit)
{
  //byte data[5];
    
  //int len = string.length();
  //if (len > 4) len = 4;

  //string.getBytes(data,5);


  tcaselect(unit);
  //display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  if (selected) drawSegColon(1,1,offsetSmall);    
  drawComChar(fart[0],10,1, offsetSmall);
  drawComChar(fart[1],40,1, offsetSmall);
  drawComChar(fart[2],70,1, offsetSmall);
  drawComChar(fart[3],100,1, offsetSmall);
  display.display();

/*
  bool fin = false;
  for (int j=0; j < 4; j++)
  {
    if (string[j] == 0x00)
    {
      fin = false;
    }

    if (fin)
    {
      // padd with spaces
      // module.setSegments16(0x00,j+(unit*4));


      drawSegColon(1,1,offsetSmall);
      drawComChar(newValue[0],10,1, offsetSmall);
      drawComChar(newValue[1],40,1, offsetSmall);
      drawComChar(newValue[2],70,1, offsetSmall);
      drawComChar(newValue[3],100,1, offsetSmall);
      display.display();
    }
    else
    {
      uint16_t value = HK4516_FONT[string[j]-32];
      if (selected)
        bitSet(value,1);
      else
        bitClear(value, 1);   

      module.setSegments16(value,j+(unit*4));
    }
    */
   
  }
