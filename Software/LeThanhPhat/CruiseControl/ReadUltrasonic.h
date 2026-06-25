#ifndef READ_ULTRASONIC_H
#define READ_ULTRASONIC_H

#include <Arduino.h>

/* Initialize ultrasonic sensor */

void read_ultrasonic_begin(uint8_t trig_pin, uint8_t echo_pin);

/* Update ultrasonic measurement */

void read_ultrasonic_update(void);

/* Get distance in centimeters */

uint16_t read_ultrasonic_get_distance_cm(void);

#endif