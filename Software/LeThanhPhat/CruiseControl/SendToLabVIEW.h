#ifndef SEND_TO_LABVIEW_H
#define SEND_TO_LABVIEW_H

#include <Arduino.h>

void send_to_labview_begin(void);

void send_to_labview_update(
	float rpm_value);

#endif