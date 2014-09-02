// ************************************************************************************************************
uint8_t menu_current = 0;
uint8_t last_key_code = KEY_NONE;
struct page_desc_t * currentPage = 0;
boolean _drawFail = false;
// ************************************************************************************************************

typedef void (*draw_func_ptr)(void *);
typedef void (*menu_func_ptr)(void *);
typedef void (*attr_func_ptr)(void *);
typedef void (*attr_draw_func_ptr)(void *);
typedef void (*timer_func_ptr)(void *);

#define NULL_DRAW_FMT 0
#define NULL_ATT_FMT 0
#define NULL_MENU_FMT 0
#define NULL_ATTR 0

#define DEFAULT_MENU_FMT &_drawMenu

struct menu_item_desc_t {
  const void * forward;
  const void * attr;
};

struct page_desc_t {
  draw_func_ptr drawFmt;
  timer_func_ptr timerFmt;
  const void * backward;
  const menu_item_desc_t * const * data;
  const char * const * str_table;
  uint8_t index;
  uint8_t total;
};

struct item_attr_t {
  attr_func_ptr attrFmt;
  attr_draw_func_ptr drawFmt;
};

extern page_desc_t __MAIN;
extern page_desc_t __GPS;
extern page_desc_t __ACM;
extern page_desc_t __CFG;
extern page_desc_t __ABT;

extern page_desc_t __ACM_FOLLOW;

const char main_menu_item01 [] = "GPS Status";
const char main_menu_item02 [] = "ArduCopterMega";
const char main_menu_item03 [] = "Configuration";
const char main_menu_item04 [] = "About";

const char * main_string_table[] = 	   // change "string_table" name to suit
{   
  main_menu_item01,
  main_menu_item02,
  main_menu_item03,
  main_menu_item04 
};

static menu_item_desc_t main_item01 = { &__GPS, NULL_ATTR };
static menu_item_desc_t main_item02 = { &__ACM, NULL_ATTR };
static menu_item_desc_t main_item03 = { &__CFG, NULL_ATTR };
static menu_item_desc_t main_item04 = { &__ABT, NULL_ATTR };

const menu_item_desc_t * const main_menu_ptr_table [] = {
  &main_item01,
  &main_item02,
  &main_item03,
  &main_item04,
};
#define MAINMENUMAX (sizeof(main_menu_ptr_table)/2)

// -----------
const char acm_menu_item01 [] = "Arm/Disarm";
const char acm_menu_item02 [] = "Takeoff";
const char acm_menu_item03 [] = "Follow me";
const char acm_menu_item04 [] = "Loiter";
const char acm_menu_item05 [] = "RTL";

const char * acm_string_table[] = 	   // change "string_table" name to suit
{   
  acm_menu_item01,
  acm_menu_item02,
  acm_menu_item03,
  acm_menu_item04, 
  acm_menu_item05
};

static item_attr_t acm_handle_item01 = { &__handleArm,     NULL_DRAW_FMT };
static item_attr_t acm_handle_item02 = { &__handleTakeoff, NULL_DRAW_FMT };
//static item_attr_t acm_handle_item03 = { &__handleFollow,  NULL_DRAW_FMT };
static item_attr_t acm_handle_item04 = { &__handleLoiter,  NULL_DRAW_FMT };
static item_attr_t acm_handle_item05 = { &__handleRtl,     NULL_DRAW_FMT };

static menu_item_desc_t acm_item01 = { NULL_MENU_FMT, &acm_handle_item01 };
static menu_item_desc_t acm_item02 = { NULL_MENU_FMT, &acm_handle_item02 };
static menu_item_desc_t acm_item03 = { &__ACM_FOLLOW, NULL_ATTR };
static menu_item_desc_t acm_item04 = { NULL_MENU_FMT, &acm_handle_item04 };
static menu_item_desc_t acm_item05 = { NULL_MENU_FMT, &acm_handle_item05 };

const menu_item_desc_t * const acm_menu_ptr_table [] = {
  &acm_item01,
  &acm_item02,
  &acm_item03,
  &acm_item04,
  &acm_item05,
};
#define ACMMENUMAX (sizeof(acm_menu_ptr_table)/2)

// -----------
const char cfg_menu_item01 [] = "GPS Read";
const char cfg_menu_item02 [] = "SDCard log";
const char cfg_menu_item03 [] = "Serial Debug";

