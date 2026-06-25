#include "ReadUltrasonic.h"

namespace
{
const uint32_t ECHO_TIMEOUT_US = 30000UL;
const uint32_t MEASURE_PERIOD_MS = 50UL;

uint8_t trig_pin_ = 0;
uint8_t echo_pin_ = 0;

uint16_t distance_cm_ = 0;

uint32_t last_measure_time_ms_ = 0;
}

void read_ultrasonic_begin(uint8_t trig_pin, uint8_t echo_pin)
{
	trig_pin_ = trig_pin;
	echo_pin_ = echo_pin;

	pinMode(trig_pin_, OUTPUT);
	pinMode(echo_pin_, INPUT);

	digitalWrite(trig_pin_, LOW);

	last_measure_time_ms_ = millis();
}

void read_ultrasonic_update(void)
{
	uint32_t current_time_ms;
	uint32_t pulse_width_us;

	current_time_ms = millis();

	if ((current_time_ms - last_measure_time_ms_) < MEASURE_PERIOD_MS)
		return;

	last_measure_time_ms_ = current_time_ms;

	digitalWrite(trig_pin_, LOW);
	delayMicroseconds(2);

	digitalWrite(trig_pin_, HIGH);
	delayMicroseconds(10);

	digitalWrite(trig_pin_, LOW);

	pulse_width_us = pulseIn(echo_pin_, HIGH, ECHO_TIMEOUT_US);

	if (pulse_width_us == 0)
	{
		distance_cm_ = 0;
		return;
	}

	distance_cm_ = (uint16_t)(pulse_width_us / 58UL);
}

uint16_t read_ultrasonic_get_distance_cm(void)
{
	return distance_cm_;
}

/*
 * RISK REVIEW:
 * - pulseIn() vẫn là blocking function.
 * - Timeout sẽ trả về 0 cm.
 * - Chu kỳ đo hiện tại là 50 ms (~20 Hz).
 * - Caller phải gọi read_ultrasonic_update() liên tục trong loop.
 */