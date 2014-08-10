void gps_databegin()
{
  char c = gps.read();
  // if you want to debug, this is a good time to do it!
  // if a sentence is received, we can check the checksum, parse it...
  if (gps.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    if (!gps.parse(gps.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
}

#define init_updata 500			//gps update interval
unsigned long gps_timer = millis();

void gps_dataread()
{
  if (gps_timer > millis()) gps_timer = millis();
  if (millis() - gps_timer > init_updata)
  {
    gps_timer = millis(); // reset the timer

    itime[0]=gps.hour;
    itime[1]=gps.minute;
    itime[2]=gps.seconds;
    itime[3]=gps.milliseconds;

    idate[0]=gps.year;
    idate[1]=gps.month;
    idate[2]=gps.day;

    f_fixquality=gps.fixquality;	//get signal quality
    STA=gps.fix;					//GPS status

    //if (gps.fix)		// while GPS is online
    {
      f_latitude=gps.latitude;
      f_longitude=gps.longitude;
      c_lat=gps.lat;
      c_lon=gps.lon;

      lat_lon_transform();		// trasform latitude and longitude

      f_Speed=1.852*gps.speed;		// transform speed
      i_Speed[0]=int(f_Speed*10)%10;	// formating speed
      i_Speed[1]=int(f_Speed);		// formating speed

      f_Height=gps.altitude;		// altitude

      i_satellites=gps.satellites;      // number of satellite

    }
  }
}

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

#define init_serial 5000		//Serial writing interval
unsigned long time_serial = millis();

void gps_serial()
{
  if (time_serial > millis()) time_serial = millis();
  if (millis() - time_serial <= init_serial)
    return;

  time_serial = millis(); // reset the timer

  Serial.print("fix:");
  Serial.println(STA);

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
