#ifndef RECEIVE_FROM_LABVIEW_H
#define RECEIVE_FROM_LABVIEW_H

#include <Arduino.h>

void receive_from_labview_begin(void);

void receive_from_labview_update(void);

int16_t receive_from_labview_get_pwm(void);

int16_t receive_from_labview_get_value_2(void);

int16_t receive_from_labview_get_value_3(void);

#endif