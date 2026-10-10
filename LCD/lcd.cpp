#include "lcd.h"

static uint8_t lcd_port;

static uint8_t rs;
static uint8_t en;

static uint8_t d4;
static uint8_t d5;
static uint8_t d6;
static uint8_t d7;


/* LCD timing delay */
static void lcd_delay(void)
{
    volatile uint16_t i;

    for(i = 0; i < 1000; i++)
    {
    }
}


/* Send 4 bits to LCD */
static void lcd_write_nibble(uint8_t value)
{
    pin_write(lcd_port, d4, (value >> 0) & 1);
    pin_write(lcd_port, d5, (value >> 1) & 1);
    pin_write(lcd_port, d6, (value >> 2) & 1);
    pin_write(lcd_port, d7, (value >> 3) & 1);

    /* Enable pulse */
    pin_write(lcd_port, en, HIGH);

    lcd_delay();

    pin_write(lcd_port, en, LOW);

    lcd_delay();
}


/* Initialize LCD in 4-bit mode */
void lcd_init(uint8_t port,
              uint8_t rs_pin,
              uint8_t en_pin,
              uint8_t d4_pin,
              uint8_t d5_pin,
              uint8_t d6_pin,
              uint8_t d7_pin)
{
    lcd_port = port;

    rs = rs_pin;
    en = en_pin;

    d4 = d4_pin;
    d5 = d5_pin;
    d6 = d6_pin;
    d7 = d7_pin;


    /* Configure LCD pins as OUTPUT */
    pin_config(lcd_port, rs, OUTPUT);
    pin_config(lcd_port, en, OUTPUT);

    pin_config(lcd_port, d4, OUTPUT);
    pin_config(lcd_port, d5, OUTPUT);
    pin_config(lcd_port, d6, OUTPUT);
    pin_config(lcd_port, d7, OUTPUT);


    /* Initial state */
    pin_write(lcd_port, rs, LOW);
    pin_write(lcd_port, en, LOW);

    lcd_delay();


    /* HD44780 4-bit initialization */
    lcd_write_nibble(0x03);
    lcd_delay();

    lcd_write_nibble(0x03);
    lcd_delay();

    lcd_write_nibble(0x03);
    lcd_delay();

    lcd_write_nibble(0x02);
    lcd_delay();


    /* 4-bit mode, 2 lines, 5x8 font */
    lcd_command(0x28);

    /* Display ON, cursor OFF */
    lcd_command(0x0C);

    /* Cursor moves right */
    lcd_command(0x06);

    /* Clear display */
    lcd_command(0x01);

    lcd_delay();
}


/* Send command */
void lcd_command(uint8_t command)
{
    /* RS = 0 means command */
    pin_write(lcd_port, rs, LOW);

    /* Upper 4 bits */
    lcd_write_nibble((command >> 4) & 0x0F);

    /* Lower 4 bits */
    lcd_write_nibble(command & 0x0F);

    lcd_delay();
}


/* Send data/character */
void lcd_data(uint8_t data)
{
    /* RS = 1 means data */
    pin_write(lcd_port, rs, HIGH);

    /* Upper 4 bits */
    lcd_write_nibble((data >> 4) & 0x0F);

    /* Lower 4 bits */
    lcd_write_nibble(data & 0x0F);

    lcd_delay();
}


/* Clear LCD */
void lcd_clear(void)
{
    lcd_command(0x01);
    lcd_delay();
}


/* Return cursor to home */
void lcd_home(void)
{
    lcd_command(0x02);
    lcd_delay();
}


/* Set cursor position */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if(row == 0)
        address = 0x00 + column;
    else
        address = 0x40 + column;

    lcd_command(0x80 | address);
}


/* Print string */
void lcd_print(const char *text)
{
    while(*text != '\0')
    {
        lcd_data(*text);
        text++;
    }
}


/* Print number */
void lcd_print_number(uint16_t number)
{
    char buffer[6];
    uint8_t i = 0;

    if(number == 0)
    {
        lcd_data('0');
        return;
    }

    while(number > 0)
    {
        buffer[i] = (number % 10) + '0';
        number = number / 10;
        i++;
    }

    while(i > 0)
    {
        i--;
        lcd_data(buffer[i]);
    }
}