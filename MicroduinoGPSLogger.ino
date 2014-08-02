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
#define setFont_L u8g.setFont(u8g_font_courB14)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)
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
int key_up,key_down;
void vooled()
{
  if (time_oled > millis()) time_oled = millis();
  if(millis()-time_oled>init_oled)
  {
    u8g.firstPage();
    do
    {
      draw();
    }
    while( u8g.nextPage() );
    time_oled=millis();
  }
}

void volcdlogo(unsigned int x, unsigned int y)
{
  u8g.firstPage();
  do
  {
    u8g.drawXBMP( x, y, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
  }
  while( u8g.nextPage() );
}

void volcdPrintText(char * text)
{
  u8g.firstPage();
  do
  {
    setFont_L;
    u8g.setPrintPos(2, 18);
    u8g.print(text);
  }
  while( u8g.nextPage() );
}


void draw(void)
{
  setFont_L;

  u8g.setPrintPos(2, 18);

  u8g.print("Speed:");
  if(STA)
  {
    u8g.print(i_Speed[1]);
    setFont_M;
    u8g.print(".");
    u8g.print(i_Speed[0]);
  }
  else
  {
    u8g.print("N/A");
    setFont_M;
  }

  u8g.setPrintPos(2, 32);
  u8g.print("Lat.: ");
  u8g.print( c_lat);
  u8g.print(" ");
  u8g.print( f_latitude,4);

  u8g.setPrintPos(2, 41);
  u8g.print("Lon.: ");
  u8g.print( c_lon);
  u8g.print(" ");
  u8g.print(f_longitude,4);


  u8g.drawLine(0, 44, 128, 44);

  u8g.drawLine(0, 55, 128, 55);

  u8g.setPrintPos(2, 53);
      u8g.print("20");
      u8g.print(idate[0]);
      u8g.print("-");
      u8g.print(idate[1]);
      u8g.print("-");
      u8g.print(idate[2]);

      u8g.print("  ");
      u8g.print(itime[0]);
      u8g.print(":");
      u8g.print(itime[1]);
      u8g.print(":");
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
  u8g.print("ELE.:");
  u8g.print(int(f_Height));

  u8g.setPrintPos(20, 64);
  u8g.print("Sat.:");
  u8g.print(i_satellites);
}

#define KEY_UP     0x0001
#define KEY_LEFT   0x0002
#define KEY_MIDDLE 0x0004
#define KEY_RIGHT  0x0008
#define KEY_DOWN   0x0010
#define KEY_SINGLE 0x0020
#define KEY_DOUBLE 0x0040

int voCubeV1Key()
{
  if (time_key > millis()) time_key = millis();
  if(millis()-time_key>init_key)
  {  
    int button2 = analogRead(A6);
    if(button2>370&&button2<380)
      key_down=KEY_SINGLE;  // single
    else if(button2>240&&button2<250)
      key_down=KEY_DOUBLE;  // double
    else
      key_down=0;
    
    int button5 = analogRead(A7);
    if(button5<40)  //left
      key_up=KEY_LEFT;
    else if(button5>50&&button5<100) // right
      key_up=KEY_RIGHT;
    else if(button5>140&&button5<155) // up
      key_up=KEY_UP;
    else if(button5>230&&button5<250) // down
      key_up=KEY_DOWN;
    else if(button5>370&&button5<400) // middle
      key_up=KEY_MIDDLE;
    else
      key_up=0;   
            
    return key_up | key_down;
    if ( key_up != 0 || key_down!= 0 )
    {
      Serial.print("key status: ");    
      Serial.print(key_up);
      Serial.print(", ");
      Serial.println(key_down);    
      delay(100);
    }
        
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
      Serial.println("sd card file creation failed");
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

    // Sat
    myGPX.setSat(String(i_satellites));        
    
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

//Serail===================================================
void voserial()
{
  if (time_serial > millis()) time_serial = millis();
  if (millis() - time_serial <= init_serial)
    return;

  time_serial = millis(); // reset the timer

  if (!STA)
  {
    Serial.println("NO GPS");
    return;
  }

  Serial.print("Speed:");
  Serial.print(i_Speed[1]);
  Serial.print(".");
  Serial.print(i_Speed[0]);
  Serial.print("\t");
  
  Serial.print("Lat.: ");
  Serial.print( c_lat);
  Serial.print(" ");
  Serial.print( f_latitude,8);
  Serial.print("\t");

  Serial.print("Lon.: ");
  Serial.print( c_lon);
  Serial.print(" ");
  Serial.print(f_longitude,8);
  Serial.print("\t");

  Serial.print("ELE.:");
  Serial.print(int(f_Height));
  Serial.print("\t");

  Serial.print("Sat.:");
  Serial.print(i_satellites);    
  
  Serial.print("\n");
}

void setup()
{
  Serial.begin(57600);
  GPS.begin(38400);
  
  file_sta = false;
  sd_sta = false;
  pinMode(PIN_CS_SD, OUTPUT);
  if (SD.begin(PIN_CS_SD)) 
    sd_sta = true;
  else
    Serial.println("sd card initialization failed");
  if (sd_sta)
    checkLastTimeFileStatus();
    
  delay(2000);
  volcdlogo(0,0);
  delay(1000);
}

void loop()
{
  //GPS-------------------------------
  vogps_databegin();

  vogps_dataread();

  vosdwrite();
  
  voserial();
  
  //OLED-------------------------------
  if ( STA )
    vooled();  
  int keyCode = voCubeV1Key();
  if ( keyCode & KEY_UP )
  {
      volcdPrintText("Up Press!");
  }
  if ( keyCode & KEY_DOWN )
  {
      volcdPrintText("Down Press!");
  }
  if ( keyCode & KEY_LEFT )
  {
      volcdPrintText("Left Press!");
  }
  if ( keyCode & KEY_RIGHT )
  {
      volcdPrintText("Right Press!");
  }
  if ( keyCode & KEY_MIDDLE )
  {
    if (file_sta)
    {
      checkLastTimeFileStatus();
      volcdPrintText("File Written!");
      file_sta = false;
    }
    else
      volcdPrintText("Select!");
  }
  if ( keyCode & KEY_SINGLE )
  {
      volcdPrintText("Single Press!");
  }
  if ( keyCode & KEY_DOUBLE )
  {
      volcdPrintText("Double Press!");
  }
}

