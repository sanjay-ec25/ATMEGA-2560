#include "ultra.h"
#include "gpio.h"
#include "timer.h"

static uint8_t ultra_port;
static uint8_t trig_pin;
static uint8_t echo_pin;

/* ATmega2560 Timer1 registers */
#define TCCR1A  (*(volatile uint8_t *)0x80)
#define TCCR1B  (*(volatile uint8_t *)0x81)
#define TCNT1L  (*(volatile uint8_t *)0x84)
#define TCNT1H  (*(volatile uint8_t *)0x85)
#define TCNT1   (*(volatile uint16_t *)0x84)

/* Timer1 control bits */
#define CS11 1
#define CS10 0

void ultra_init(uint8_t port, uint8_t trig, uint8_t echo)
{
    ultra_port = port;
    trig_pin = trig;
    echo_pin = echo;

    pin_config(ultra_port, trig_pin, OUTPUT);
    pin_config(ultra_port, echo_pin, INPUT);

    pin_write(ultra_port, trig_pin, LOW);

    /* Configure Timer1 in normal mode */
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
}


  void ultra_trigger(void)
{
    volatile uint8_t i;

    /* Trigger LOW */
    pin_write(ultra_port, trig_pin, LOW);

    for (i = 0; i < 10; i++)
    {
        __asm__ __volatile__("nop");
    }

    /* Trigger HIGH */
    pin_write(ultra_port, trig_pin, HIGH);

    /*
       Approximate 10 us pulse at 16 MHz.
       Actual duration depends on compiler-generated code.
    */
    for (i = 0; i < 50; i++)
    {
        __asm__ __volatile__("nop");
    }

    /* Trigger LOW */
    pin_write(ultra_port, trig_pin, LOW);
}


uint16_t ultra_get_distance_cm(void)
{
    uint16_t start;
    uint16_t elapsed;
    uint32_t timeout;

    ultra_trigger();

    /* Timer1: 16 MHz / 8 = 2 MHz, or 0.5 us per tick */
    TCCR1B = (1 << CS11);
    TCNT1 = 0;

    /* Wait for ECHO to become HIGH */
    timeout = 60000;

    while (pin_read(ultra_port, echo_pin) == LOW)
    {
        if (--timeout == 0)
        {
            TCCR1B = 0;
            return 0;
        }
    }

    /* Start measuring the echo pulse */
    start = TCNT1;

    timeout = 60000;

    while (pin_read(ultra_port, echo_pin) == HIGH)
    {
        if (--timeout == 0)
        {
            TCCR1B = 0;
            return 0;
        }
    }

    elapsed = (uint16_t)(TCNT1 - start);

    TCCR1B = 0;

    /* Timer tick = 0.5 us; distance in cm = pulse_us / 58 */
    return (uint16_t)(elapsed / 116);
}

uint16_t ultra_get_distance_mm(void)
{
    uint16_t distance_cm;

    distance_cm = ultra_get_distance_cm();

    return distance_cm * 10;
}