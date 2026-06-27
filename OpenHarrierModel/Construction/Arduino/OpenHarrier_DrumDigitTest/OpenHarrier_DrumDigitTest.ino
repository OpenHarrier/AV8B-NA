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

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

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
char odu1[5];
char odu2[5];
char odu3[5];
char odu4[5];
char odu5[5];
char *podu1 = odu1;
char *podu2 = odu2;
char *podu3 = odu3;
char *podu4 = odu4;
char *podu5 = odu5;















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


  display.setFont(&FreeSans12pt7b);

  delay(1000);

  // Clear the buffer
  for (int i=0; i < 5; i++)
  {
    tcaselect(i);
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
  }


  tcaselect(0);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  //drawSegColon(1,1,offsetSmall);
  //drawComChar('G',10,1, offsetSmall);
  //drawComChar('E',40,1, offsetSmall);
  //drawComChar('N',70,1, offsetSmall);
  //drawComChar('*',100,1, offsetSmall);

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("8"));
  display.display();

  tcaselect(1);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // drawSegColon(1,1,offsetSmall);
   drawComChar('N',10,1, offsetSmall);
  // drawComChar('A',40,1, offsetSmall);
  // drawComChar('*',70,1, offsetSmall);
  // drawComChar('1',100,1, offsetSmall);
  display.display();

  tcaselect(2);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // drawSegColon(1,1,offsetSmall);
  // drawComChar('G',10,1, offsetSmall);
  // drawComChar('R',40,1, offsetSmall);
  // drawComChar('A',70,1, offsetSmall);
  // drawComChar('8',100,1, offsetSmall);
   display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("yes!");
  display.display();

  tcaselect(3);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  // drawSegColon(1,1,offsetSmall);
  // drawComChar('T',10,1, offsetSmall);
  // drawComChar('H',40,1, offsetSmall);
  // drawComChar('E',70,1, offsetSmall);
  // drawComChar('*',100,1, offsetSmall);
  display.display();

  tcaselect(4);
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  //drawSegColon(1,1,offsetSmall);
  //drawComChar('N',10,1, offsetSmall);
  //drawComChar('A',40,1, offsetSmall);
  //drawComChar('T',70,1, offsetSmall);
  //drawComChar('E',100,1, offsetSmall);

  display.display();



  //DcsBios::setup();


  tcaselect(0);
  //display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
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


void loop() {
  int n = counter;
  
  // Step1:  Recompute target drum positions
  for (int i=0; i < 4; i++)
  {
    
    targetvalue[i][4] = n % 10;
    n /= 10;

    targetvalue[i][3] = n % 10;
    n /= 10;

    targetvalue[i][2] = n % 10;
    n /= 10;

    targetvalue[i][1] = n % 10;
    n /= 10;

    targetvalue[i][0] = n % 10;


    tcaselect(i);
    //display.clearDisplay();

    for (int j=0; j<5; j++)
    {
      int valmid = targetvalue[i][j];
      
      

      if (valmid != prevtarget[i][j])
      {
        int valtop = valmid - 1;
        if (valtop < 0) valtop = 9;

        int valbot = valmid + 1;
        if (valbot > 9) valbot = 0;
        
        display.fillRect(j*28, 0, 12, 32, 0); // clear column
        display.setCursor(j*28, 3);
        display.print(valtop);
        display.setCursor(j*28, 24);
        display.print(valmid);
        display.setCursor(j*28, 45);
        display.print(valbot);        
      }
      prevtarget[i][j] = valmid;

    }
    display.display();
      


  }
  counter += 1;

  if (counter > 9999) counter = 0;
}


