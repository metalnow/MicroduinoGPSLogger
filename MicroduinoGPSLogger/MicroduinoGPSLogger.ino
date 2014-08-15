#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#undef PGM_P
#define PGM_P prog_char *


#include <FastSerial.h>
#include <math.h>
#include <inttypes.h>
#include <avr/pgmspace.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "wiring.h"
#endif
#include <EEPROM.h>

#include "define.h"
#include "vars.h"

FastSerialPort0(Serial);

void setup()
{
  Serial.begin(9600);
  Serial.println("start...");
}

void loop()
{
  uiKeyCode = KEY_NONE;
  if ( Serial.available() > 0)
  {
    uint8_t c = Serial.read();
    if ( c == 'a' )
      uiKeyCode = KEY_LEFT;
    else if ( c == 'd' )
      uiKeyCode = KEY_RIGHT;
    else if ( c == 'w' )
      uiKeyCode = KEY_UP;
    else if ( c == 'x' )
      uiKeyCode = KEY_DOWN;
    else if ( c == 's' )
      uiKeyCode = KEY_SELECT;    
    else if ( c == 'j' )
      test();        
  }
  updateMenu();
//  Serial.println(uiKeyCode);
//  delay(3000);
  if ( refreshLCD == 1 )
  {
    refreshScreen();
    refreshLCD = 0;
    Serial.println("refresh Screen");
  }
  
}




