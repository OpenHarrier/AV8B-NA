/*
  Basic library example for TM1640. Kept small to fit in the limited space of an ATtiny44.

  Library based on TM1638 library by Ricardo Batista, adapted by Maxint-RD MMOLE 2018.
  Confirmed to work in the following environments:
      ATtiny44A using Arduino IDE 1.8.2 and ATTinyCore (8MHz, LTO enabled), 3232 bytes flash, 103 bytes RAM
      ATtiny44A using Arduino IDE 1.8.2 and ATtiny Microcontrolers (8MHz), 3212 bytes flash, 103 bytes RAM ==> 2892/95 ==> 2878/95

  Only compiled: not tested yet:
      Arduino Nano using Arduino IDE 1.8.2, Nano (Old Bootloader)), 3176 bytes flash, 95 bytes RAM

  For more information see  https://github.com/maxint-rd/TM16xx
*/
#define DCSBIOS_DEFAULT_SERIAL
#include <TM1640Anode.h>
#include <TM16xxMatrix.h>
#define MATRIX_NUMCOLUMNS 8
#define MATRIX_NUMROWS 16

#include "DcsBios.h"

#if !defined(LED_BUILTIN)
#define LED_BUILTIN 4
#endif

// Define a 4-digit display module. Pin suggestions:
// ESP8266 (Wemos D1): data pin 5 (D1), clock pin 4 (D2)
// ATtiny44A: data pin 9, clock pin 10 (LED_BUILTIN: 8 on ATTinyCore)
TM1640Anode module(9, 10, 4);    // data, clock, 4 digits
TM16xxMatrix matrix(&module, MATRIX_NUMCOLUMNS, MATRIX_NUMROWS);    // TM16xx object, columns, rows

    // PONM LKJI HGFE DCBA
    // 1111 1111 1111 1111  
uint16_t HK4516_FONT[] = {
  0x0000, // (32)  <space>
  0x0000, // (33)  !
  0x0000, // (34) "
  0x0000, // (35) #
  0x4BBB, // (36) $
  0b100100000100100, // (37)  %
  0b1101001011001, // (38)  &
  0b00000010, // (39) '
  0b00110000, // (40) (
  0b00000110, // (41) )
  0b111111101000000, // (42)  *
  0b010010101000000, // (43)  +
  0b100000000000000, // (44)  ,
  0x300, // (45) -  // 0000 0011 0000 0000
  0b10000000, // (46) .
  0x2400, // (47) / // 0010 0100 0000 0000
  0x24FF, // (48) 0
  0x040C, // (49) 1
  0x0377, // (50)  2
  0x023F, // (51)  3
  0x038C, // (52)  4
  0x03BB, // (53)  5
  0x03FB, // (54)  6
  0x2403, // (55) 7
  0x03FF, // (56)  8
  0x038F, // (57)  9
  0b10000000, // (58) :
  0b100000000000010, // (59)  ;
  0b1100000000000, // (60)  <
  0b101001000, // (61)  =
  0b100001000000000, // (62)  >
  0b10000110000011, // (63) ?
  0b100111011, // (64)  @
  0x3CF,  // (65) A
  0x4A3F, // (66) B
  0xF3,   // (67) C
  0x483F, // (68) D
  0x3F3,  // (69) E
  0x3C3,  // (70) F
  0x2FB,  // (71) G
  0x3CC,  // (72) H
  0x4833, // (73) I
  0x7C,   // (74) J
  0x85C0, // (75) K
  0xF0,   // (76) L
  0x14CC, // (77) M
  0x90CC, // (78) N
  0x00FF, // (79) O
  0x3C7,  // (80) P
  0x80FF, // (81) Q
  0x83C7, // (82) R
  0x03BB, // (83) S
  0x4803, // (84) T
  0x00FC, // (85) U
  0x24C0, // (86) V
  0xA0CC, // (87) W
  0xB400, // (88) X
  0x5400, // (89) Y
  0x2433, // (90) Z
  0b00111001, // (91) [
  0b1001000000000, // (92)  \ (this can't be the last char on a line, even in comment or it'll concat)
  0b00001111, // (93) ]
  0b101000000000000, // (94)  ^
  0b00001000, // (95) _
  0b1000000000, // (96) `
  0b101011111, // (97)  a
  0b101111100, // (98)  b
  0b101011000, // (99)  c
  0b101011110, // (100) d
  0b101111011, // (101) e
  0b01110001, // (102)  f
  0b101101111, // (103) g
  0b101110100, // (104) h
  0b00000100, // (105)  i
  0b00001110, // (106)  j
  0b100001110100, // (107)  k
  0b00110000, // (108)  l
  0b101000110110, // (109)  m
  0b10000100000100, // (110)  n
  0b101011100, // (111) o
  0b101110011, // (112) p
  0b101100111, // (113) q
  0b101010000, // (114) r
  0b101101101, // (115) s
  0b01111000, // (116)  t
  0b00011100, // (117)  u
  0b100000000010000, // (118) v
  0b101000000010100, // (119) w
  0b101101000000000, // (120) x
  0b100101000000000, // (121) y
  0b100100000001001, // (122) z
  0b100001001001001, // (123) {
  0b010010000000000, // (124) |
  0b1100100001001, // (125) }
  0b00000001, // (126)  ~
};

