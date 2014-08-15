typedef void (*menu_func_ptr)(void *);
typedef void (*attr_func_ptr)(void *);
typedef void (*attr_draw_func_ptr)(void *);

struct menu_item_desc_t {
  menu_func_ptr fmt;
  const void * forward;
  const void * backward;
  const void * attr;
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

static menu_item_desc_t main_item01 = { 0, &__GPS, 0, 0 };
static menu_item_desc_t main_item02 = { 0, &__ACM, 0, 0 };
static menu_item_desc_t main_item03 = { 0, &__CFG, 0, 0 };
static menu_item_desc_t main_item04 = { 0, &__ABT, 0, 0 };

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

static menu_item_desc_t acm_item01 = { 0, &__ACM_ARM, &__MAIN, 0 };
static menu_item_desc_t acm_item02 = { 0, &__ACM_TAKEOFF, &__MAIN, 0 };
static menu_item_desc_t acm_item03 = { 0, &__ACM_FOLLOW, &__MAIN, 0 };
static menu_item_desc_t acm_item04 = { 0, &__ACM_RTL, &__MAIN, 0 };

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

static menu_item_desc_t cfg_item01 = { 0, 0, &__MAIN, &cfg_handle_item01 };
static menu_item_desc_t cfg_item02 = { 0, 0, &__MAIN, &cfg_handle_item02 };
static menu_item_desc_t cfg_item03 = { 0, 0, &__MAIN, &cfg_handle_item03 };

const menu_item_desc_t * const cfg_menu_ptr_table [] = {
  &cfg_item01,
  &cfg_item02,
  &cfg_item03,
};

#define CFGMENUMAX (sizeof(cfg_menu_ptr_table)/2)

// -----------
static menu_item_desc_t gps_item01 = { &__drawGPS, 0, &__MAIN, 0 };
const menu_item_desc_t * const gps_menu_ptr_table [] = {
  &gps_item01,
};

static menu_item_desc_t abt_item01 = { &__drawABT, 0, &__MAIN, 0 };
const menu_item_desc_t * const abt_menu_ptr_table [] = {
  &abt_item01,
};

static menu_item_desc_t acm_arm_item01 = { &__drawArm, 0, &__ACM, 0 };
const menu_item_desc_t * const acm_arm_menu_ptr_table [] = {
  &acm_arm_item01,
};
static menu_item_desc_t acm_takeoff_item01 = { &__drawTakeoff, 0, &__ACM, 0 };
const menu_item_desc_t * const acm_takeoff_menu_ptr_table [] = {
  &acm_takeoff_item01,
};
static menu_item_desc_t acm_follow_item01 = { &__drawFollow, 0, &__ACM, 0 };
const menu_item_desc_t * const acm_follow_menu_ptr_table [] = {
  &acm_follow_item01,
};
static menu_item_desc_t acm_rtl_item01 = { &__drawRtl, 0, &__ACM, 0 };
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
void test()
{
  Serial.print("sizeof main_menu_ptr_table: ");
  Serial.println(sizeof(main_menu_ptr_table));
  Serial.print("sizeof acm_menu_ptr_table: ");
  Serial.println(sizeof(acm_menu_ptr_table));
  Serial.print("sizeof cfg_menu_ptr_table: ");
  Serial.println(sizeof(cfg_menu_ptr_table));
  
}

// ************************************************************************************************************
void __drawGPS( void * p )
{
    Serial.println("draw gps");  
}

void __drawABT( void * p )
{
      Serial.println("draw abt");  
}

void __drawArm( void * p )
{
    Serial.println("draw arm");  
}

void __drawTakeoff( void * p )
{
    Serial.println("draw takeoff");  
}

void __drawFollow( void * p )
{
    Serial.println("draw follow");  
}

void __drawRtl( void * p )
{
    Serial.println("draw rtl");
}

void __handleCfgGPS(void *)
{
  Serial.println("handle cfg gps");
}

void __handleCfgSdcard(void * )
{
  Serial.println("handle cfg sdcard");
}

void __handleCfgDebug(void * )
{
  Serial.println("handle cfg debug");
}

void __drawCfgGPS( void *)
{  
  Serial.println("CFGGPS");
}

void __drawCfgSdcard(void * )
{
  Serial.println("CFGSdcard");
}

void __drawCfgDebug(void * )
{
  Serial.println("CFGDebug");
}

// ************************************************************************************************************
uint8_t menu_current = 0;
uint8_t last_key_code = KEY_NONE;
struct page_desc_t * currentPage = 0;
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
        menu_current++;
        if ( menu_current >= currentPage->total )
          menu_current = 0;
        break;
      case KEY_UP:
        if ( menu_current <= 0 )
          menu_current = currentPage->total-1;
        else
          menu_current--;      
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
          Serial.println("do attr");
          item_attr_t * handle = (item_attr_t*)item->attr;
          handle->attrFmt(0);
        }
        else if ( item->forward )
        {
          Serial.println("move to next");
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
  Serial.println("drawMenu");
  
  for (uint8_t i = 0; i < currentPage->total; i++)
  {
    const menu_item_desc_t* const item  = *(currentPage->data+i);

    const char * const str_table = *(currentPage->str_table+i);
//    Serial.println(&str_table[0]);
       
    strcpy_P(line1,PSTR("          "));
    strcpy(line2,line1);
    strcpy(line1, &str_table[0]);
    
    if ( i == menu_current )  Serial.print(">  ");
    Serial.print(line1); // the label
    if ( item->attr )
    {
        item_attr_t * handle = (item_attr_t*)item->attr;
        handle->drawFmt(0);
    }
    Serial.print(line2);
    if ( i == menu_current )  Serial.print("  <");
    Serial.println();
    
  }
}

void refreshScreen()
{
  if ( currentPage > 0 )
  {  
    Serial.print("has page: ");
    Serial.println(currentPage->total);
    
    if ( currentPage->total == 1 )
    {
      const menu_item_desc_t* const item  = *(currentPage->data);
      if ( item->fmt )
        item->fmt( 0 );
    }
    else 
      _drawMenu();
  }
}

