#include "pwm.h"
#include "timer.h"

void setup()
{
    timer_init();
    pwm_init();
}

void loop()
{
    // Start the buzzer at 50% duty cycle
    pwm_set_duty(128);

    // Buzzer ON for 1 second
    timer_delay_ms(1000);

    // Stop the PWM output
    pwm_stop();

    // Buzzer OFF for 1 second
    timer_delay_ms(1000);
}