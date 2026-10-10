#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include "gpio.h"

void lcd_init(uint8_t port,
              uint8_t rs_pin,
              uint8_t en_pin,
              uint8_t d4_pin,
              uint8_t d5_pin,
              uint8_t d6_pin,
              uint8_t d7_pin);

void lcd_command(uint8_t command);
void lcd_data(uint8_t data);

void lcd_clear(void);
void lcd_home(void);

void lcd_set_cursor(uint8_t row, uint8_t column);

void lcd_print(const char *text);
void lcd_print_number(uint16_t number);

#endif