#include "SendToLabVIEW.h"

namespace
{
const uint32_t SEND_PERIOD_MS = 20;

uint32_t last_send_time_ms = 0;
}

void send_to_labview_begin(void)
{
	last_send_time_ms = millis();
}

void send_to_labview_update(float rpm_value, uint16_t gas_value, uint16_t brake_value, uint16_t distance_cm, int8_t gear_position)
{
	uint32_t current_time_ms = millis();

	if ((current_time_ms - last_send_time_ms) < SEND_PERIOD_MS)
	{
		return;
	}

	last_send_time_ms = current_time_ms;

	Serial.print((int32_t)rpm_value);
	Serial.print(",");

	Serial.print(gas_value);
	Serial.print(",");

	Serial.print(brake_value);
	Serial.print(",");

	/* send distance then gear position */
	Serial.print(distance_cm);
	Serial.print(",");
	Serial.println((int32_t)gear_position);
}

/*
 * RISK REVIEW:
 * - UART frame format changed from 3 fields to 4 fields.
 * - Caller must update LabVIEW parser.
 * - Distance unit fixed in centimeters.
 */