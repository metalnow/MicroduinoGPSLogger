typedef void (*menu_func_ptr)(void *);
typedef void (*attr_func_ptr)(void *);
typedef void (*attr_draw_func_ptr)(void *);

#define NULL_TIMER_FMT 0
#define NULL_DRAW_FMT 0
#define NULL_ATT_FMT 0
#define NULL_MENU_FMT 0
#define NULL_ATTR 0

struct menu_item_desc_t {
  menu_func_ptr fmt;
  const void * forward;
  const void * backward;
  const void * attr;
  timer_func_ptr timerFmt;
};

struct page_desc_t {
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

const char main_menu_item01 [] = "GPS Status";
const char main_menu_item02 [] = "ACM Planner";
const char main_menu_item03 [] = "Configuration";
const char main_menu_item04 [] = "About";

const char * main_string_table[] = 	   // change "string_table" name to suit
{   
  main_menu_item01,
  main_menu_item02,
  main_menu_item03,
  main_menu_item04 
};

static menu_item_desc_t main_item01 = { NULL_MENU_FMT, &__GPS, 0, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t main_item02 = { NULL_MENU_FMT, &__ACM, 0, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t main_item03 = { NULL_MENU_FMT, &__CFG, 0, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t main_item04 = { NULL_MENU_FMT, &__ABT, 0, NULL_ATTR, NULL_TIMER_FMT };

const menu_item_desc_t * const main_menu_ptr_table [] = {
  &main_item01,
  &main_item02,
  &main_item03,
  &main_item04,
};
#define MAINMENUMAX (sizeof(main_menu_ptr_table)/2)
// -----------

extern page_desc_t __ACM_ARM;
extern page_desc_t __ACM_TAKEOFF;
extern page_desc_t __ACM_FOLLOW;
extern page_desc_t __ACM_RTL;

const char acm_menu_item01 [] = "Arm";
const char acm_menu_item02 [] = "Takeoff";
const char acm_menu_item03 [] = "Follow me";
const char acm_menu_item04 [] = "RTL";

const char * acm_string_table[] = 	   // change "string_table" name to suit
{   
  acm_menu_item01,
  acm_menu_item02,
  acm_menu_item03,
  acm_menu_item04 
};

static menu_item_desc_t acm_item01 = { NULL_MENU_FMT, &__ACM_ARM,     &__MAIN, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t acm_item02 = { NULL_MENU_FMT, &__ACM_TAKEOFF, &__MAIN, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t acm_item03 = { NULL_MENU_FMT, &__ACM_FOLLOW,  &__MAIN, NULL_ATTR, NULL_TIMER_FMT };
static menu_item_desc_t acm_item04 = { NULL_MENU_FMT, &__ACM_RTL,     &__MAIN, NULL_ATTR, NULL_TIMER_FMT };

const menu_item_desc_t * const acm_menu_ptr_table [] = {
  &acm_item01,
  &acm_item02,
  &acm_item03,
  &acm_item04,
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

static menu_item_desc_t cfg_item01 = { NULL_MENU_FMT, 0, &__MAIN, &cfg_handle_item01, NULL_TIMER_FMT };
static menu_item_desc_t cfg_item02 = { NULL_MENU_FMT, 0, &__MAIN, &cfg_handle_item02, NULL_TIMER_FMT };
static menu_item_desc_t cfg_item03 = { NULL_MENU_FMT, 0, &__MAIN, &cfg_handle_item03, NULL_TIMER_FMT };

const menu_item_desc_t * const cfg_menu_ptr_table [] = {
  &cfg_item01,
  &cfg_item02,
  &cfg_item03,
};

#define CFGMENUMAX (sizeof(cfg_menu_ptr_table)/2)

// -----------
static menu_item_desc_t gps_item01 = { &__drawGPS, 0, &__MAIN, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const gps_menu_ptr_table [] = {
  &gps_item01,
};

static menu_item_desc_t abt_item01 = { &__drawABT, 0, &__MAIN, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const abt_menu_ptr_table [] = {
  &abt_item01,
};

static menu_item_desc_t acm_arm_item01 = { &__drawArm, 0, &__ACM, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const acm_arm_menu_ptr_table [] = {
  &acm_arm_item01,
};
static menu_item_desc_t acm_takeoff_item01 = { &__drawTakeoff, 0, &__ACM, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const acm_takeoff_menu_ptr_table [] = {
  &acm_takeoff_item01,
};
static menu_item_desc_t acm_follow_item01 = { &__drawFollow, 0, &__ACM, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const acm_follow_menu_ptr_table [] = {
  &acm_follow_item01,
};
static menu_item_desc_t acm_rtl_item01 = { &__drawRtl, 0, &__ACM, NULL_ATTR, NULL_TIMER_FMT };
const menu_item_desc_t * const acm_rtl_menu_ptr_table [] = {
  &acm_rtl_item01,
};

// -----------
page_desc_t __MAIN = {&main_menu_ptr_table[0], &main_string_table[0], 0, MAINMENUMAX};

page_desc_t __GPS  = {&gps_menu_ptr_table[0], 0, 0, 1};
page_desc_t __ACM  = {&acm_menu_ptr_table[0], &acm_string_table[0], 1, ACMMENUMAX};
page_desc_t __CFG  = {&cfg_menu_ptr_table[0], &cfg_string_table[0], 2, CFGMENUMAX};
page_desc_t __ABT  = {&abt_menu_ptr_table[0], 0, 3, 1};

page_desc_t __ACM_ARM     = {&acm_arm_menu_ptr_table[0], 0, 0, 1};
page_desc_t __ACM_TAKEOFF = {&acm_takeoff_menu_ptr_table[0], 0, 1, 1};
page_desc_t __ACM_FOLLOW  = {&acm_follow_menu_ptr_table[0], 0, 2, 1};
page_desc_t __ACM_RTL     = {&acm_rtl_menu_ptr_table[0], 0, 3, 1};
// ************************************************************************************************************
void __drawGPS( void * p )
{
  char buffer[5];

  LCDclear();
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

void __drawArm( void * p )
{
  DoArmDisarm();
}

void __drawTakeoff( void * p )
{
  DoTakeoff();
}

void __drawFollow( void * p )
{
  DoFlyHere(f_latitude, f_longitude, followAltitude);
}

void __drawRtl( void * p )
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
uint8_t menu_current = 0;
uint8_t last_key_code = KEY_NONE;
struct page_desc_t * currentPage = 0;
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
        const menu_item_desc_t* const item  = *(currentPage->data);
        if ( item->backward )
        {
          menu_current = currentPage->index;
          currentPage = (page_desc_t*)item->backward;
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
          currentPage = (page_desc_t *)item->forward;
          menu_current = 0;
        }
      }
        break;
    }
  }
  else
  {
    currentPage = &__MAIN;
  }
  
  refreshLCD = 1;
}

void _drawMenu()
{
  LCDclear();
  
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
        handle->drawFmt(0);
    }
    LCDprintChar(line2); // the value
    if ( i == menu_current )  LCDattributesOff();
    
    LCD_FLUSH;
  }
  LCDcrlf();  
}

void refreshScreen()
{
  if ( currentPage == 0 )
    return;
    
  if ( currentPage->total == 1 )
  {
    const menu_item_desc_t* const item  = *(currentPage->data);
    if ( item->fmt )
      item->fmt( 0 );
  }
  else 
    _drawMenu();
}
