#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "LCD_I2C.h"

int main()
{ 
  LCD_I2C lcd = LCD_I2C(0x27);
 
  lcd.LCD_setCursor(0,0);
  lcd.LCD_print("Hello World!");

  while(1)
  {}
  
  return 0;
}
