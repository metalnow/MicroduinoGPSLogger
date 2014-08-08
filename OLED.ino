
/*
font:
 u8g.setFont(u8g_font_7x13)
 u8g.setFont(u8g_font_fixed_v0r);
 u8g.setFont(u8g_font_chikitar);
 u8g.setFont(u8g_font_osb21);
 */

#include "define.h"

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


uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t last_key_code = KEY_NONE;

bool anyKey()
{
  if ( uiKeyCode != KEY_NONE )
  {
    last_key_code = uiKeyCode;
    return true;
  }    
  return false;
}

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

//Menus ===================================================



char *menu_strings[MENU_ITEMS] = { "GPS Status", "ACM Planner", "Configuration", "About" };
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

char *menu_acm_strings[MENU_ACM_ITEMS] = { "", "Takeoff", "Follow me", "RTL" };
void drawACMMenu() 
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_6x13);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ACM_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_acm_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    if ( i == MENU_ACM_ARM )
    {
      if ( mavLink.isArm() )
        u8g.drawStr(d, i*h, F("Disarm"));
      else
        u8g.drawStr(d, i*h, F("Arm"));
    }
    else if ( i == MENU_ACM_TAKEOFF )
    {
      if ( mavLink.isInAir() )
        u8g.drawStr(d, i*h, F("Land"));
      else
        u8g.drawStr(d, i*h, F("Takeoff"));      
    }
    else
      u8g.drawStr(d, i*h, menu_acm_strings[i]);
  }
}

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


char questionTitle[30];
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
  for( i = 0; i < MENU_ACM_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_question_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, h+i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, h+i*h, menu_question_strings[i]);
  }  
  
  u8g.drawStr(2, 0, questionTitle);
  
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
  u8g.print(acmAltitude);
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
    case STAGE_ACM:
      menu_current = MENU_ACM;   
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
    case STAGE_ACM_FOLLOW:
      question_state = 0;
      menu_current = MENU_QUESTION_YES;
      sprintf(questionTitle, "Leave FollowMe Mode?");
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
      case MENU_ACM:
        menu_current = 0;
        setNextSysStage(STAGE_ACM);
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
  else if ( SysStage == STAGE_ACM )
  {    
    switch(menu_current)
    {
      case MENU_ACM_ARM:
        mavLink.DoArmDisarm( !mavLink.isArm() );
      break;
      case MENU_ACM_FOLLOW:
        setNextSysStage(STAGE_ACM_FOLLOW);
        if ( !b_read_gps )
        {
          b_read_gps = true;
          writeCFG();
        }
      break;
      case MENU_ACM_TAKEOFF:
        if ( mavLink.isInAir() )
          mavLink.DoLand();
        else
          mavLink.DoTakeoff();
      break;
      case MENU_ACM_RTL:
        mavLink.DoRTL();
      break;
    }
    
    if ( menu_current != MENU_ACM_FOLLOW )
      setNextSysStage(STAGE_ACM_STATUS);
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
    || SysStage == STAGE_ACM )
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

// acm handle ===================================================
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
      acmAltitude++;
      if ( acmAltitude > 30 )
        acmAltitude = 30;
      break;
    case KEY_DOWN:
      menu_redraw_required = 1;
      acmAltitude--;
      if ( acmAltitude < 1 )
        acmAltitude = 1;
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


