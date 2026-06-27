/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.14" TFT breakout
  ----> https://www.adafruit.com/product/4383
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 1.69" TFT breakout
    ----> https://www.adafruit.com/product/5206
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#define DCSBIOS_DEFAULT_SERIAL
//#define DCSBIOS_IRQ_SERIAL
#include <DcsBios.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Fonts/rcu8150_9pt7b.h>
#include <SPI.h>



#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
  #define TFT_CS         14
  #define TFT_RST        15
  #define TFT_DC         32

#elif defined(ESP8266)
  #define TFT_CS         4
  #define TFT_RST        16                                            
  #define TFT_DC         5

#else
  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8
  // I swapped 8 and 9  (originally rst was 9 and dc was 8)
#endif


Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);


// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
#define TFT_MOSI 11  // Data out
#define TFT_SCLK 13  // Clock out


float p = 3.1415926;

int Freq1w = 146;
int Freq1h = 22;
GFXcanvas1 canvasChan(54, 33);
GFXcanvas1 canvasFreq1(Freq1w, Freq1h);


#define PIN_BTN1  4
#define PIN_BTN2  3
#define PIN_BTN3  5

#define PIN_FLOODTEST A0

int fadeValue = 255;

int state = 0;

int countchan = 0;
int countfreq = 0;
char text[5]; // Buffer for a 4-digit number + null terminator
char freqtext[8];
char prevfreq[8];
char currfreq[8];
char currclock[8];
char currchan[3];

void setup(void) {
  
  Serial.dtr(); // wait until serial port becomes ready?

 
  tft.init(76,284);  //SPI_MODE3
//SPI.setClockDivider(SPI_CLOCK_DIV4);
//SPI.setClockDivider(SPI_CLOCK_DIV16); 

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(40000000);

  
  tft.invertDisplay(false); // gotta invert first.



  tft.setFont(&rcu8150_9pt7b);
  canvasChan.setFont(&rcu8150_9pt7b);
  canvasFreq1.setFont(&rcu8150_9pt7b);
  tftPrintTest();


  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);

  sprintf(freqtext,"000.000");
  sprintf(currclock, " 12:59A");
  sprintf(currchan,"88");
  renderChan(currchan);

  //pinMode(A0, INPUT); 
  //pinMode(A1, INPUT); 


  DcsBios::setup();

}




long nextFreqRender = 0;
bool freqDirty = true;

bool fastFlipflop = false;
bool faultFlipflop = true;

long nextfastblink = 0;
long nextfaultblink = 100;

int underline = 0;
long nextunderline = 0;

int menuopt = 0;
long nextmenuopt = 0;

long btn1last = 0;
int prevbtn1 = HIGH;

long nextfade = 0;

void loop() {
  DcsBios::loop();

  int btnval = digitalRead(PIN_BTN1);
  if (btnval == LOW && millis() > btn1last) // pressed
  {
   
      state += 1;
      if (state > 10) state = 0;
      btn1last = millis() + 250;      
      updateTime();  
      tft.fillRect(78, 4, 148, 27, ST77XX_BLACK);
      renderMenu(state);
      renderChan(currchan);
   
  }
  prevbtn1 = btnval;

  if (freqDirty && millis() > nextFreqRender)
  {
    renderFreq(freqtext);
    freqDirty = false;
    nextFreqRender = millis() + 0; // milliseconds
  }  
  


  if (millis() > nextfastblink)
  {
    fastFlipflop = !fastFlipflop;
    renderFast(fastFlipflop);
    nextfastblink = millis() + 250;
  }

  if (millis() > nextfaultblink)
  {
    faultFlipflop = !faultFlipflop;
    renderFault(faultFlipflop);
    nextfaultblink = millis() + 500;
  }

  if (millis() > nextunderline)
  {
    renderUnderscore(underline);
    underline += 1;
    if (underline > 7) underline = 0;
    nextunderline = millis() + 350;
  }


  // if (millis() > nextmenuopt)
  // {
  //   renderMenu(menuopt);
  //   menuopt += 1;
  //   if (menuopt > 9) menuopt = 0;
  //   nextmenuopt = millis() + 250;
  // }

  if (millis() > nextfade)
  {
    analogWrite(PIN_FLOODTEST, fadeValue);
    fadeValue -= 1;
    if (fadeValue == 80) fadeValue = 255;
    nextfade = millis() + 20;
  }

}



