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


#define SSD1306_128_64

#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32
  #error "At least one SSD1306 display must be specified in SSD1306.h"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#endif