void loopold() {
  //DcsBios::loop();
  starttime = millis();
  int basespeed = 32;
  int speed = basespeed;

  // Recompute values
  int n = counter;
  

  
  


  // Step1:  Recompute target drum positions
  for (int j=0; j < 4; j++)
  {
    n = counter+(j*100);

    targetvalue[j][3] = n % 10;
    n /= 10;

    targetvalue[j][2] = n % 10;
    n /= 10;

    targetvalue[j][1] = n % 10;
    n /= 10;

    targetvalue[j][0] = constrain(n % 10,0,9);


    for (int i=0; i < 4; i++)
    {
        //targetvalue[j][i] = random(0,9);
        updateDrumPosition(prevtarget[j][i], targetvalue[j][i], &curdrumpos[j][i], 0, 9);
    }    
  }


  // Update MODE
  //updateDrumPosition(prevmodecounter, modecounter, &modeline, 0, 9);

  for (int i=0; i < 4; i++)
  {
    tcaselect(i);
    //display.setRotation(0);  Does not seem to be supported on SSD1306 displays
    display.clearDisplay();
   

    switch (i)
    {
      case 0:
      case 1:
      case 2:
      case 3:
        
        display.setTextSize(1); // Draw 2X-scale text
        //updateDrum(0, curdrumpos[i][0]);
        //updateDrum(30, curdrumpos[i][1]);
        //updateDrum(60, curdrumpos[i][2]);
        //updateDrum(90, curdrumpos[i][3]);

        for (int j=0;j<4;j++)
        {
          int valmid = targetvalue[i][j];
          int valtop = valmid - 1;
          if (valtop < 0) valtop = 9;
          int valbot = valmid + 1;
          if (valbot > 9) valbot = 0;

          display.setCursor(j*31, 5);
          display.print(valtop);
          display.setCursor(j*31, 26);
          display.print(valmid);
          display.setCursor(j*31, 45);
          display.print(valbot);
        }





      break;

      case 4:
        display.setTextSize(1); // Draw 2X-scale text
        renderModeDrum(30, modeline);
        break;
    }

    display.display();

  }

  //updateyval(&drumval, 1);
  //updateyval(&drumval2, 2);
  //updateyval(&drumval3, 3);
  //updateyval(&drumval4, 4);

  // tcaselect(4);
  // display.clearDisplay();
  // display.setCursor(0, 0);
  // display.setTextSize(1); // Draw 2X-scale text
  // if (!flipflop)
  //   display.print("m0");
  // else
  //   display.print("m1");
  // display.setCursor(30, 0);
  
      counter += 1;
      if (counter > 2000) counter = 0;

  //delay(10);

  if (millis() > nextflop) 
  {
    flipflop = !flipflop;
    nextflop = millis() + modeinterval;
    if (flipflop == false)
    {
      numdisplays += 1;
      if (numdisplays > 4) numdisplays = 1;

      //targetvalue[0] = counter;

      prevmodecounter = modecounter;
      modecounter +=1; 
      if (modecounter > 4) modecounter = 0;
      

      // Copy old values for comparisons
      for (int j=0; j<4; j++)
      {
        for (int i=0; i<4; i++)
        {
          prevtarget[j][i] = targetvalue[j][i];
        }
      }

    }
  }

    float fps = (float)(1000.0f / (millis() - starttime));
  // display.print(fps);
  // display.setCursor(0, 12);
  // display.print(targetvalue[0]);
  // //display.setCursor(0, 24);
  // //display.print(targetdrumpos[0]);
  //   display.setCursor(60, 24);
  // display.print(counter);
  // display.display();

}


