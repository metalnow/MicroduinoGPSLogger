#ifndef _OLED_VARS_H
#define _OLED_VARS_H

#include "define.h"

static uint8_t uiKeyCodeFirst = KEY_NONE;
static uint8_t uiKeyCodeSecond = KEY_NONE;
static uint8_t uiKeyCode = KEY_NONE;

static uint8_t refreshLCD = 1;

static int16_t  i2c_errors_count = 0;

static char line1[17],line2[17];

#define LCD_FLUSH {/*UartSendData();*/ delayMicroseconds(20000); }

#endif