void onUhfChannelChange(char* newValue)
{
  for (int i=0; i < 3; i++)
  {
    currchan[i] = newValue[i];
  }
  renderChan(newValue);
  return;


}DcsBios::StringBuffer<2> uhfChannelBuffer(0x7992, onUhfChannelChange);



//typedef DcsBios::RotaryEncoderT<POLL_EVERY_TIME, DcsBios::FOUR_STEPS_PER_DETENT> CusomRotaryEncoder; // A custom rotary encoder with four quadrature steps per physical detent.
//CusomRotaryEncoder ufcCom1Sel("UFC_COM1_SEL", "-3200", "+3200", 5, 6);


void renderChan(char * newValue)
{

  canvasChan.fillScreen(0);

  if (state == 0)
  {
    canvasChan.setTextColor(1);
    canvasChan.setCursor(-6, 30);
    canvasChan.setTextSize(3);
    canvasChan.print(currchan);
  }

  tft.drawBitmap(16, 7, canvasChan.getBuffer(), 54, 33, ST77XX_GREEN, ST77XX_BLACK); 
}

void renderClock()
{
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(76, 26);
  tft.print(prevfreq);

  // Calculate new text
  //sprintf(freqtext,"%c%c%c.%c%c%c", newValue[0], newValue[1], newValue[2], newValue[4], newValue[5], newValue[6] );
  sprintf(freqtext,"%c%c%c:%c%c%c",  ' ', '1', '2', '0', '0','A');

  // print new text
  tft.setCursor(76, 26);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(freqtext);

}

void renderFreq(char* newValue)
{

  tft.setTextSize(2);

  // print prev text in black.
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(76, 26);
  tft.print(prevfreq);

  // Calculate new text
  //sprintf(freqtext,"%c%c%c.%c%c%c", newValue[0], newValue[1], newValue[2], newValue[4], newValue[5], newValue[6] );

  // print new text
  tft.setCursor(76, 26);
  switch(state)
  {
    case 0: 
      //sprintf(freqtext,"%c%c%c.%c%c%c", freqtext[0], freqtext[1], freqtext[2], newValue[4], newValue[5], newValue[6] );
      tft.setTextColor(ST77XX_GREEN);
      tft.print(freqtext);
      break;

    case 1:
      tft.setTextColor(ST77XX_YELLOW);
      tft.print(currclock);
      break;

    default:
      tft.setTextColor(ST77XX_BLUE);
      tft.print(" NO OP ");
      //         000.000
      break;
  }
 
  
  

  // canvasFreq1.fillScreen(0);
  // canvasFreq1.setTextColor(1);
  // canvasFreq1.setCursor(-4, 20);
  // canvasFreq1.setTextSize(2);
  // canvasFreq1.print(newValue);

  // tft.drawBitmap(78, 8, canvasFreq1.getBuffer(), Freq1w, Freq1h, ST77XX_WHITE, ST77XX_RED); 
  
}
void updateTime()
{
  // clearblock
 
  //sprintf(currclock, "%c%c%c:%c%c%c", '0','1','2','5','9','A');
  sprintf(currclock, "23:5900");
  freqDirty = true;
}


void onUhfFrequencyChange(char* newValue) {
  // Calculate new text
  for (int i=0; i < 8; i++)
  {
    prevfreq[i] = freqtext[i];
    currfreq[i] = newValue[i];
  }

  //if (state == 0)
  //{
     sprintf(freqtext,"%c%c%c.%c%c%c", currfreq[0], currfreq[1], currfreq[2], currfreq[4], currfreq[5], currfreq[6] );
  //}
  //else
  //{
  //  sprintf(freqtext, "%c%c%c:%c%c%c",  currclock[0], currclock[1], currclock[2], currclock[4], currclock[5], currclock[6]);
  //}
 
  freqDirty = true;
  //renderFreq(newValue);

  return;

  sprintf(text,"%c%c%c", newValue[4], newValue[5], newValue[6] );

  canvasFreq1.fillScreen(0);
  canvasFreq1.setTextColor(1);
  canvasFreq1.setCursor(-4, 20);
  canvasFreq1.setTextSize(2);
  canvasFreq1.print(newValue);

  tft.drawBitmap(78, 10, canvasFreq1.getBuffer(), Freq1w, Freq1h, ST77XX_WHITE, ST77XX_RED); 



  /*


  sprintf(text,"%c%c%c", newValue[0], newValue[1], newValue[2] );
  tft.setCursor(75, 30);
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print(text);
  */

  //sprintf(text,"%c%c%c", newValue[4], newValue[5], newValue[6] );
  tft.setCursor(157, 30);
  tft.print(text);

  

} DcsBios::StringBuffer<8> uhfFrequencyBuffer(0x7994, onUhfFrequencyChange);