void updateDrumPosition(int prevtargetrow, int newtargetrow, int *linepos, int charheight, int maxrow)
{
  int stepslow = 0;
  int stepshigh = 0;
  int basespeed = 48;  //16
  int speed = 1;
  int targetlinepos=0;

  // Step 1:  Figure out whether it is shorter to rotate drum upwards or downwards to get to our target poistion.

  int currstep = prevtargetrow;
  while (currstep != newtargetrow)
  {
    currstep--;
    if (currstep < 0) currstep = maxrow;
    stepslow +=1;
  }

  currstep = prevtargetrow;
  while (currstep != newtargetrow)
  {
    currstep++;
    if (currstep > maxrow) currstep = 0;
    stepshigh += 1;
  }
    

    speed = basespeed;
    targetlinepos = (newtargetrow * -drumoffset) + drumadjust;

    // determine whether going up or down is shortest path.
    // count going backwards
    int startpos = *linepos;
    int countpos = 0;

    if (*linepos < targetlinepos)
    {
      speed = constrain(targetlinepos - *linepos, 1, 12);
    }
    else if (*linepos > targetlinepos)
    {
      speed = constrain(*linepos - targetlinepos, 1, 12);
    }
    else
    {
      speed = 0;
    }

    if (linepos != targetlinepos)
    {
      if (stepslow > stepshigh)
      {
        *linepos -= speed;
        if (*linepos < mindrum) *linepos = maxdrum;
      }
      else if (stepslow < stepshigh)
      {
        *linepos += speed; 
        if (*linepos > maxdrum) *linepos = mindrum;
      }  
    }

}


void updateDrum(int x, int yval) {
  
 
  //drawSegColon(1,1,offsetSmall);
  //drawComChar('G',10,1, offsetSmall);
  //drawComChar('E',40,1, offsetSmall);
  //drawComChar('N',70,1, offsetSmall);
  //drawComChar('*',100,1, offsetSmall);


  // display.setCursor(x, drumval);
  // display.print("0");
  // display.setCursor(x, drumval+(drumoffset));
  // display.print("1");
  // display.setCursor(x, drumval+(drumoffset*2));
  // display.print("2");
  // display.setCursor(x, drumval+(drumoffset*3));
  // display.print("3");




// NEW WAY


// OLD WAY
/*

  display.setCursor(x, yval+(drumoffset*-1));
  display.print("9");

  display.fillRect(x, 0, 12, 32, 0);
  for (int i=0; i <= 10; i++)
  {
    //display.setCursor(x, *yval+(drumoffset*i));
    int y = yval+(drumoffset*i);
    if (y >= -31 && y < 62)
    {
      // drawbox to clear data
      
      display.setCursor(x, yval+(drumoffset*i));
      if (i == 10) 
        display.print("0");
      else
        display.print(i);      
    }


  }
*/

  
 
}


void renderModeDrum(int x, int yval) {
  
  //display.setCursor(x, yval+(drumoffset*-1));
  //display.print("9");

  for (int i=0; i <= 4; i++)
  {
    //display.setCursor(x, *yval+(drumoffset*i));
    display.setCursor(x, yval+(drumoffset*i));
    if (i == 10) 
      display.print("---");
    else
      //display.print(i);
      switch(i)
      {
        case 0:
          display.print("AUT");
          break;

        case 1:
          display.print("CIP");
          break;
        
        case 2:
          display.print("DSL");
          break;

        case 3:
          display.print("DIR");
          break;

        default:
          display.print("---");
      }

  }

}

void updateyval(int *yval, int speed)
{
   *yval += speed;
  if (*yval > 31) *yval = (drumoffset * -9)+5;

}


void method2(int x, int *yval)
{
    display.setCursor(x, *yval+(drumoffset*-1));
    display.print("9");

    //int y = *yval+(drumoffset);

    // We know that there are only 32 lines that can be shown.  So we need to figure out how to suppress the characters that shouldn't be shown.
    // Basically we should only be able to show no more than 3 characters at a time.

    // Window is from 0 to 31.

    for (int i=0; i < 10; i++)
    {
      // If partially or wholly within view window!
      int y = *yval+(drumoffset*i);
      if (y >= -31 && y < 62)
      {
        display.setCursor(x, y);
        display.print(i);

        //display.drawBitmap(x, y, logo_bmp, 16, 16, SSD1306_WHITE);
      }
 
    }
}







void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2




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
