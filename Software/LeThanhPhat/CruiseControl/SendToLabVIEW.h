#ifndef SEND_TO_LABVIEW_H
#define SEND_TO_LABVIEW_H

#include <Arduino.h>

/* Initialize UART transmit module */

void send_to_labview_begin(void);

/* Send RPM, pedal values and ultrasonic distance */

void send_to_labview_update(float rpm_value, uint16_t gas_value, uint16_t brake_value, uint16_t distance_cm);

#endif