void renderFault(bool value)
{
  if (value)
  {
    tft.setTextColor(ST77XX_RED);
  }
  else
  {
    tft.setTextColor(ST77XX_BLACK);
  }
  tft.setCursor(235, 35);
  tft.setTextSize(1);
  tft.println("FAULT");
}

void renderFast(bool value)
{
  if (value)
  {
    tft.setTextColor(ST77XX_YELLOW);
  }
  else
  {
    tft.setTextColor(ST77XX_BLACK);
  }
  tft.setCursor(235, 18);
  tft.setTextSize(1);
  tft.println("FAST");

}

void renderUnderscore(int value)
{
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(76, 28);
  for (int i=0; i < 7; i++)
  {
    if (i == value)
    {
      tft.setTextColor(ST77XX_YELLOW);
    }
    else
    {
      tft.setTextColor(ST77XX_BLACK);
    }
    tft.print("_");
  }
  //tft.print("_______");
}

void renderMenu(int value)
{
  char option[10];
  for (int i=1; i < 11; i++)
  {
    if (i == value)
    {
      option[i-1] = '>';
    }
    else
    {
      option[i-1] = ' ';
    }
  }

  
  tft.setTextSize(1);

  char menuline[29];

  if (state != 0)
  {
      // clear first line pointers
    tft.setCursor(8, 55);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(">     >     >    >     >    ");

    // render first line
    tft.setCursor(8, 55);
    tft.setTextColor(ST77XX_WHITE);
    sprintf(menuline, "%cTIME %cAM   %cH   %cSND  %cLE", option[0], option[2], option[4], option[6], option[8]);
    tft.print(menuline);

    // clear second line pointers
    tft.setCursor(8, 70);
    tft.setTextColor(ST77XX_BLACK);
    tft.print(">     >     >    >     >    ");
    tft.setCursor(8, 70);

    // render second line
    tft.setTextColor(ST77XX_WHITE);
    sprintf(menuline, "%cGPS  %cPM   %cL   %cRCV  %cOFST", option[1], option[3], option[5], option[7], option[9]);
    tft.print(menuline);
  }
  else
  {
    // Just clear this area for now.
    tft.fillRect(0, 40, 283, 55, ST77XX_BLACK);
  }


  


}



void speedtest()
{
  sprintf(text, "%02d", countchan);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setCursor(10, 5);
  tft.setTextSize(5);
  tft.print(text);
  countchan += 1;
  if (countchan > 30) countchan = 0;

  sprintf(text, "%03d", countfreq);
  tft.setCursor(70, 5);
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.setTextSize(4);
  tft.print(countfreq);

  tft.setCursor(152, 5);
  tft.print(countfreq);

  countfreq += 111;
  if (countfreq > 999) countfreq = 0;
}




void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(3);

  // SHOW
  renderFast(true);
  renderFault(true);



  // // Big double digits (CHAN)
  // tft.setTextColor(ST77XX_GREEN);
  // tft.setCursor(10, 40);
  // tft.setTextSize(3);
  // tft.println("88");

  renderChan("88");

  renderFreq("000.000");

  // // Small digits
  // tft.setCursor(75, 30);
  // tft.setTextColor(ST77XX_BLUE);
  // tft.setTextSize(2);
  // tft.print("888");

  // tft.setCursor(138, 30);
  // tft.print(".");

  // tft.setCursor(157, 30);
  // tft.print("888");

  // underline 
  tft.setCursor(76, 28);
  tft.print("_______");

  //tft.setCursor(157, 32);
  //tft.print("___");


  // Modes
  renderMenu(0);
  // tft.setCursor(8, 55);
  // tft.setTextColor(ST77XX_WHITE);
  // tft.setTextSize(1);
  // tft.println(">TIME >AM   >H   >SND  >LE");
  // tft.setCursor(8, 70);
  // tft.println(">GPS  >PM   >L   >RCV  >OFST");


}


