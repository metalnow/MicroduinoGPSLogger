#include "define.h"

#include <EEPROM.h>
boolean b_write_sdcard = false;
boolean b_read_gps = false;
boolean b_debug_serail = false;
#define CFG_EEPROM_ADDR 100;
//oled=======================================
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//-------Font size: large, midium, small
#define setFont_L u8g.setFont(u8g_font_courB14)    // 11 pixel height,Font Bounding box     w=17 h=26 x=-4 y=-7
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)  // 7 pixel height, Font Bounding box     w= 7 h= 9 x= 0 y=-2
#define setFont_S u8g.setFont(u8g_font_chikitar)   // 5 pixel height, Font Bounding box     w= 9 h=10 x= 0 y=-2

//==========================
#define Core_Plus 1
#define GPS_RXTX_Default 0
//==========================
#define PIN_CS_SD 7
//==========================
#define init_updata 500			//gps update interval
#define init_sdwrite 3000		//SD writing interval
#define init_serial 5000		//Serial writing interval
#define init_oled 600			//OLED refreshing interval
#define init_key 100			//OLED refreshing interval
unsigned long timer = millis();
unsigned long time_sdwrite = millis();
unsigned long time_serial = millis();
unsigned long time_oled = millis();
unsigned long time_key = millis();
//==========================
boolean STA;	// GPS status

float f_latitude,f_longitude;	// coordinate
char c_lat,c_lon;		//

int itime[4];	// time
int idate[3];	// date

float f_Speed;	// speed
int i_Speed[2];	// speed format

float f_Height;	// height

int i_satellites;	// number of satellite

float f_fixquality;	// signal quality


//PKJ=======================================
#define num_name 13		// length of file name

char file_name[num_name]="";	// file name

boolean file_updata;		// if update new GPX file
boolean sd_sta,file_sta;	//SD card status, file status
int file_num;				// file serial number

//==========================
#include <SD.h>
File myFile;
#include <GPX.h>
GPX myGPX;
//==========================
#include <Adafruit_GPS.h>
#if GPS_RXTX_Default
  Adafruit_GPS GPS(&Serial);
#else
  #if Core_Plus
    Adafruit_GPS GPS(&Serial1);
  #else
    #include <SoftwareSerial.h>
    SoftwareSerial mySerial(3, 2);
    Adafruit_GPS GPS(&mySerial);
  #endif
#endif
//==========================
#include <ArduinoMAVLink.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 4);  // RX, TX
ArduinoMAVLink mavLink(&mySerial);
uint32_t acmAltitude = 5;
boolean mavlink_initial = false;
/*
#if Core_Plus
  ArduinoMAVLink mavLink(&Serial);
#else
  #include <SoftwareSerial.h>
  SoftwareSerial mySerial(3, 2);
  ArduinoMAVLink mavLink(&mySerial);
#endif
*/
//==========================
uint8_t SysStage = STAGE_NONE;
uint8_t LastSysStage = STAGE_NONE;

boolean noSTA_draw = true;

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
int8_t question_state = 0;

//Serail debug===================================================
void voserial()
{
  if (time_serial > millis()) time_serial = millis();
  if (millis() - time_serial <= init_serial)
    return;

  time_serial = millis(); // reset the timer

  if (!STA)
  {
    Serial.println(F("NO GPS"));
    return;
  }

  Serial.print(F("Speed:"));
  Serial.print(i_Speed[1]);
  Serial.print(F("."));
  Serial.print(i_Speed[0]);
  Serial.print(F("\t"));
  
  Serial.print(F("Lat.: "));
  Serial.print( c_lat);
  Serial.print(F(" "));
  Serial.print( f_latitude,8);
  Serial.print(F("\t"));

  Serial.print(F("Lon.: "));
  Serial.print( c_lon);
  Serial.print(F(" "));
  Serial.print(f_longitude,8);
  Serial.print(F("\t"));

  Serial.print(F("ELE.:"));
  Serial.print(int(f_Height));
  Serial.print(F("\t"));

  Serial.print(F("Sat.:"));
  Serial.print(i_satellites);    
  
  Serial.print(F("\n"));
}

// Mavlink callback===================================================
void MAVLinkStatusReport(uint8_t status, uint32_t msg)
{
  u8g.firstPage();
  do  {  
    setFont_L;
    u8g.setPrintPos(0, 18);
    if ( status == ML_INITIAL )
    {
      u8g.print(F("==MAVLink=="));  
      u8g.setPrintPos(2, 36);
      u8g.print(F("Init"));        
      setFont_M;
      u8g.setPrintPos(2, 50);
      u8g.print(F("Elapsed Time: "));
      u8g.print(msg);
    }
  } while( u8g.nextPage() );   
}