const char * cfg_string_table[] = 	   // change "string_table" name to suit
{   
  cfg_menu_item01,
  cfg_menu_item02,
  cfg_menu_item03 
};

static item_attr_t cfg_handle_item01 = { &__handleCfgGPS, &__drawCfgGPS };
static item_attr_t cfg_handle_item02 = { &__handleCfgSdcard, &__drawCfgSdcard };
static item_attr_t cfg_handle_item03 = { &__handleCfgDebug, &__drawCfgDebug };

static menu_item_desc_t cfg_item01 = { NULL_MENU_FMT, &cfg_handle_item01 };
static menu_item_desc_t cfg_item02 = { NULL_MENU_FMT, &cfg_handle_item02 };
static menu_item_desc_t cfg_item03 = { NULL_MENU_FMT, &cfg_handle_item03 };

const menu_item_desc_t * const cfg_menu_ptr_table [] = {
  &cfg_item01,
  &cfg_item02,
  &cfg_item03,
};

#define CFGMENUMAX (sizeof(cfg_menu_ptr_table)/2)

// -----------
page_desc_t __MAIN = {DEFAULT_MENU_FMT, 0, 0, &main_menu_ptr_table[0], &main_string_table[0], 0, MAINMENUMAX};

page_desc_t __GPS  = {&__drawGPS,       &__drawGPSInfo, &__MAIN, 0,                      0,                    0, 0};
page_desc_t __ACM  = {&__drawACMMenu,   &__drawACMInfo, &__MAIN, &acm_menu_ptr_table[0], &acm_string_table[0], 1, ACMMENUMAX};
page_desc_t __CFG  = {DEFAULT_MENU_FMT, 0,              &__MAIN, &cfg_menu_ptr_table[0], &cfg_string_table[0], 2, CFGMENUMAX};
page_desc_t __ABT  = {&__drawABT,       0,              &__MAIN, 0,                      0,                    3, 0};

page_desc_t __ACM_FOLLOW  = {&__drawFollow, &__handleFollow, &__ACM, 0, 0, 2, 0};

// ************************************************************************************************************
void __drawGPSInfo(void *)
{
  char buffer[5];
  LCDsetLine(1);
  LCDprintChar("Speed:");
  if ( STA )
  {
    String speed = String(i_Speed[1]) + "." + String(i_Speed[0]);
    speed.toCharArray(line1, speed.length());
    LCDprintChar(line1);    
  }
  else
    LCDprintChar("N/A");
    
  LCDsetLine(2);
  LCDprintChar("Lat.: ");  
  LCDprint(c_lat);
  LCDprint(' ');
  LCDprintChar(dtostrf(f_latitude, 1, 4, buffer));  
  LCDsetLine(3);
  LCDprintChar("Lon.: ");  
  LCDprint(c_lon);
  LCDprint(' ');
  LCDprintChar(dtostrf(f_longitude, 1, 4, buffer));  

  LCDsetLine(4);
  String time = "20" + String(idate[0]) + "-" + String(idate[1]) + "-" + String(idate[2]) + " "
    + String(itime[0]) + ":" + String(itime[1]) + ":" + String(itime[2]);
  time.toCharArray(line1, time.length());
  LCDprintChar(line1);    

  LCDsetLine(6);  
  strcpy_P(line1,PSTR("Sat.:"));
  LCDprintChar(line1);  
  String sat = String(i_satellites);
  time.toCharArray(line1, sat.length());
  LCDprintChar(line1);    
  
  LCDprintChar("   ELE.:");  
  LCDprintChar(dtostrf(f_Height, 1, 4, buffer));  
  LCDprintChar(" ");   
}

void __drawGPS( void * p )
{
  LCDclear();
  __drawGPSInfo(p);
  LCDcrlf();
}

void __drawABT( void * p )
{
  LCDclear();
  LCDsetLine(3);
  LCDprintChar("=====================");    
  LCDsetLine(4);
  LCDprintChar("       Built by");
  LCDsetLine(5);
  LCDprintChar("      Shaune Jan");  
  LCDsetLine(6);
  LCDprintChar("=====================");    
}

