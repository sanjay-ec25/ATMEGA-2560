#include "define.h"
#include "gpio.h"
#include "lcd.h"
#include "ultra.h"
#include "buzzer.h"
#include "ir.h"
#include <util/delay.h>

/* 7-Segment Digits Lookup Table for PORT_B (Digits 0, 1, and 2)
 * Note: Digit '2' avoids Segment C (PB2 / Pin 51) because of the hardware defect! */
const uint8_t seg7_map[3] = {
    0x3F, // '0' -> Segments A, B, C, D, E, F
    0x06, // '1' -> Segments B, C
    0x5B  // '2' -> Segments A, B, D, E, G (Safe workaround for broken Seg C)
};

void seg7_init(void)
{
    for (uint8_t i = 0; i <= 6; i++) {
        pin_config(PORT_B, i, OUTPUT);
    }
}

void seg7_display_number(uint8_t num)
{
    if (num > 2) num = 2;
    uint8_t pattern = seg7_map[num];
    for (uint8_t i = 0; i <= 6; i++) {
        pin_write(PORT_B, i, (pattern >> i) & 1);
    }
}

void leds_init(void)
{
    /* Initialize status LEDs using custom gpio driver */
    pin_config(PORT_E, 4, OUTPUT); // Pin 2: SAFE (Green)
    pin_config(PORT_H, 3, OUTPUT); // Pin 6: CAUTION (Yellow)
    pin_config(PORT_G, 5, OUTPUT); // Pin 4: WARNING (Orange)
    pin_config(PORT_E, 3, OUTPUT); // Pin 5: STOP (Red)
}

void set_led_status(uint8_t safe, uint8_t caution, uint8_t warning, uint8_t stop)
{
    pin_write(PORT_E, 4, safe);    // Pin 2
    pin_write(PORT_H, 3, caution); // Pin 6
    pin_write(PORT_G, 5, warning); // Pin 4
    pin_write(PORT_E, 3, stop);    // Pin 5
}

void setup()
{
    Serial.begin(9600);

    /* 1. Initialize 7-Segment Display on PORT_B */
    seg7_init();

    /* 2. Initialize Dual IR Sensors on PORT_K (PK0 = Pin A8, PK1 = Pin A9) */
    ir_init_dual(PORT_K, 0, 1);

    /* 3. Initialize Buzzer on PORT_A, Pin 1 (Arduino Pin 23) */
    buzzer_init(PORT_A, 1);

    /* 4. Initialize Status LEDs */
    leds_init();

    /* 5. Initialize Ultrasonic Sensor on PORT_H (Trig = PH4 / Pin 7, Echo = PH5 / Pin 8) */
    ultra_init(PORT_H, 4, 5);

    /* 6. Initialize LCD 1602 on PORT_C */
    lcd_init(PORT_C, 0, 1, 2, 3, 4, 5);

    /* Display static headers on LCD */
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("DIST: ");
    
    lcd_set_cursor(1, 0);
    lcd_print("STATUS: ");
}

void loop()
{
    /* 1. Fetch Sensor Readings */
    uint16_t distance_cm = ultra_get_distance_cm();
    uint8_t total_free_slots = ir_get_total_free(); // Returns 0, 1, or 2

    /* 2. Update 7-Segment Display (Shows available slots 0-2) */
    seg7_display_number(total_free_slots);

    /* 3. Update LCD Row 0: Distance */
    lcd_set_cursor(0, 6);
    lcd_print_number(distance_cm);
    lcd_print(" cm   ");

    /* 4. Update LCD Row 1 & LEDs & Buzzer Based on Distance Status */
    lcd_set_cursor(1, 8);
    
    if (distance_cm > 50)
    {
        lcd_print("SAFE    ");
        set_led_status(HIGH, LOW, LOW, LOW); // Green LED ON
    }
    else if (distance_cm > 30)
    {
        lcd_print("CAUTION ");
        set_led_status(LOW, HIGH, LOW, LOW); // Yellow LED ON
    }
    else if (distance_cm > 15)
    {
        lcd_print("WARNING ");
        set_led_status(LOW, LOW, HIGH, LOW); // Orange LED ON
    }
    else
    {
        lcd_print("STOP!!  ");
        set_led_status(LOW, LOW, LOW, HIGH); // Red LED ON
    }

    buzzer_update_pattern(distance_cm);

    /* 5. Serial Debugger Output */
    Serial.print("Dist: ");
    Serial.print(distance_cm);
    Serial.print(" cm | Free Slots: ");
    Serial.println(total_free_slots);

    _delay_ms(50);
}