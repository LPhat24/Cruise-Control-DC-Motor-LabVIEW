#ifndef SEND_TO_LABVIEW_H
#define SEND_TO_LABVIEW_H

#include <Arduino.h>

void send_to_labview_begin(void);

void send_to_labview_update(
	float rpm_value,
	uint16_t gas_value,
	uint16_t brake_value);

void send_to_labview_receive(void);

int16_t send_to_labview_get_motor_pwm(void);

#endif