#include "two_7segment.h"

/* Segment codes for common-cathode display */
static const uint8_t sevensegment_code[10] =
{
    0x3F,   /* 0 */
    0x06,   /* 1 */
    0x5B,   /* 2 */
    0x4F,   /* 3 */
    0x66,   /* 4 */
    0x6D,   /* 5 */
    0x7D,   /* 6 */
    0x07,   /* 7 */
    0x7F,   /* 8 */
    0x6F    /* 9 */
};

static uint8_t seg2_port;
static uint8_t digit_port;

static uint8_t digit1;
static uint8_t digit2;

static uint8_t tens_digit;
static uint8_t units_digit;

static uint8_t current_digit;

/* Initialize segment lines and digit-enable lines */
void sevensegment2_init(uint8_t segment_port,
                        uint8_t select_port,
                        uint8_t select_digit1_pin,
                        uint8_t select_digit2_pin)
{
    seg2_port  = segment_port;
    digit_port = select_port;

    digit1 = select_digit1_pin;
    digit2 = select_digit2_pin;

    /* Segment pins -> output */
    port_config(seg2_port, 0xFF);

    /* Digit select pins -> output */
    pin_config(digit_port, digit1, OUTPUT);
    pin_config(digit_port, digit2, OUTPUT);

    /* Both digits OFF initially */
    pin_write(digit_port, digit1, LOW);
    pin_write(digit_port, digit2, LOW);

    port_write(seg2_port, 0x00);

    tens_digit  = 0;
    units_digit = 0;
    current_digit = 0;
}

/* Store a number from 0 to 99 */
void sevensegment2_set_number(uint8_t number)
{
    if (number > 99)
    {
        number = 0;
    }
   
  
    tens_digit  = number / 10;
    units_digit = number % 10;
  

    
}

/*
 * Refresh one digit.
 *
 * Call this function continuously:
 *
 * while(1)
 * {
 *     sevensegment2_refresh();
 * }
 *
 * If a delay/timer is available, keep the refresh rate high enough
 * that both digits appear continuously illuminated.
 */
void sevensegment2_refresh(void)
{
    /* Turn both digits OFF before changing segment data */
    pin_write(digit_port, digit1, LOW);
    pin_write(digit_port, digit2, LOW);

    if (current_digit == 0)
    {
        /* Display tens digit */
        port_write(seg2_port, sevensegment_code[tens_digit]);
        pin_write(digit_port, digit1, HIGH);

        current_digit = 1;
    }
    else
    {
        /* Display units digit */
        port_write(seg2_port, sevensegment_code[units_digit]);
        pin_write(digit_port, digit2, HIGH);

        current_digit = 0;
    }
}

/* Turn OFF both digits and all segments */
void sevensegment2_clear(void)
{
    pin_write(digit_port, digit1, LOW);
    pin_write(digit_port, digit2, LOW);

    port_write(seg2_port, 0x00);
}