int interval = 250;
bool oduselect1 = false;
bool oduselect2 = false;
bool oduselect3 = false;
bool oduselect4 = false;
bool oduselect5 = false;
char odu1[5];
char odu2[5];
char *podu1 = odu1;
char *podu2 = odu2;

// ODU 1
////////////////////

void onAv8bnaOdu1SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect1 = true;
    else
      oduselect1 = false;
    //matrix.setPixel(0,1,oduselect1);
    //matrix.setPixel(1,1,oduselect1);
    setMyDisplayChars(odu1, oduselect1, 0);      
}
DcsBios::StringBuffer<1> av8bnaOdu1SelectBuffer(0x7966, onAv8bnaOdu1SelectChange);

void onAv8bnaOdu1TextChange(char* newValue) {
  setMyDisplayChars(newValue, oduselect1, 0);
  for (int i=0; i < 4; i++)
  {
    odu1[i] = newValue[i];
  }  
}
DcsBios::StringBuffer<4> av8bnaOdu1TextBuffer(0x7968, onAv8bnaOdu1TextChange);


// ODU 2
////////////////////

void onAv8bnaOdu2SelectChange(char* newValue) {
    if (newValue[0] == ':')
      oduselect2 = true;
    else
      oduselect2 = false;
    //matrix.setPixel(4,1,oduselect2);
    //matrix.setPixel(5,1,oduselect2);
    setMyDisplayChars(odu2, oduselect2, 1);
}
DcsBios::StringBuffer<1> av8bnaOdu2SelectBuffer(0x796c, onAv8bnaOdu2SelectChange);

void onAv8bnaOdu2TextChange(char* newValue) {
  setMyDisplayChars(newValue, oduselect2, 1);
  for (int i=0; i < 4; i++)
  {
    odu2[i] = newValue[i];
  }
}
DcsBios::StringBuffer<4> av8bnaOdu2TextBuffer(0x796e, onAv8bnaOdu2TextChange);

void setup()
{
  
  //Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  //module.setDisplayToString("HALO");
  //delay(500);                        // wait
  
  module.setupDisplay(true, 7);   // sometimes noise on the line may change the intensity level
  delay(200);
  module.clearDisplay();

  setMyString2("WAIT", false,  0);
  
  DcsBios::setup();
}

void loop()
{
  DcsBios::loop();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  
//  int nTime = ((millis() / 1000) / 60) * 100 + (millis() / 1000) % 60; // minutes+seconds as integer
//  module.setDisplayToDecNumber(nTime, _BV(4)); // display dot on digit 4
//  delay(500);                        // wait
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  module.setDisplayToDecNumber(nTime, _BV(3)); // display colon on digit 3
//  delay(500);                        // wait

//module.sendAsciiChar(1,'A',false);
//module.setSegments16(0xFFFF,0);
//delay(200);
//module.setSegments16(0xFFFF,1);
//delay(200);
//module.setSegments16(0xFF,2);  //last segment
//delay(200);
//module.setSegments16(0xFF00,3);
//delay(200);
//
//
//module.setSegments16(0xFF,0);
//delay(200);
//module.setSegments16(getCharValue('C'),1);
//delay(200);
//module.setSegments16(getCharValue(' '),2);  //last segment
//delay(200);
//module.setSegments16(getCharValue('B'),3);
//delay(200);

//setMyString2("NTEA");
//delay(2000);




//
//setMyString2("NATE", false,  0);
//setMyString2("IS  ", false, 1);
//delay(2000);
//
//setMyString2("COOL", false, 0);
//setMyString2("    ", false,  1);
//delay(2000);





//setMyString2("WYPT", true,  0);
//setMyString2("TGPT", false, 1);
//delay(2000);
/*
setMyString2("    ");
delay(interval);
setMyString2("   H");
delay(interval);
setMyString2("  HA");
delay(interval);
setMyString2(" HAR");
delay(interval);
setMyString2("HARR");
delay(interval);
setMyString2("ARRI");
delay(interval);
setMyString2("RRIE");
delay(interval);
setMyString2("RIER");
delay(interval);
setMyString2("IER ");
delay(interval);
setMyString2("ER  ");
delay(interval);
setMyString2("R   ");
delay(interval);
setMyString2("    ");
delay(interval);
*/
//module.setSegments(0xFF,0);
//delay(interval);
//module.sendAsciiChar(1,' ',false);

//module.clearDisplay();
//delay(200);
module.setupDisplay(true, 7);
//delay(200);
//module.setSegments(0x00,0);

}

