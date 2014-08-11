#ifndef _DEFINE_H
#define _DEFINE_H

#define u8g_logo_width 128
#define u8g_logo_height 18

#define KEY_NONE   0
#define KEY_LEFT   1
#define KEY_RIGHT  2
#define KEY_SELECT 3
#define KEY_UP     4
#define KEY_DOWN   5
#define KEY_SINGLE 6
#define KEY_DOUBLE 7
#define KEY_PREV   KEY_LEFT
#define KEY_NEXT   KEY_RIGHT

#define STAGE_NONE 0
#define STAGE_INIT 1
#define STAGE_MENU 2
#define STAGE_GPS  3  // 
#define STAGE_CFG  4  // config
#define STAGE_ACM  5  // amp control
#define STAGE_ABT  6  // about
#define STAGE_QUESTION  7  // about
#define STAGE_MAIN_TOTAL STAGE_QUESTION
#define STAGE_ACM_FOLLOW STAGE_MAIN_TOTAL+1  // acm follow me
#define STAGE_ACM_STATUS STAGE_ACM_FOLLOW+1

#define MENU_ITEMS 4
#define MENU_GPS  0 
#define MENU_ACM  1 
#define MENU_CFG  2 
#define MENU_ABT  3 

#define MENU_ACM_ITEMS 4
#define MENU_ACM_ARM      0 
#define MENU_ACM_TAKEOFF  1 
#define MENU_ACM_FOLLOW   2 
#define MENU_ACM_RTL      3 

#define MENU_CFG_ITEMS 3
#define MENU_CFG_GPS      0 
#define MENU_CFG_SDCARD   1 
#define MENU_CFG_SERIAL   2 

#define MENU_QUESTION_ITEMS 2
#define MENU_QUESTION_NO    0 
#define MENU_QUESTION_YES   1 


#endif