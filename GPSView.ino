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