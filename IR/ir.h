#ifndef IR_H
#define IR_H

#include <stdint.h>

/* Initialize 1 IR sensor on specified port and pin */
void ir_init(uint8_t port, uint8_t pin);

/* Check if vehicle is present at sensor (1 = Occupied, 0 = Empty) */
uint8_t ir_is_occupied(void);

/* Returns total available slots (1 = Open, 0 = Full) */
uint8_t ir_get_available_slots(void);

#endif /* IR_H */