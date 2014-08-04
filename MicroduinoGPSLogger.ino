#include <EEPROM.h>
boolean b_write_sdcard = false;
boolean b_read_gps = false;
boolean b_debug_serail = false;
#define CFG_EEPROM_ADDR 100;

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

//oled=======================================
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//-------Font size: large, midium, small
#define setFont_L u8g.setFont(u8g_font_courB14)    // 11 pixel height,Font Bounding box     w=17 h=26 x=-4 y=-7
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)  // 7 pixel height, Font Bounding box     w= 7 h= 9 x= 0 y=-2
#define setFont_S u8g.setFont(u8g_font_chikitar)   // 5 pixel height, Font Bounding box     w= 9 h=10 x= 0 y=-2
/*
font:
 u8g.setFont(u8g_font_7x13)
 u8g.setFont(u8g_font_fixed_v0r);
 u8g.setFont(u8g_font_chikitar);
 u8g.setFont(u8g_font_osb21);
 */

#define u8g_logo_width 128
#define u8g_logo_height 18

const unsigned char u8g_logo_bits[] U8G_PROGMEM =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0xE0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00,
  0x00, 0xFE, 0xF9, 0xF7, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0xF8,
  0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF9, 0xE1, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x38, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xEF, 0xF9, 0x8F, 0xD7, 0x73, 0xF1, 0xC1, 0x3B, 0x9F, 0xFF,
  0xFF, 0x1E, 0x3E, 0x00, 0x00, 0xBC, 0xEF, 0xC1, 0xE1, 0x9F, 0xFF, 0xDD,
  0xE3, 0x3F, 0xCC, 0xE1, 0xF0, 0xBF, 0x7B, 0x00, 0x00, 0x3C, 0xF7, 0xE1,
  0xE1, 0x9F, 0xFF, 0xC6, 0xF7, 0x3E, 0x8E, 0xF3, 0xF0, 0xFF, 0xF8, 0x00,
  0x00, 0x3C, 0xF3, 0xE1, 0xF1, 0x93, 0xFF, 0xE6, 0xF7, 0x3C, 0x8F, 0xF7,
  0xF0, 0xFF, 0xFC, 0x00, 0x00, 0x7C, 0xF2, 0xE1, 0xF1, 0x83, 0x87, 0xFE,
  0xF7, 0x39, 0xFF, 0xF7, 0xF0, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xF0, 0xE3,
  0xF3, 0xA3, 0x03, 0xFE, 0xF7, 0x3F, 0xFF, 0xF7, 0x71, 0xFC, 0xFF, 0x00,
  0x00, 0x7C, 0xF8, 0xE3, 0xF3, 0xBF, 0x03, 0xFE, 0xE3, 0x3F, 0xFF, 0xF3,
  0x71, 0xDC, 0x7F, 0x00, 0x00, 0x7E, 0xFC, 0xE7, 0xE3, 0xBF, 0x03, 0xFC,
  0xE3, 0x3F, 0xFE, 0xF3, 0x71, 0x9C, 0x7F, 0x00, 0x00, 0xC1, 0x03, 0xF8,
  0xCF, 0xE7, 0x0F, 0xF0, 0x00, 0x7F, 0xFC, 0xFC, 0xFF, 0x3E, 0x1E, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

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
uint32_t apmAltitude = 5;
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

//lat_lon_transform================================
void lat_lon_transform()
{
  f_latitude=(int(f_latitude)/100)+((int(f_latitude)%100)/60.0)+((f_latitude-int(f_latitude))/60.0);
  if(c_lat=='S')		//South
    f_latitude=-f_latitude;

  //---------------------------------

  f_longitude=(int(f_longitude)/100)+((int(f_longitude)%100)/60.0)+((f_longitude-int(f_longitude))/60.0);
  if(c_lon=='W')		//West
    f_longitude=-f_longitude;
}

// FileName_StringtoChar================================
// create a new file name with date from gps
void vostring()
{
  String s_file_name="";

  for(int a=0;a<3;a++)
  {
    if(idate[a]<10)
      s_file_name+="0";
    s_file_name+=idate[a];
  }
  if(file_num<10)
    s_file_name+="0";
  s_file_name+=file_num;
  s_file_name+=".gpx";

  for(int a=0;a<(num_name-1);a++)
  {
    file_name[a]=s_file_name[a];
  }
}

