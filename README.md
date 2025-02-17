# lcd-i2c-library

**Description - English**

The LCD_I2C library provides an interface to control an HD44780-based LCD via an I2C converter using the PCF8574 module. It operates in Master Transmitter mode, meaning data is sent from the microcontroller (master) to the LCD module (slave).

Main Features:

- Control LCD via I2C

- Display text on LCD

- Clear the screen

- Set cursor position (row and column)

- Use HD44780 controller registers in 4-bit mode

Hardware Requirements:

- Arduino Nano / Uno / Mega or any I2C-compatible microcontroller

- LCD 16x2 or 20x4 with I2C interface (PCF8574)

- 4.7kΩ pull-up resistors on SDA and SCL (optional if not built-in)

Installation & Usage:

- Copy LCD_I2C.h and LCD_I2C.cpp files into your project folder.

- Include the header file in your main.ino:

      #include "LCD_I2C.h"
    
      LCD_I2C lcd(0x27); // I2C address may vary, check with an I2C scanner

- Initialize the LCD in setup():
  
      void setup() {
          lcd.LCD_clear();
          lcd.LCD_setCursor(0, 0);
          lcd.LCD_print("Hello, World!");
      }

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Opis - Polski**

Biblioteka LCD_I2C służy do obsługi wyświetlacza LCD ze sterownikiem HD44780 za pomocą konwertera I2C z modułem PCF8574. Umożliwia komunikację w trybie Master Transmitter, co oznacza, że dane są zapisywane z mikrokontrolera (master) do wyświetlacza (slave).

Główne funkcjonalności:

- Obsługa wyświetlacza LCD przez magistralę I2C

- Wyświetlanie tekstu na LCD

- Czyszczenie ekranu

- Ustawianie kursora na wybranym wierszu i kolumnie

- Wykorzystanie rejestrów sterownika HD44780 w trybie 4-bitowym

Wymagania sprzętowe:

- Arduino Nano / Uno / Mega lub inne kompatybilne urządzenie z I2C

- Wyświetlacz LCD 16x2 lub 20x4 z konwerterem I2C (PCF8574)

- Rezystory podciągające 4.7kΩ na SDA i SCL (opcjonalnie, jeśli nie ma wbudowanych)

Instalacja i użycie:

- Skopiuj pliki LCD_I2C.h i LCD_I2C.cpp do folderu swojego projektu.

- W main.ino dołącz plik nagłówkowy

      #include "LCD_I2C.h"
    
      LCD_I2C lcd(0x27); // I2C address may vary, check with an I2C scanner

- Zainicjalizuj wyświetlacz w setup():

      void setup() {
          lcd.LCD_clear();
          lcd.LCD_setCursor(0, 0);
          lcd.LCD_print("Hello, World!");
      }
