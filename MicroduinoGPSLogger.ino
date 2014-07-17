//==========================
#define PIN_CS_SD 7
//==========================
#define init_updata 500			//gps update interval
#define init_sdwrite 3000		//SD writing interval
#define init_serial 5000		//SD writing interval
unsigned long timer = millis();
unsigned long time_sdwrite = millis();
unsigned long time_serial = millis();
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

int idate_cache;			// date cache

#include <SD.h>
File myFile;

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
//SoftwareSerial mySerial(3, 2);
//Adafruit_GPS GPS(&mySerial);
Adafruit_GPS GPS(&Serial1);

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

void vogps_datasdwrite()
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
      myFile.close();
    }
    else
    {
      Serial.println("sd card file creation failed");
    }
  }
  
  if ( file_sta ) // write date
  {
    myFile = SD.open(file_name, FILE_WRITE);
    
    String s_timestamp="";
  
    for(int a=0;a<4;a++)
    {
      if(itime[a]<10)
        s_timestamp+="0";
      s_timestamp+=itime[a];
    }

    myFile.print(s_timestamp);
    myFile.print("\t");
    
    myFile.print("Speed:");
    if(STA)
    {
      myFile.print(i_Speed[1]);
      myFile.print(".");
      myFile.print(i_Speed[0]);
    }
    else
    {
      myFile.print("N/A");
    }
    myFile.print("\t");
    
    myFile.print("Lat.: ");
    myFile.print( c_lat);
    myFile.print(" ");
    myFile.print( f_latitude,8);
    myFile.print("\t");

    myFile.print("Lon.: ");
    myFile.print( c_lon);
    myFile.print(" ");
    myFile.print(f_longitude,8);
    myFile.print("\t");

    myFile.print("ELE.:");
    myFile.print(int(f_Height));
    myFile.print("\t");

    myFile.print("Sat.:");
    myFile.print(i_satellites);    
    
    myFile.print("\n");
    myFile.close();
  }
  
}

void vogps_serial()
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
  if (SD.begin(PIN_CS_SD)) 
    sd_sta = true;
  else
    Serial.println("sd card initialization failed");
  
  delay(2000);
}

void loop()
{
  //GPS-------------------------------
  vogps_databegin();

  vogps_dataread();

  vogps_datasdwrite();
  
  vogps_serial();
}