// 0:Stabilize,1:Acro,2:AltHold,3:Auto,4:Guided,5:Loiter,6:RTL,7:Circle,9:Land,10:OF_Loiter,11:Drift,13:Sport
prog_char string_0[] PROGMEM = "Stabilize"; 
prog_char string_1[] PROGMEM = "Acro";
prog_char string_2[] PROGMEM = "SAltHold";
prog_char string_3[] PROGMEM = "Auto";
prog_char string_4[] PROGMEM = "Guided";
prog_char string_5[] PROGMEM = "Loiter";
prog_char string_6[] PROGMEM = "RTL";
prog_char string_7[] PROGMEM = "Circle";
prog_char string_8[] PROGMEM = "Pos Hold";
prog_char string_9[] PROGMEM = "Land";
prog_char string_10[] PROGMEM = "OF_Loiter";
prog_char string_11[] PROGMEM = "Drift";
prog_char string_12[] PROGMEM = "Loiter_FW";
prog_char string_13[] PROGMEM = "Sport";
prog_char string_14[] PROGMEM = "Unknow";
prog_char string_15[] PROGMEM = "Autotune";
prog_char string_16[] PROGMEM = "Hybrid";

PROGMEM const char *mode_string_table[] = {   
  string_0, string_1, string_2, string_3, string_4, string_5,
  string_6, string_7, string_8, string_9, string_10, string_11,
  string_12, string_13, string_14, string_15, string_16};
 
void __drawACMInfo( void * p )
{
  if ( !mavlink_active )
  {
    if ( !_drawFail)
    {
      _drawFail = true;
      strcpy_P(line1,PSTR("                     "));
      LCDsetLine(1);
      LCDprintChar(line1);
      LCDsetLine(2);
      LCDprintChar(line1);
      LCDsetLine(3);
      LCDprintChar(line1);
      LCDsetLine(4);
      LCDprintChar(line1);
      LCDsetLine(5);
      LCDprintChar(line1);
      
      LCDsetLine(3);
      LCDattributesReverse();
      strcpy_P(line1,PSTR(" No Drone Connected! "));
      LCDprintChar(line1);    
      LCDattributesOff();
    }
    
    return;
  }
  
  // Motors
  LCDsetLine(1);
  LCDprintChar("Motors:");
  if ( motor_armed )
    LCDprintChar("Armed");    
  else
    LCDprintChar("Disarmed");    

  // Flight Mode
  LCDsetLine(2);
  LCDprintChar("Mode:");  
  strcpy_P(line1, (char*)pgm_read_word(&(mode_string_table[custom_mode])));
  LCDprintChar( line1 );
  
  // GPS
  LCDsetLine(3);
  LCDprintChar("GPS:");
  if ( apm_fix_type == 2 )
    LCDprintChar("2D Lock");    
  else if ( apm_fix_type == 3 )
    LCDprintChar("3D Lock");
  else
    LCDprintChar("NoFix");
  
  // Battery
  LCDsetLine(4);
  LCDprintChar("Bat.:");  
  LCDprintChar(dtostrf(vbat_A, 1, 4, line1));  
  LCDprintChar(" v");
  LCDprintChar(dtostrf(curr_A, 1, 4, line1));  
  LCDprintChar(" A");  
  
  strcpy_P(line1,PSTR("           "));  
  LCDprintChar(line1);
  
  // Leash GPS
  LCDsetLine(5);
  LCDprintChar("                     ");  
}

// draw acm menu in line 6~8
void __drawACMMenu( void * )
{
  for (uint8_t i = 0; i < currentPage->total; i++)
  {
    const menu_item_desc_t* const item  = *(currentPage->data+i);
    const char * const str_table = *(currentPage->str_table+i);    
    strcpy_P(line1,PSTR("          "));
    strcpy(line2,line1);
    strcpy(line1, &str_table[0]);
    
    if ( ( menu_current - 1 == i )
      || (i == currentPage->total - 1 && menu_current == 0) )
      LCDsetLine(6);
    else if ( menu_current == i )
      LCDsetLine(7);
    else if ( ( menu_current + 1 == i )
      || (i == 0 && menu_current == currentPage->total - 1) )
      LCDsetLine(8);
    else
      continue;
      
    if ( i == menu_current )  LCDattributesReverse();
    LCDprintChar(line1); // the label
    if ( item->attr )
    {
        item_attr_t * handle = (item_attr_t*)item->attr;
        if ( handle->drawFmt )
          handle->drawFmt(0);
    }
    LCDprintChar(line2); // the value
    if ( i == menu_current )  LCDattributesOff();
    
    LCD_FLUSH;
  }
  
}

void __drawFollow( void * p )
{
  LCDsetLine(7);

  LCDprintChar("Alt. set: ");
  LCDprintChar(dtostrf(followAltitude, 1, 0, line1)); 
  LCDprintChar(" m");  
  strcpy_P(line1,PSTR("           "));  
  LCDprintChar(line1);
}

