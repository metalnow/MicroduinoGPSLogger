#ifndef _OLED_VARS_H
#define _OLED_VARS_H

#include "define.h"

static uint8_t uiKeyCodeFirst = KEY_NONE;
static uint8_t uiKeyCodeSecond = KEY_NONE;
static uint8_t uiKeyCode = KEY_NONE;

static uint8_t refreshLCD = 1;

static uint8_t SysStage = STAGE_NONE;
static uint8_t LastSysStage = STAGE_NONE;

static int16_t  i2c_errors_count = 0;

#endif