//GPS========================================
void vogps_databegin()
{
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
}

void vogps_dataread()
{
  if (timer > millis()) timer = millis();
  if (millis() - timer > init_updata)
  {
    timer = millis(); // reset the timer

    itime[0]=GPS.hour;
    itime[1]=GPS.minute;
    itime[2]=GPS.seconds;
    itime[3]=GPS.milliseconds;

    idate[0]=GPS.year;
    idate[1]=GPS.month;
    idate[2]=GPS.day;

    f_fixquality=GPS.fixquality;	//get signal quality
    STA=GPS.fix;					//GPS status

    if (STA)		// while GPS is online
    {
      f_latitude=GPS.latitude;
      f_longitude=GPS.longitude;
      c_lat=GPS.lat;
      c_lon=GPS.lon;

      lat_lon_transform();		// trasform latitude and longitude

      f_Speed=1.852*GPS.speed;		// transform speed
      i_Speed[0]=int(f_Speed*10)%10;	// formating speed
      i_Speed[1]=int(f_Speed);		// formating speed

      f_Height=GPS.altitude;		// altitude

      i_satellites=GPS.satellites;      // number of satellite

    }
  }
}

//OLED===================================================
void vooled()
{
  if (time_oled > millis()) time_oled = millis();
  if(millis()-time_oled>init_oled)
  {
    u8g.firstPage();
    do
    {
      GPSDraw();
    }
    while( u8g.nextPage() );
    time_oled=millis();
  }
}

