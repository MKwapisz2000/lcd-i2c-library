//  Biblioteka obsługująca wyświetlacz LCD ze sterownikiem HD44780, za pomocą konwertera I2C z modułem PCF8574.
//  Ustawienia przystsowane do trybu Master Tansmiter, zapis danych z master do slave.

#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

class LCD_I2C {
  public:
  LCD_I2C(uint8_t address);
  
  void LCD_setCursor(uint8_t row, uint8_t col);
  void LCD_clear();
  void LCD_print(const char *array);
  
  private:
  uint8_t I2C_address;

  void E_pulse(uint8_t data, uint8_t rs); 
  void LCD_command(uint8_t com);
  void LCD_sign(uint8_t sign);
  void LCD_init();

  void I2C_init();
  void I2C_start();
  void I2C_firstByte();
  void I2C_writeData(uint8_t DATA);
  void I2C_stop();
  
};


#endif
