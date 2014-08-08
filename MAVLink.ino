void InitMavlink()
{  
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