void volcdlogo(unsigned int x, unsigned int y, boolean msg)
{
  u8g.firstPage();
  do
  {
    u8g.drawXBMP( x, y, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
    if ( msg )
    {
      setFont_S;
      u8g.setPrintPos(2, 64);    
      u8g.print(F("press any key to continue...")); 
    }
  }
  while( u8g.nextPage() );
}

void GPSDraw(void)
{
  setFont_L;
  u8g.setPrintPos(2, 18);
  u8g.print(F("Speed:"));
  if(STA)
  {
    u8g.print(i_Speed[1]);
    setFont_M;
    u8g.print(".");
    u8g.print(i_Speed[0]);
  }
  else
  {
    u8g.print(F("N/A"));
    setFont_M;
  }

  u8g.setPrintPos(2, 32);
  u8g.print(F("Lat.: "));
  u8g.print( c_lat);
  u8g.print(" ");
  u8g.print( f_latitude,4);

  u8g.setPrintPos(2, 41);
  u8g.print(F("Lon.: "));
  u8g.print( c_lon);
  u8g.print(F(" "));
  u8g.print(f_longitude,4);


  u8g.drawLine(0, 44, 128, 44);

  u8g.drawLine(0, 55, 128, 55);

  u8g.setPrintPos(2, 53);
  u8g.print(F("20"));
  u8g.print(idate[0]);
  u8g.print(F("-"));
  u8g.print(idate[1]);
  u8g.print(F("-"));
  u8g.print(idate[2]);

  u8g.print(F("  "));
  u8g.print(itime[0]);
  u8g.print(F(":"));
  u8g.print(itime[1]);
  u8g.print(F(":"));
  u8g.print(itime[2]);

  for(int a=0;a<3;a++)
  {
    u8g.drawFrame(2+(5*a), 61-(a*2), 4, 3+(a*2));
  }
  for(int a=0;a<f_fixquality+1;a++)
  {
    u8g.drawBox(2+(5*a), 61-(a*2), 4, 3+(a*2));
  }

  u8g.setPrintPos(72, 64);
  u8g.print(F("ELE.:"));
  u8g.print(int(f_Height));

  u8g.setPrintPos(20, 64);
  u8g.print(F("Sat.:"));
  u8g.print(i_satellites);
}

#define KEY_NONE   0
#define KEY_LEFT   1
#define KEY_RIGHT  2
#define KEY_SELECT 3
#define KEY_UP     4
#define KEY_DOWN   5
#define KEY_SINGLE 6
#define KEY_DOUBLE 7
#define KEY_PREV   KEY_LEFT
#define KEY_NEXT   KEY_RIGHT

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
void voCubeV1Key()
{
  if (time_key > millis()) time_key = millis();
  if(millis()-time_key>init_key)
  {  
    uiKeyCodeSecond = uiKeyCodeFirst;
    int button2 = analogRead(A6);
    if(button2>365&&button2<375)
      uiKeyCodeFirst=KEY_SINGLE;  // single
    else if(button2>240&&button2<250)
      uiKeyCodeFirst=KEY_DOUBLE;  // double
    
    int button5 = analogRead(A7);
    if(button5<40)  //left
      uiKeyCodeFirst=KEY_LEFT;
    else if(button5>55&&button5<65) // right
      uiKeyCodeFirst=KEY_RIGHT;
    else if(button5>135&&button5<145) // up
      uiKeyCodeFirst=KEY_UP;
    else if(button5>230&&button5<240) // down
      uiKeyCodeFirst=KEY_DOWN;
    else if(button5>360&&button5<375) // middle
      uiKeyCodeFirst=KEY_SELECT;
    else
      uiKeyCodeFirst=KEY_NONE;   
      
    if ( uiKeyCodeSecond == uiKeyCodeFirst )
      uiKeyCode = uiKeyCodeFirst;
    else
      uiKeyCode = KEY_NONE;        
  }
}  

//SD Card===================================================
#define   WRDELAY        10
#define TmpFileName "GpsTmpFile.tmp"
void checkLastTimeFileStatus()
{
  if (SD.exists(TmpFileName))
  {
    char tmp[num_name];
    tmp[0] = '\0';
    File file = SD.open(TmpFileName);
    if (file)
    {
      char * pTmp = tmp;
      while (file.available()) 
      {
        *(pTmp++) = file.read();
      }
      *pTmp = 0;
      file.close();      
    }
    
    //write end tag
    if ( tmp[0] != '\0')
    {
      myFile = SD.open(tmp, FILE_WRITE);    
      if (myFile)
      {
        closeElement();
        myFile.close();      
      }    
    }
    
    SD.remove(TmpFileName);
  }  
}

void openElement(){
  myFile.print(myGPX.getOpen());
  delay(WRDELAY);
  myFile.print(myGPX.getTrakOpen());
  delay(WRDELAY);
  myFile.print(myGPX.getTrakSegOpen());  
  
//  if (myFile.writeError || !myFile.sync()) error ("print or sync");
}

void closeElement(){
  myFile.print(myGPX.getTrakSegClose());
  delay(WRDELAY);
  myFile.print(myGPX.getTrakClose());
  delay(WRDELAY);
  myFile.print(myGPX.getClose());
  
//  if (file.writeError || !file.sync()) error ("print or sync");
}

void vosdwrite()
{
  if (time_sdwrite > millis()) time_sdwrite = millis();
  if (millis() - time_sdwrite <= init_sdwrite)
    return;
  
  time_sdwrite = millis(); // reset the timer
  
  if ( !sd_sta )
    return;
    
  if ( !file_sta )
  {
    if ( !STA )
      return;    
    do
    {
      vostring();
      file_num++;
    } while(SD.exists(file_name));
    myFile = SD.open(file_name, FILE_WRITE);
    if (myFile)
    {
      file_sta = true;
      openElement();
      myFile.close();
      
      File file = SD.open(TmpFileName, FILE_WRITE);
      if (file)
      {
        file.print(file_name);
        file.close();
      }
    }
    else
    {
      Serial.println(F("sd card file creation failed"));
    }
  }
  
  if ( file_sta ) // write date
  {
    myFile = SD.open(file_name, FILE_WRITE);  
    
    //Elevation
    myGPX.setEle(String(int(f_Height)));

    //Speed
    String speed = String(i_Speed[1]) + "." + String(i_Speed[0]);
    myGPX.setSpeed(speed);

    //Date/Time
    char* stmp;    
    stmp=(char*)malloc(sizeof(char)*42);
    sprintf_P(stmp,PSTR("20%2.i-%0.2i-%0.2iT%0.2i:%0.2i:%0.2i.%0.2i+00:00"),idate[0],idate[1],idate[2],itime[0],itime[1],itime[2],itime[3]);
    myGPX.setTime(stmp);

    // Set lon, lat and get track data
    char flatBuffer[20]; //buffer for dtostrf() to use
    char flonBuffer[20]; //buffer for dtostrf() to use
    dtostrf(f_latitude, 10, 6, flatBuffer); //Convert Latitude to a char array
    dtostrf(f_longitude, 10, 6, flonBuffer); //Convert Latitude to a char array
    myFile.print(myGPX.getPt(GPX_TRKPT, flonBuffer, flatBuffer));      
    
    free(stmp);    
    myFile.close();
  }
  
}

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

//Menus ===================================================
#define STAGE_NONE 0
#define STAGE_INIT 1
#define STAGE_MENU 2
#define STAGE_GPS  3  // 
#define STAGE_CFG  4  // config
#define STAGE_APM  5  // amp control
#define STAGE_ABT  6  // about
#define STAGE_QUESTION  7  // about
#define STAGE_MAIN_TOTAL STAGE_QUESTION
#define STAGE_APM_FOLLOW  STAGE_MAIN_TOTAL+1  // apm follow me


uint8_t SysStage = STAGE_NONE;
uint8_t LastSysStage = STAGE_NONE;

boolean noSTA_draw = true;

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;

#define MENU_ITEMS 4
#define MENU_GPS  0 
#define MENU_APM  1 
#define MENU_CFG  2 
#define MENU_ABT  3 
char *menu_strings[MENU_ITEMS] = { "GPS Status", "APM Planner", "Configuration", "About" };
void drawMenu() 
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
}

