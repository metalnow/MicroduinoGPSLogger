#undef PROGMEM 
#define PROGMEM __attribute__(( section(".progmem.data") )) 

#undef PSTR 
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];})) 

#include "define.h"

#include <FastSerial.h>
#include <EEPROM.h>
#include <SimpleTimer.h>
#include <GCS_MAVLink.h>
#include "GPS.h"
#include "GPS_Vars.h"

#define TELEMETRY_SPEED  57600  // How fast our MAVLink telemetry is coming to Serial port
#define GPS_SPEED  38400 

FastSerialPort0(Serial);
FastSerialPort1(Serial1);

GPS gps;

SimpleTimer  timer;

void setup()
{
  Serial.begin(TELEMETRY_SPEED);
  Serial1.begin(GPS_SPEED);
  
  // setup mavlink port
  mavlink_comm_0_port = &Serial;  
  
  // Startup MAVLink timers  
  timer.Set(&OnTimer, 120);
  timer.Enable();
}

void loop()
{
  gps_databegin();
  gps_dataread();
  gps_serial();
  
  timer.Run();
}


void OnTimer()
{
  
}