void setMyString(String string)
{
  int len = string.length();
  for (int i=0; i < 4; i++)
  {
    char c = string.charAt(i);
    module.setSegments16(getCharValue(c),i);
    //delay(100);
  }
}

void setMyString2(String string, bool selected, byte unit)
{
  byte data[5];
    
  int len = string.length();
  if (len > 4) len = 4;

  string.getBytes(data,5);
  
  for (int i=0; i < len; i++)
  {
    uint16_t value = HK4516_FONT[data[i]-32];
    //Override B bit to make digit 5 flash
    if (selected)
      bitSet(value,1);
    else
      bitClear(value, 1);
    
    
    module.setSegments16(value,i+(unit*4));
    //delay(100);
    //module.setSegments16(value,i+4);
  }
//  flipflop = !flipflop;
}


void setMyDisplayChars(char* string, bool selected, byte unit)
{
  //byte data[5];
    
  //int len = string.length();
  //if (len > 4) len = 4;

  //string.getBytes(data,5);


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
       module.setSegments16(0x00,j+(unit*4));
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
   
  }

  
//  for (int i=0; i < len; i++)
//  {
//    uint16_t value = HK4516_FONT[data[i]-32];
//    //Override B bit to make digit 5 flash
//    if (selected)
//      bitSet(value,1);
//    else
//      bitClear(value, 1);
//    
//    
//    module.setSegments16(value,i+(unit*4));
//    //delay(100);
//    //module.setSegments16(value,i+4);
//  }
////  flipflop = !flipflop;
}



uint16_t getCharValue(char c)
{
  uint16_t value = 0x00;
  switch (c)
  {
    // PONM LKJI HGFE DCBA
    // 1111 1111 1111 1111             PONM LKJI HGFE DCBA
    case 'A': value = 0x3CF; break;  //0000 0011 1100 1111
    case 'B': value = 0x4A3F; break; //0100 1010 0011 1111
    case 'C': value = 0xF3;   break; //          1111 0011
    case 'D': value = 0x483F; break; //0100 1000 0011 1111
    case 'E': value = 0x3F3; break;  //       11 1111 0011
    case 'F': value = 0x3C3; break;  //       11 1100 0011
    case 'G': value = 0x2FB; break;  //       10 1111 1011
    case 'H': value = 0x3CC; break;  //       11 1100 1100
    case 'I': value = 0x4833; break; //0100 1000 0011 0011
    case 'J': value = 0x7C; break;   //          0111 1100    
    case 'K': value = 0x85C0; break; //1000 0101 1100 0000   
    case 'L': value = 0xF0; break;   //          1111 0000
    case 'M': value = 0x14CC; break; //0001 0100 1100 1100
    case 'N': value = 0x90CC; break; //1001 0000 1100 1100
    case 'O': value = 0x00FF; break; //          1111 1111 
    case 'P': value = 0x3C7; break;  //     0011 1100 0111
    case 'Q': value = 0x80FF; break; //1000 0000 1111 1111
    case 'R': value = 0x83C7; break; //1000 0011 1100 0111
    case 'S': value = 0x03BB; break; //       11 1011 1011
    case 'T': value = 0x4803; break; //0100 1000 0000 0011
    case 'U': value = 0x00FC; break; //          1111 1100
    case 'V': value = 0x24C0; break; //0010 0100 1100 0000
    case 'W': value = 0xA0CC; break; //1010 0000 1100 1100
    case 'X': value = 0xB400; break; //1011 0100 0000 0000
    case 'Y': value = 0x5400; break; //0101 0100 0000 0000
    case 'Z': value = 0x2433; break; //0010 0100 0011 0011

    case '0': value = 0x24FF; break; //0010 0100 1111 1111
    case '1': value = 0x040C; break; //     0100 0000 1100
    case '2': value = 0x0377; break; //     0011 0111 0111
    case '3': value = 0x023F; break; //     0010 0011 1111
    case '4': value = 0x038C; break; //     0011 1000 1100
    case '5': value = 0x03BB; break; //     0011 1011 1011
    case '6': value = 0x03FB; break; //     0011 1111 1011
    case '7': value = 0x2403; break; //0010 0100 0000 0011
    case '8': value = 0x03FF; break; //     0011 1111 1111
    case '9': value = 0x038F; break; //     0011 1000 1111

    case ' ': value = 0x0000; break; //0000 0000 0000 0000
    case '$': value = 0x4BBB; break; //0100 1011 1011 1011
  }

  return value;
}