#define MENU_APM_ITEMS 4
#define MENU_APM_ARM      0 
#define MENU_APM_FOLLOW   1 
#define MENU_APM_TAKEOFF  2 
#define MENU_APM_RTL      3 
char *menu_apm_strings[MENU_APM_ITEMS] = { "Arm", "Follow me", "Takeoff", "RTL" };
void drawAPMMenu() 
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_APM_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_apm_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_apm_strings[i]);
  }
}

#define MENU_CFG_ITEMS 3
#define MENU_CFG_GPS      0 
#define MENU_CFG_SDCARD   1 
#define MENU_CFG_SERIAL   2 
char *menu_cfg_strings[MENU_CFG_ITEMS] = { "GPS Read", "SDCard Write", "Serial Debug" };
void drawCFGMenu()
{
  uint8_t i, h, optSize;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  optSize = h - 2 ;
  for( i = 0; i < MENU_CFG_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_cfg_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(h+2, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }

    u8g.drawStr(d, i*h, menu_cfg_strings[i]);
    
    u8g.setDefaultForegroundColor();
    if ( i == MENU_CFG_GPS && b_read_gps 
      || i == MENU_CFG_SDCARD && b_write_sdcard 
      || i == MENU_CFG_SERIAL && b_debug_serail )
    {      
      u8g.drawBox(2, i*(h)+2, optSize, optSize);    
    }
    else
      u8g.drawFrame(2, i*(h)+2, optSize, optSize);    
  }  
}

void drawAbout()
{
  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.setPrintPos(2, 64);
  u8g.print(F("It's me, baby."));
}

#define MENU_QUESTION_ITEMS 2
#define MENU_QUESTION_NO    0 
#define MENU_QUESTION_YES   1 
int8_t question_state = 0;
char questionTitle[10];
char *menu_question_strings[MENU_QUESTION_ITEMS] = { "No", "Yes" };
void drawQuestionMenu()
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_APM_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_question_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, h+i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, h+i*h, menu_question_strings[i]);
  }  
  
//  u8g.drawStr(2, 0, questionTitle);
  u8g.drawStr(2, 0, "Leave ?");  
  
}

