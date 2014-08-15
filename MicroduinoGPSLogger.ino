#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#undef PGM_P
#define PGM_P prog_char *

#include "define.h"
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
#include "config.h"
#include <SimpleTimer.h>
#include <GCS_MAVLink.h>
#include "GPS.h"
#include "GPS_Vars.h"
#include "MAVLink_Vars.h"
#include "OLED_Vars.h"
#include "Global_Vars.h"

#define TELEMETRY_SPEED  57600  // How fast our MAVLink telemetry is coming to Serial port
#define GPS_SPEED  38400 

FastSerialPort0(Serial);
FastSerialPort1(Serial1);

GPS gps;

SimpleTimer  acm_timer;

void setup()
{
  Serial.begin(TELEMETRY_SPEED);
  Serial1.begin(GPS_SPEED);
  
  // setup mavlink port
  mavlink_comm_0_port = &Serial;  

  i2c_init();
  delay(100);
  // LCD
  initLCD();
  
  // Startup MAVLink timers  
  acm_timer.Set(&OnACMTimer, 100);
  acm_timer.Enable();
}

void loop()
{
  gps_databegin();
  gps_dataread();    
  
  if ( b_debug_serail )
    gps_serial();
  
  if(enable_mav_request == 1){//Request rate control
      for(int n = 0; n < 3; n++){
          request_mavlink_rates();//Three times to certify it will be readed
          delay(50);
      }
      enable_mav_request = 0;
      delay(2000);
      waitingMAVBeats = 0;
      lastMAVBeat = millis();//Preventing error from delay sensing
  }  
   
  read_mavlink();
  loopLCD();
  acm_timer.Run();
}


void OnACMTimer()
{
  if ( acm_timer_fmt )
    acm_timer_fmt(0);
}

