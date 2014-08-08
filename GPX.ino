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