void drawFollow()
{
  setFont_M;
  
  for(int a=0;a<3;a++)
  {
    u8g.drawFrame(2+(5*a), 5-(a*2), 4, 3+(a*2));
  }
  for(int a=0;a<f_fixquality+1;a++)
  {
    u8g.drawBox(2+(5*a), 5-(a*2), 4, 3+(a*2));
  }

  u8g.setPrintPos(20, 8);
  u8g.print(F("Sat.:"));
  u8g.print(i_satellites);  

  u8g.setPrintPos(2, 17);
  u8g.print(F("Lat.: "));
  u8g.print(f_latitude,4);
  u8g.setPrintPos(2, 26);
  u8g.print(F("Lon.: "));
  u8g.print(f_longitude,4);

  u8g.setPrintPos(72, 8);
  u8g.print(F("set Alt.:"));
  setFont_L;
  u8g.setPrintPos(90, 24);  
  u8g.print(apmAltitude);
  setFont_M;


  // draw gps time
  u8g.drawLine(0, 52, 128, 52);
  u8g.drawLine(0, 63, 128, 63);

  u8g.setPrintPos(2, 61);
  u8g.print(F("20"));
  u8g.print(idate[0]);
  u8g.print(F("-"));
  u8g.print(idate[1]);
  u8g.print(F("-"));
  u8g.print(idate[2]);

  u8g.print(F("  "));
  u8g.print(itime[0]);
  u8g.print(F(":"));
  u8g.print(itime[1]);
  u8g.print(F(":"));
  u8g.print(itime[2]);



}

//Button handle===================================================
void prevBtn()
{
  boolean back2Menu = false;
  switch(SysStage)
  {
    case STAGE_GPS:
      menu_current = MENU_GPS;
      back2Menu = true;
    break;      
    case STAGE_CFG:
      writeCFG();
      menu_current = MENU_CFG;
      back2Menu = true;
    break;      
    case STAGE_APM:
      menu_current = MENU_APM;   
      back2Menu = true; 
    break;      
    case STAGE_ABT:
      menu_current = MENU_ABT;    
      back2Menu = true;
    break;
    case STAGE_QUESTION:
      question_state = -1;
      menu_current = 0;
      back2Menu = true;
    break;
    case STAGE_APM_FOLLOW:
      question_state = 0;
      menu_current = MENU_QUESTION_YES;
      setNextSysStage(STAGE_QUESTION);
    break;
    default:
    break;
  }
  if ( back2Menu )
    setNextSysStage(LastSysStage);

}

void nextBtn()
{
  if ( SysStage == STAGE_MENU )
  {
    switch(menu_current)
    {
      case MENU_GPS:
        noSTA_draw = true;
        setNextSysStage(STAGE_GPS);
      break;
      case MENU_APM:
        menu_current = 0;
        setNextSysStage(STAGE_APM);
      break;
      case MENU_CFG:
        menu_current = 0;
        setNextSysStage(STAGE_CFG);
      break;
      case MENU_ABT:
        setNextSysStage(STAGE_ABT);
      break;
    }
  }
  else if ( SysStage == STAGE_APM )
  {    
    switch(menu_current)
    {
      case MENU_APM_ARM:
      break;
      case MENU_APM_FOLLOW:
        setNextSysStage(STAGE_APM_FOLLOW);
        if ( !b_read_gps )
        {
          b_read_gps = true;
          writeCFG();
        }
      break;
      case MENU_APM_TAKEOFF:
      break;
      case MENU_APM_RTL:
      break;
    }
    menu_redraw_required = 1;    
  }
  else if ( SysStage == STAGE_CFG )
  {
    switch(menu_current)
    {
      case MENU_CFG_GPS:
        b_read_gps = !b_read_gps;
      break;
      case MENU_CFG_SDCARD:
        b_write_sdcard = !b_write_sdcard;
      break;
      case MENU_CFG_SERIAL:
        b_debug_serail = !b_debug_serail;
      break;
    }
    menu_redraw_required = 1;
  }
  else if ( SysStage == STAGE_ABT )
  {
  }
  else if ( SysStage == STAGE_QUESTION )
  {
    if ( menu_current == MENU_QUESTION_YES )
      question_state = 1;
    else 
      question_state = -1;
    setNextSysStage(LastSysStage);
  }
}

