#include "LCD_I2C.h"

#define LCD_light         0b00001000
#define E                 0b00000100
#define RS_command        0b00000000
#define RS_data           0b00000001
#define I2C_course_write  0b00000000
#define I2C_course_read   0b00000001  


//...................................................... KONSTRUKTOR ............................................................
LCD_I2C::LCD_I2C(uint8_t address): I2C_address(address)
{
  //Dioida wbudowana w arduino  nano, do obsługi błędów
  DDRB |= (1<<PB5);
  PORTB &= ~(1<<PB5);

  //Inicjalizacja konwertera I2C i wyświetlacza LCD
  I2C_init();
  LCD_init();
}



//.......................................................... LCD ................................................................
void LCD_I2C::E_pulse(uint8_t data, uint8_t rs){
  //E=1 -> wyświetlacz odbiera dane
  uint8_t data_a = data | LCD_light | E | rs;
  I2C_writeData(data_a);
  _delay_us(1);
  
  //E=0 -> wyświetlacz przerwarza odebrane dane;
  uint8_t data_b = data | LCD_light | rs;
  I2C_writeData(data_b);
  _delay_ms(2);
}

void LCD_I2C::LCD_command(uint8_t com){
  I2C_start();
  
  //adres urządzenia i bit kierunku
  I2C_firstByte();
  
  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  uint8_t old_bits = (com & 0b11110000);
  E_pulse(old_bits, RS_command);
  
  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  uint8_t young_bits = ((com<<4) & 0b11110000);
  E_pulse(young_bits, RS_command);
  
  I2C_stop();
  _delay_us(2000);
}

void LCD_I2C::LCD_sign(uint8_t sign){
  I2C_start();

  //adres urządzenia i bit kierunku
  I2C_firstByte();

  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  uint8_t old_bits = (sign & 0b11110000);
  E_pulse(old_bits, RS_data);

  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  uint8_t young_bits = ((sign<<4) & 0b11110000);
  E_pulse(young_bits, RS_data);

  I2C_stop();
   _delay_us(50);
  
}

void LCD_I2C::LCD_init(){
  _delay_ms(50);
  
  //Display/cursor home - Ustawia adres DDRAM 0 w liczniku adresów. Przywraca również przesunięcie wyświetlacza do pierwotnej pozycji. Zawartość pamięci DDRAM pozostaje niezmieniona
  LCD_command(0b00000010);
  _delay_us(2000);
 
  //Function set - przesył, ilosc linijek, czcionka
  LCD_command(0b00101000);
  _delay_us(50);

  //Display ON/OFF - - Włącza/wyłącza cały wyświetlacz, włącza/wyłącza kursor i znak pozycji kursora.
  LCD_command(0b00001100);
  _delay_us(50);

  //Entry mode set - Ustawia kierunek ruchu kursora i określa przesunięcie wyświetlania. Operacje te są wykonywane podczas zapisu i odczytu danych.
  LCD_command(0b00000110);
  _delay_us(50);

  //Display cursor shift - Przesuwa kursor i przesuwa ekran bez zmiany zawartości pamięci DDRAM.
  LCD_command(0b00010100);
  _delay_us(50);

  //Display clear - czyści wyświetlacz
  LCD_command(0b00000001);
  
}

void LCD_I2C::LCD_setCursor(uint8_t row, uint8_t col){
   uint8_t pos;
    
    if (row == 0) {
        pos = 0x80 + col; // Pierwsza linia
    } else if (row == 1) {
        pos = 0xC0 + col; // Druga linia
    } else {
        return; // Nieprawidłowy wiersz
    }
  
    LCD_command(pos);
  
}

void LCD_I2C::LCD_clear(){
  LCD_command(0b00000001);
  
}

void LCD_I2C::LCD_print(const char *array){
  for(int i=0; i<strlen(array); i++)
  {
      LCD_sign(array[i]); 
  }
  
}



//......................................................... I2C .................................................................
void LCD_I2C::I2C_init(){
  //SCL frequency - 100000Hz
  //scl_freq = 16000000/(16+2(72)*1)= 100000Hz
  //TWBR = (f_cpu/(f_scl)-16)/(2*N)
  TWBR = 0b01001000; //72

  // prescaler = 1
  TWSR &= ~(1<<TWPS1);
  TWSR &= ~(1<<TWPS0);
  
}

void LCD_I2C::I2C_start(){
  //Master Transmitter Mode
  //start
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  
  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że warunek START został przesłany
  while (!(TWCR & (1<<TWINT)))
  {}

  //Warunek Start został przesłany
  if ((TWSR & 0xF8) != 0x08)
  {
    PORTB |= (1<<PB5);  
  }
  else
  {
    PORTB &= ~(1<<PB5);
  }
  
}

void LCD_I2C::I2C_firstByte(){
  //adres urządzenia i bit kierunku (0, bo zapis danych z master do slave)
  uint8_t address_ = (I2C_address<<1)|(I2C_course_write); 
  TWDR = address_;
  TWCR = (1<<TWINT) | (1<<TWEN);

  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
  while (!(TWCR & (1<<TWINT)))
  {}

  //SLA+W została przekazana, ACK został odebrany
  if ((TWSR & 0xF8) != 0x18)
  {
    PORTB |= (1<<PB5);  
  }
  else
  {
    PORTB &= ~(1<<PB5);
  }
  
}

void LCD_I2C::I2C_writeData(uint8_t DATA){
  //Załaduj DANE do rejestru TWDR. Wyczyść bit TWINT w TWCR, aby rozpocząć transmisję danych
  TWDR = DATA;
  TWCR = (1<<TWINT) | (1<<TWEN);

  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
  while (!(TWCR & (1<<TWINT)))
  {}

  //Bajt danych został przesłany, ACK został odebrany
  if ((TWSR & 0xF8) != 0x28)
  {
    PORTB |= (1<<PB5);  
  }
  else
  {
    PORTB &= ~(1<<PB5);
  }
  
}

void LCD_I2C::I2C_stop(){
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
 while(TWCR & (1<<TWSTO)) 
  {}
  
}
