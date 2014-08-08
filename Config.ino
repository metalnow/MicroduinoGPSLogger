#include "define.h"

void readCFG()
{
  return;
  
  
  uint8_t eeprom_addr = CFG_EEPROM_ADDR;
  b_write_sdcard = EEPROM.read(eeprom_addr);
  eeprom_addr++;
  b_read_gps = EEPROM.read(eeprom_addr);
  eeprom_addr++;
  b_debug_serail = EEPROM.read(eeprom_addr);
  eeprom_addr++;
}

void writeCFG()
{
  return;
  
  
  uint8_t eeprom_addr = CFG_EEPROM_ADDR;
  EEPROM.write(eeprom_addr, b_write_sdcard);
  eeprom_addr++;
  EEPROM.write(eeprom_addr, b_read_gps);
  eeprom_addr++;
  EEPROM.write(eeprom_addr, b_debug_serail);
  eeprom_addr++;  
}