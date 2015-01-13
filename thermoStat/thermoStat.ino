
// ATtiny85 + Nokia 5110 LCD + 3x DS18B20 sensors 
// Binary sketch size: 7,476 bytes (of a 8,192 byte maximum)
//
// Pin connections:
//
// The LCD module is connected to the following 
// pins (using a levelshifter to get the correct 
// voltage to the module, its 3v!)
//
// LCD Clk    >  D0    LCD Din    >  D1
// LCD DC     >  D2    LCD CE     >  GND
// LCD RST    >  D3
// To save one pin the CS pin of LCD is connected to GND 
//
// 1-Wire bus >  D4
// pin D4 is used for the OneWire bus.
//
// The only free pin is the RESET pin.
// This pin could be used as D5 but with limited load.
//

// not enough memory for dallas libs, so basic onewire it is
#include <OneWire.h>


// For the 5110 LCD screen, use the basic library from http://www.henningkarlsen.com/electronics/library.php?id=44
#include <LCD5110_Basic.h>

//                     __________ SCK
//                    /  ________ MOSI
//                   /  /  ______ DC (register select)
//                  /  /  /  ____ RST
//                 /  /  /  /  __ CS
//                /  /  /  /  /
  LCD5110 myGLCD(0, 1, 2, 3, 6); //D6 don't exist - we conected CS to GND, but need to fool lib.

extern uint8_t MediumNumbers[]; //font that we use - 12x16 pixels, 13 chars

OneWire  ds(4);  // 1-wire bus D4

// change the following hex addr to match your sensors!

// use the "Arduino 1-Wire Address Finder" sketch (found in many sites) to discover your device address.

byte addr1[8]={0x28, 0xEC, 0xAF, 0x5B, 0x05, 0x00, 0x00, 0x6F}; //sensor #1
byte addr2[8]={0x28, 0x6D, 0xF0, 0x3B, 0x05, 0x00, 0x00, 0xD8}; //sensor #2
byte addr3[8]={0x28, 0x27, 0xA0, 0x5B, 0x05, 0x00, 0x00, 0x67}; //sensor #3
float temp;

void setup(void) {
   myGLCD.InitLCD(); // (xx)contrast value 70 default
   myGLCD.clrScr();
}

void loop(void) {
  myGLCD.setFont(MediumNumbers);
  byte i;
  byte present = 0;
  byte data[12];
  
 myGLCD.print("1.", LEFT, 0);
 myGLCD.print("2.", LEFT, 16);
 myGLCD.print("3.",LEFT, 32);

  ds.reset();
  ds.select(addr1); // sensor #1
  ds.write(0x44,0); //start conversion, NO parasite power
  delay(750); 
  present = ds.reset();
  ds.select(addr1);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++){
       data[i] = ds.read();
  }
  // convert the data to actual temperature
 temp = ( (data[1] << 8) + data[0] )*0.0625;
 myGLCD.printNumF(temp, 2, RIGHT, 0);
 
// I know doing three time the same is NOT a clever thing!!
// sketch need to optimize

 ds.reset();
  ds.select(addr2); // sensor #2
  ds.write(0x44,0); 
  delay(750);     
  present = ds.reset();
  ds.select(addr2);    
  ds.write(0xBE);         
  for ( i = 0; i < 9; i++) 
  {           
    data[i] = ds.read();
  }
  temp = ( (data[1] << 8) + data[0] )*0.0625;
  myGLCD.printNumF(temp, 2, RIGHT, 16); 

 ds.reset();
  ds.select(addr3); // sensor #3
  ds.write(0x44); 
  delay(750);     
  present = ds.reset();
  ds.select(addr3);    
  ds.write(0xBE);         
  for ( i = 0; i < 9; i++) 
  {           // we need 9 bytes
    data[i] = ds.read();
  }
  temp = ( (data[1] << 8) + data[0] )*0.0625;
  myGLCD.printNumF(temp, 2, RIGHT, 32);  
}