void setup()
{
  Serial.println(F("readCFG"));
  readCFG();
  setNextSysStage(STAGE_INIT);
  Serial.begin(57600);
  GPS.begin(38400);
  mavLink.begin(57600);
  mavLink.SetStatusCallback(MAVLinkStatusReport);
  Serial.println(F("setUp Mavlink"));
  
  file_sta = false;
  sd_sta = false;
  pinMode(PIN_CS_SD, OUTPUT);
  if (SD.begin(PIN_CS_SD)) 
    sd_sta = true;
  else
    Serial.println(F("sd card initialization failed"));
  if (sd_sta)
    checkLastTimeFileStatus();
  Serial.println(F("init sdcard"));
    
  delay(1000);
  volcdlogo(0,0, false);
  delay(1000);
  volcdlogo(0,0, true);
}

void loop()
{
  //GPS-------------------------------
  if ( b_read_gps )
  {
    vogps_databegin();  
    vogps_dataread();
  
    if ( b_write_sdcard )
      vosdwrite();
    
    if ( b_debug_serail )
      voserial();
  }  
  
  voCubeV1Key();
  
  if ( SysStage == STAGE_INIT )
  {
    if ( anyKey() )
    {
        setNextSysStage(STAGE_MENU);
    }
  }
  else if ( SysStage == STAGE_MENU )
  {
    if (  menu_redraw_required != 0 ) 
    {
      u8g.firstPage();
      do  {
        drawMenu();
      } while( u8g.nextPage() );
      menu_redraw_required = 0;
    }
    updateMenu();
  }
  else if ( SysStage == STAGE_GPS )
  {
    //OLED-------------------------------
    if ( STA )
      vooled();  
    else if ( noSTA_draw )
    {
      drawText("No GPS.", 2, 18, 0);
      noSTA_draw = false;
    }
    updateMenu();
  }
  else if ( SysStage == STAGE_ACM )
  {
    if (  menu_redraw_required != 0 ) 
    {
      u8g.firstPage();
      do  {
        drawACMMenu();
      } while( u8g.nextPage() );
      menu_redraw_required = 0;
    }
    if ( !mavlink_initial )
      InitMavlink();
    updateMenu();    
  }
  else if ( SysStage == STAGE_CFG )
  {
    if (  menu_redraw_required != 0 ) 
    {
      u8g.firstPage();
      do  {
        drawCFGMenu();
      } while( u8g.nextPage() );
      menu_redraw_required = 0;
    }    
    updateConfigure();        
  }
  else if ( SysStage == STAGE_ABT )
  {
    if ( menu_redraw_required != 0 ) 
    {
      /*
      u8g.firstPage();
      do  {
        drawAbout();
      } while( u8g.nextPage() );
      */
      Serial.println(F("draw about"));
      drawText("About", 2, 18, 0);
      menu_redraw_required = 0;
    }
    updateMenu(); 
  }
  else if ( SysStage == STAGE_QUESTION )
  {
    if (  menu_redraw_required != 0 ) 
    {
      u8g.firstPage();
      do  {
        drawQuestionMenu();
      } while( u8g.nextPage() );
      menu_redraw_required = 0;
    }    
    updateQuestion();            
  }
  else if ( SysStage == STAGE_ACM_FOLLOW )
  {    
    if ( question_state == 0 )
    {      
      if (STA)
      {
        mavLink.DoFlyHere(f_latitude, f_longitude, acmAltitude);
        menu_redraw_required = 1;
      }
      
      if (  menu_redraw_required != 0 ) 
      {
        u8g.firstPage();
        do  {
          drawFollow();
        } while( u8g.nextPage() );
        menu_redraw_required = 0;
      }
      updateAltitude();                         
    }
    else
    {
      if ( question_state < 0 ) // No, do nothing
      {
      }
      else if ( question_state > 0 ) // Yes
      {
        mavLink.DoLoiter();
        menu_current = MENU_ACM_FOLLOW;    
        setNextSysStage(STAGE_ACM);     
        LastSysStage = STAGE_MENU;
      }
      question_state = 0;
    }    
  }

  if ( mavlink_initial )
  {
    if ( SysStage == STAGE_ACM 
      || SysStage == STAGE_ACM_FOLLOW )
    {
      mavLink.Received();
    }
  }
  
}

