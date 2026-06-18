#include "SendToLabVIEW.h"

namespace
{
const uint32_t SEND_PERIOD_MS = 100;

uint32_t last_send_time_ms = 0;
}

void send_to_labview_begin(void)
{
	last_send_time_ms = millis();
}

void send_to_labview_update(
	float rpm_value)
{
	uint32_t current_time_ms = millis();

	if ((current_time_ms - last_send_time_ms) <
	    SEND_PERIOD_MS)
	{
		return;
	}

	last_send_time_ms = current_time_ms;

	Serial.print("RPM:");
	Serial.println(rpm_value, 2);
}