void __handleArm( void * p )
{
  DoArmDisarm();
}

void __handleTakeoff( void * p )
{
  DoTakeoff();
}

void __handleFollow( void * p )
{
  if (STA)
  {
    DoFlyHere(f_latitude, f_longitude, followAltitude);
    __drawACMInfo(p);
  }
  else
  {
    // Leash GPS
    LCDsetLine(5);
    LCDprintChar("==No Leash GPS Lock==");    
  }
}

void __handleLoiter( void * p )
{
  DoLoiter();
}

void __handleRtl( void * p )
{
  DoRTL();
}

void __handleCfgGPS(void *)
{
  b_read_gps = !b_read_gps;
}

void __handleCfgSdcard(void * )
{
  b_write_sdcard = !b_write_sdcard;
}

void __handleCfgDebug(void * )
{
  b_debug_serail = !b_debug_serail;
}

void __drawCfgGPS( void *)
{  
  line2[6] = b_read_gps ? 'O' : 'X';
}

void __drawCfgSdcard(void * )
{
  line2[6] = b_write_sdcard ? 'O' : 'X';
}

void __drawCfgDebug(void * )
{
  line2[6] = b_debug_serail ? 'O' : 'X';
}


// ************************************************************************************************************


void setCurrentPage( struct page_desc_t * page )
{
  LCDclear();
  _drawFail = false;
  currentPage = page;
  timer_fmt = currentPage->timerFmt;
}

void doKeyupHandle()
{
  
  if ( currentPage == &__ACM_FOLLOW )
  {
    followAltitude++;
    if ( followAltitude > 30 )
      followAltitude = 30;
  }
  else
  {
    if ( menu_current <= 0 )
      menu_current = currentPage->total-1;
    else
      menu_current--;    
  }
  
}

void doKeydownHandle()
{
  
  if ( currentPage == &__ACM_FOLLOW )
  {
    followAltitude--;
    if ( followAltitude < 5 )
      followAltitude = 5;    
  }
  else
  {
    menu_current++;
    if ( menu_current >= currentPage->total )
      menu_current = 0;      
  }
}

void updateMenu() 
{
  if ( uiKeyCode == KEY_NONE || last_key_code == uiKeyCode ) 
  {
    last_key_code = KEY_NONE;
    return;
  }
  last_key_code = uiKeyCode;
  
  if ( currentPage > 0 )
  {  
    switch ( uiKeyCode ) 
    {
      case KEY_DOWN:
        doKeydownHandle();
        break;
      case KEY_UP:
        doKeyupHandle();
        break;
      case KEY_PREV:
      {
        if ( currentPage->backward )
        {
          menu_current = currentPage->index;
          setCurrentPage((page_desc_t *)currentPage->backward);          
        }
      }
        break;
      case KEY_NEXT:
      case KEY_SELECT:
      {
        const menu_item_desc_t* const item  = *(currentPage->data+menu_current);
        if ( item->attr )
        {
          item_attr_t * handle = (item_attr_t*)item->attr;
          handle->attrFmt(0);
        }
        else if ( item->forward )
        {
          setCurrentPage((page_desc_t *)item->forward);
          menu_current = 0;
        }
      }
        break;
    }
  }
  else
  {
    menu_current = 0;
    setCurrentPage(&__MAIN);
  }
  
  refreshLCD = 1;
}

void _drawMenu( void * )
{    
  for (uint8_t i = 0; i < currentPage->total; i++)
  {
    const menu_item_desc_t* const item  = *(currentPage->data+i);
    const char * const str_table = *(currentPage->str_table+i);    
    strcpy_P(line1,PSTR("          "));
    strcpy(line2,line1);
    strcpy(line1, &str_table[0]);
    
    LCDsetLine(i+1);
    
    if ( i == menu_current )  LCDattributesReverse();
    LCDprintChar(line1); // the label
    if ( item->attr )
    {
        item_attr_t * handle = (item_attr_t*)item->attr;
        if ( handle->drawFmt )
          handle->drawFmt(0);
    }
    LCDprintChar(line2); // the value
    if ( i == menu_current )  LCDattributesOff();
    
    LCD_FLUSH;
  }
  //LCDcrlf();  
}

void refreshScreen()
{
  if ( currentPage == 0 )
    return;

  if ( currentPage->drawFmt )
   currentPage->drawFmt( 0 ); 
}
