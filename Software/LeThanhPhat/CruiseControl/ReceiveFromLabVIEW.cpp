#include "ReceiveFromLabVIEW.h"

namespace
{
String receive_buffer_;

int16_t pwm_value_ = 0;
int16_t value_2_ = 0;
int16_t value_3_ = 0;
}

static void parse_frame(
	String frame);

void receive_from_labview_begin(void)
{
	receive_buffer_ = "";
}

void receive_from_labview_update(void)
{
	while (Serial.available())
	{
		char received_char =
			(char)Serial.read();

		if (received_char == '\n')
		{
			parse_frame(
				receive_buffer_);

			receive_buffer_ = "";
		}
		else if (received_char != '\r')
		{
			receive_buffer_ +=
				received_char;
		}
	}
}

int16_t receive_from_labview_get_pwm(void)
{
	return pwm_value_;
}

int16_t receive_from_labview_get_value_2(void)
{
	return value_2_;
}

int16_t receive_from_labview_get_value_3(void)
{
	return value_3_;
}

static void parse_frame(
	String frame)
{
	int first_comma =
		frame.indexOf(',');

	int second_comma =
		frame.indexOf(
			',',
			first_comma + 1);

	if (first_comma < 0 ||
	    second_comma < 0)
	{
		return;
	}

	pwm_value_ =
		frame.substring(
			0,
			first_comma).toInt();

	value_2_ =
		frame.substring(
			first_comma + 1,
			second_comma).toInt();

	value_3_ =
		frame.substring(
			second_comma + 1).toInt();
}