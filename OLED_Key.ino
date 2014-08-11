#define init_key 100			//OLED refreshing interval
unsigned long time_key = millis();

/*
// 644 16M 5v
#define LT_UPPER_CODE 40
#define RT_LOWER_CODE 55
#define RT_UPPER_CODE 65
#define UP_LOWER_CODE 135
#define UP_UPPER_CODE 145
#define DN_LOWER_CODE 230
#define DN_UPPER_CODE 240
#define MD_LOWER_CODE 360
#define MD_UPPER_CODE 375
#define SG_LOWER_CODE 365
#define SG_UPPER_CODE 375
#define DB_LOWER_CODE 240
#define DB_UPPER_CODE 250
*/

// 1284 8M 3.3V
#define LT_UPPER_CODE 50
#define RT_LOWER_CODE 85
#define RT_UPPER_CODE 95
#define UP_LOWER_CODE 195
#define UP_UPPER_CODE 205
#define DN_LOWER_CODE 320
#define DN_UPPER_CODE 330
#define MD_LOWER_CODE 505
#define MD_UPPER_CODE 515
#define SG_LOWER_CODE 505
#define SG_UPPER_CODE 515
#define DB_LOWER_CODE 335
#define DB_UPPER_CODE 345

/*
// 1284p 16M 5v
#define LT_UPPER_CODE 40
#define RT_LOWER_CODE 60
#define RT_UPPER_CODE 70
#define UP_LOWER_CODE 140
#define UP_UPPER_CODE 150
#define DN_LOWER_CODE 235
#define DN_UPPER_CODE 245
#define MD_LOWER_CODE 375
#define MD_UPPER_CODE 385
#define SG_LOWER_CODE 375
#define SG_UPPER_CODE 385
#define DB_LOWER_CODE 245
#define DB_UPPER_CODE 255
*/
void getCubeV1Key()
{
  if (time_key > millis()) time_key = millis();
  if(millis()-time_key>init_key)
  {  
    uiKeyCodeSecond = uiKeyCodeFirst;
    int button2 = analogRead(A6);
    if(button2 > SG_LOWER_CODE && button2 < SG_UPPER_CODE)
      uiKeyCodeFirst=KEY_SINGLE;  // single
    else if(button2 > DB_LOWER_CODE && button2 < DB_UPPER_CODE)
      uiKeyCodeFirst=KEY_DOUBLE;  // double
    
    int button5 = analogRead(A7);
    if(button5 < LT_UPPER_CODE)  //left
      uiKeyCodeFirst=KEY_LEFT;
    else if(button5 > RT_LOWER_CODE && button5 < RT_UPPER_CODE) // right
      uiKeyCodeFirst=KEY_RIGHT;
    else if(button5 > UP_LOWER_CODE && button5 < UP_UPPER_CODE) // up
      uiKeyCodeFirst=KEY_UP;
    else if(button5 > DN_LOWER_CODE && button5 < DN_UPPER_CODE) // down
      uiKeyCodeFirst=KEY_DOWN;
    else if(button5 > MD_LOWER_CODE && button5 < MD_UPPER_CODE) // middle
      uiKeyCodeFirst=KEY_SELECT;
    else
      uiKeyCodeFirst=KEY_NONE;   
      
    /*
    Serial.print(button2);
    Serial.print(",");
    Serial.println(button5);
    delay(500);
    */
      
    if ( uiKeyCodeSecond == uiKeyCodeFirst )
      uiKeyCode = uiKeyCodeFirst;
    else
      uiKeyCode = KEY_NONE;        
  }
}  
