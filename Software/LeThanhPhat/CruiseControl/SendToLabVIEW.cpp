#include "SendToLabVIEW.h"

namespace
{
const uint32_t SEND_PERIOD_MS = 100;

uint32_t last_send_time_ms = 0;

String receive_buffer_;

int16_t motor_pwm_ = 0;
}

void send_to_labview_begin(void)
{
	last_send_time_ms = millis();
}

void send_to_labview_update(
	float rpm_value,
	uint16_t gas_value,
	uint16_t brake_value)
{
	uint32_t current_time_ms = millis();

	if ((current_time_ms - last_send_time_ms) <
	    SEND_PERIOD_MS)
	{
		return;
	}

	last_send_time_ms = current_time_ms;

	Serial.print(rpm_value, 2);
	Serial.print(",");
	Serial.print(gas_value);
	Serial.print(",");
	Serial.println(brake_value);
}

void send_to_labview_receive(void)
{
	while (Serial.available())
	{
		char received_char =
			(char)Serial.read();

		if (received_char == '\n')
		{
			motor_pwm_ =
				receive_buffer_.toInt();

			receive_buffer_ = "";
		}
		else if (received_char != '\r')
		{
			receive_buffer_ +=
				received_char;
		}
	}
}

int16_t send_to_labview_get_motor_pwm(void)
{
	return motor_pwm_;
}