void selectBtn()
{
  if ( SysStage == STAGE_MENU 
    || SysStage == STAGE_CFG
    || SysStage == STAGE_APM )
    nextBtn();
}

void updateMenu() 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) 
  {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) 
  {
    case KEY_DOWN:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_UP:
      if ( menu_current <= 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      prevBtn();
      break;
    case KEY_NEXT:
      nextBtn();
      break;
    case KEY_SELECT:
      selectBtn();
      break;
  }
}

void updateConfigure() 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) 
  {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) 
  {
    case KEY_DOWN:
      menu_current++;
      if ( menu_current >= MENU_CFG_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_UP:
      if ( menu_current <= 0 )
        menu_current = MENU_CFG_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      prevBtn();
      break;
    case KEY_NEXT:
      nextBtn();
      break;
    case KEY_SELECT:
      selectBtn();
      break;
  }
}

void updateQuestion() 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) 
  {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) 
  {
    case KEY_DOWN:
      menu_current++;
      if ( menu_current >= MENU_QUESTION_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_UP:
      if ( menu_current <= 0 )
        menu_current = MENU_QUESTION_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      prevBtn();
      break;
    case KEY_NEXT:
    case KEY_SELECT:
      nextBtn();
      break;
  }
}

// apm handle ===================================================
void updateAltitude() 
{
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) 
  {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) 
  {
    case KEY_UP:
      menu_redraw_required = 1;
      apmAltitude++;
      if ( apmAltitude > 30 )
        apmAltitude = 30;
      break;
    case KEY_DOWN:
      menu_redraw_required = 1;
      apmAltitude--;
      if ( apmAltitude < 1 )
        apmAltitude = 1;
      break;
    case KEY_PREV:
      prevBtn();
      break;
    case KEY_NEXT:
      nextBtn();
      break;
    case KEY_SELECT:
      selectBtn();
      break;
  }
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
//===================================================
void drawText(char * text, uint8_t posX, uint8_t posY, uint8_t size)
{    
  u8g.firstPage();
  do  {
    if ( size == 0 )
      setFont_L;
    else if ( size == 1)
      setFont_M;
    else
      setFont_S;
    u8g.setPrintPos(posX, posY);
    u8g.print(text);
  } while( u8g.nextPage() );  
}
//===================================================
void setNextSysStage( uint8_t next )
{
  LastSysStage = SysStage;
  SysStage = next;
  menu_redraw_required = 1;  
}

void readCFG()
{
  return;
  
  
  uint8_t eeprom_addr = CFG_EEPROM_ADDR;
  b_write_sdcard = EEPROM.read(eeprom_addr);
  eeprom_addr++;
  b_read_gps = EEPROM.read(eeprom_addr);
  eeprom_addr++;
  b_debug_serail = EEPROM.read(eeprom_addr);
  eeprom_addr++;
}

void writeCFG()
{
  return;
  
  
  uint8_t eeprom_addr = CFG_EEPROM_ADDR;
  EEPROM.write(eeprom_addr, b_write_sdcard);
  eeprom_addr++;
  EEPROM.write(eeprom_addr, b_read_gps);
  eeprom_addr++;
  EEPROM.write(eeprom_addr, b_debug_serail);
  eeprom_addr++;  
}

#include <ArduinoMAVLinkHeader.h>
/*
void getHeartBeat()
{
  char buffer[64];
  uint8_t bufIdx = 0;
  while( true )
  {
    if ( mySerial.available() > 0 )
    {
      uint8_t c = mySerial.read();
      buffer[bufIdx++] = c;
      if ( bufIdx > 5 )
      {
        if ( buffer[5] == MAVLINK_MSG_ID_HEARTBEAT )
        {
            mavlink_heartbeat_t hb;
            memcpy( &hb, buffer, bufIdx );
            Serial.print("Heart beat: ");
            Serial.print(hb.mavlink_version);
            Serial.print(" ");
            Serial.print(hb.type);
            Serial.print(" ");
            Serial.print(hb.autopilot);
            Serial.print(" ");
            Serial.print(int(buffer[3]));
            Serial.print(" ");
            Serial.print(int(buffer[4]));
            Serial.print(" ");
            Serial.print(int(buffer[2]));
            Serial.println(" ");
        }
        bufIdx = 0;
      }
    }
    
  }
  
}
*/
 
