#ifndef GEAR_POSITION_H
#define GEAR_POSITION_H

#include <Arduino.h>

/* Gear position constants */
#define GEAR_P 0
#define GEAR_R 1
#define GEAR_N 2
#define GEAR_D 3

/* Initialize gear input pins */
void gear_begin(uint8_t pin_p, uint8_t pin_r, uint8_t pin_n, uint8_t pin_d);

/* Run debounced state machine and apply validated shifts */
void gear_update(void);

/* Get current (validated) gear */
int8_t gear_get_position(void);

#endif
