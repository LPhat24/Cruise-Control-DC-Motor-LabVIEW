#ifndef SET_MOTOR_H
#define SET_MOTOR_H

#include <Arduino.h>

void set_motor_begin(uint8_t pwm_pin, uint8_t in1_pin, uint8_t in2_pin);

void set_motor_set_pwm(int16_t pwm_value);

#endif