#include "SetMotor.h"

namespace
{
uint8_t pwm_pin_ = 6;
uint8_t in1_pin_ = 15;
uint8_t in2_pin_ = 7;
}

void set_motor_begin(
	uint8_t pwm_pin,
	uint8_t in1_pin,
	uint8_t in2_pin)
{
	pwm_pin_ = pwm_pin;
	in1_pin_ = in1_pin;
	in2_pin_ = in2_pin;

	pinMode(
		in1_pin_,
		OUTPUT);

	pinMode(
		in2_pin_,
		OUTPUT);

	pinMode(
		pwm_pin_,
		OUTPUT);

	digitalWrite(
		in1_pin_,
		LOW);

	digitalWrite(
		in2_pin_,
		LOW);

	analogWrite(
		pwm_pin_,
		0);
}

void set_motor_set_pwm(
	int16_t pwm_value)
{
	if (pwm_value > 255)
	{
		pwm_value = 255;
	}

	if (pwm_value < -255)
	{
		pwm_value = -255;
	}

	if (pwm_value > 0)
	{
		digitalWrite(
			in1_pin_,
			HIGH);

		digitalWrite(
			in2_pin_,
			LOW);

		analogWrite(
			pwm_pin_,
			pwm_value);
	}
	else if (pwm_value < 0)
	{
		digitalWrite(
			in1_pin_,
			LOW);

		digitalWrite(
			in2_pin_,
			HIGH);

		analogWrite(
			pwm_pin_,
			-pwm_value);
	}
	else
	{
		digitalWrite(
			in1_pin_,
			LOW);

		digitalWrite(
			in2_pin_,
			LOW);

		analogWrite(
			pwm_pin_,
			0);
	}
}