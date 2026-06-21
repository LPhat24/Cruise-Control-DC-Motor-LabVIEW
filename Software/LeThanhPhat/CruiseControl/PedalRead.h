#ifndef PEDAL_READ_H
#define PEDAL_READ_H

#include <Arduino.h>

void pedal_read_begin(uint8_t gas_pin, uint8_t brake_pin);

void pedal_read_update(void);

uint16_t pedal_read_get_gas_value(void);

uint16_t pedal_read_get_brake_value(void);

#endif