void InitMavlink()
{  
/*  
  //char _receive_buffer[256];
  mavlink_message_t msg;
  mavlink_status_t status;
  mavlink_heartbeat_t hb;
  
  uint8_t bufIdx = 0;
  uint8_t headIdx = 0;
  uint8_t hb_count = 0;
  timer = millis();
  while (hb_count < 2)
  {  
    unsigned long elapsed_time = millis() - timer;
    if ( elapsed_time > 30000 )      
    {
      Serial.println("time out");
      return;
    }
    
    while (mySerial.available() > 0) 
    {
      Serial.print(bufIdx);
      uint8_t c = mySerial.read();
      //_receive_buffer[bufIdx++] = c;
      Serial.print(" mavlink get: ");
      Serial.println(int(c));
      if (c==MAVLINK_STX)
        headIdx = bufIdx-1;
      // Try to get a new message
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) 
      {
        Serial.print("parsing: ");
        Serial.print(msg.msgid);
        Serial.print(" ");
        Serial.print(headIdx);
        Serial.print(" ");
        Serial.println(bufIdx);        
        // Handle message    
        switch(msg.msgid)
        {
          case MAVLINK_MSG_ID_HEARTBEAT:
          {
            //_receive_buffer[bufIdx] = '\0';
            //memcpy( &hb, &_receive_buffer[headIdx+6], MAVLINK_MSG_ID_HEARTBEAT_LEN );
            mavlink_msg_heartbeat_decode(&msg, &hb);
            //bufIdx = 0;
            //char * p_msg = (char *)&msg;
            //char * p_hb = (char*)&hb;
            //for(int i = 0; i < MAVLINK_MSG_ID_HEARTBEAT_LEN; ++i )
            //  p_hb[i] = p_msg[i];
            //hb = (mavlink_heartbeat_t *)&_receive_buffer[headIdx+6];
            
            
            Serial.print("Hearbeat: ");
            Serial.print(hb.type);
            Serial.print(" ");
            Serial.print(hb.mavlink_version);
            Serial.print(" ");
            Serial.print(hb.autopilot);
            Serial.print(" ");
            Serial.print(msg.sysid);
            Serial.print(" ");
            Serial.print(msg.compid);
            Serial.print(" ");
            Serial.print(msg.seq);
            Serial.println(" ");    
            hb_count= 2;        
          }
          break;
          default:
          //Do nothing
          break;
        }
        bufIdx = 0;
      }
      
      if ( bufIdx > 255 )
        bufIdx = 0;
      // And get the next one
    }
  }  
  
  mavlink_initial = true;

*/  
  
  
  Serial.println(F("init mvalink"));
  delay(1000);
  if ( mavLink.Initialize() )
  {
    drawText("Init MAVLink Success.", 2, 18, 2);      
    mavlink_initial = true;    
  }
  else
  {
    drawText("Init MAVLink Failed.", 2, 18, 2);      
    mavlink_initial = false;
  }
  delay(3000);  
  
  
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
    if ( uiKeyCode != KEY_NONE )
    {
        setNextSysStage(STAGE_MENU);
        last_key_code = uiKeyCode;
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
  else if ( SysStage == STAGE_APM )
  {
    if (  menu_redraw_required != 0 ) 
    {
      u8g.firstPage();
      do  {
        drawAPMMenu();
      } while( u8g.nextPage() );
      menu_redraw_required = 0;
    }
    if ( !mavlink_initial )
      //getHeartBeat();
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
  else if ( SysStage == STAGE_APM_FOLLOW )
  {    
    if ( question_state == 0 )
    {
      
      if (STA)
      {
        mavLink.FlyHere(f_latitude, f_longitude, apmAltitude);
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
        // CancelFollowMe();
        menu_current = MENU_APM_FOLLOW;    
        setNextSysStage(STAGE_APM);     
        LastSysStage = STAGE_MENU;
      }
      question_state = 0;
    }    
  }

}

