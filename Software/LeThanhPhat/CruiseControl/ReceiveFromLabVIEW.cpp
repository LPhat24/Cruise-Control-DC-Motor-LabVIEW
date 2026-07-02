#include "ReceiveFromLabVIEW.h"
#include "SetMotor.h"

namespace
{
	const size_t RX_BUFFER_SIZE = 32;
	// Use the `size_t` data type to declare the string size.

	char rx_buffer_[RX_BUFFER_SIZE];
	size_t rx_index_ = 0;

	int16_t pwm_value_ = 0;
}

static void parse_buffer(const char *buf);

void receive_from_labview_begin(void)
{
	rx_index_ = 0;
	pwm_value_ = 0;
}

void receive_from_labview_update(void)
{
	while (Serial.available())
	{
		int c = Serial.read();
		// Example -120\n -> '-' '1' '2' '0' '\n'

		if (c < 0)
			continue;

		if (c == '\r')
			continue; /* ignore CR */

		if (c == '\n')
		{
			rx_buffer_[rx_index_] = '\0';
			// Change \n to \0
			parse_buffer(rx_buffer_);
			rx_index_ = 0;
			continue;
		}

		if (rx_index_ < (RX_BUFFER_SIZE - 1))
		{
			rx_buffer_[rx_index_++] = (char)c;
			// Store the character in the buffer and increment the index. 
		}
		else
		{
			/* overflow, reset buffer */
			rx_index_ = 0;
		}
	}
}

int16_t receive_from_labview_get_pwm(void)
{
	return pwm_value_;
}

int16_t receive_from_labview_get_value_2(void)
{
	return 0;
}

int16_t receive_from_labview_get_value_3(void)
{
	return 0;
}

static void parse_buffer(const char *buf)
{
	/* skip leading spaces */
	const char *p = buf;

	while (*p == ' ' || *p == '\t')
		p++;

	if (*p == '\0')
		return;

	/* optional sign */
	int sign = 1;
	if (*p == '+')
	{
		sign = 1;
		p++;
	}
	else if (*p == '-')
	{
		sign = -1;
		p++;
	}

	int value = 0;
	bool has_digit = false;

	while (*p >= '0' && *p <= '9')
	{
		has_digit = true;
		int digit = (*p - '0');
		// Convert the character to its integer value by subtracting the ASCII value of '0'.
		/* 
		ASCII

		'0' ---- 48
		'1' ---- 49
		'2' ---- 50
		'3' ---- 51
		'4' ---- 52
		'5' ---- 53
		'6' ---- 54
		'7' ---- 55
		'8' ---- 56
		'9' ---- 57
		*/
		value = value * 10 + digit; 
		/*	Transform a discrete sequence of numbers into a complete number.
			Example: 1,2,0 -> 1 -> 12 -> 120
		*/
		if (value > 255) /* magnitude overflow */
			return; /* out of range */
		p++;
	}

	/* reject if no digits or invalid trailing characters */
	if (!has_digit)
		return;

	while (*p == ' ' || *p == '\t') /* allow trailing whitespace */
		p++;

	if (*p != '\0')
		return; /* invalid trailing chars */

	int signed_value = value * sign; 
	// Apply the sign to the value to get the final signed integer.	

	/* final range check -255..255 */
	if (signed_value < -255 || signed_value > 255)
		return;

	pwm_value_ = (int16_t)signed_value;

	/* apply command via motor module */
	set_motor_set_pwm(pwm_value_);
}