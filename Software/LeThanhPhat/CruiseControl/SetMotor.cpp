#include "SetMotor.h"

namespace
{
uint8_t pwm_pin_ = 0;
uint8_t in1_pin_ = 0;
uint8_t in2_pin_ = 0;

const uint8_t PWM_CHANNEL = 0;

const uint32_t PWM_FREQUENCY = 2000;
const uint8_t PWM_RESOLUTION = 8;
}

void set_motor_begin(uint8_t pwm_pin, uint8_t in1_pin, uint8_t in2_pin)
{
	pwm_pin_ = pwm_pin;
	in1_pin_ = in1_pin;
	in2_pin_ = in2_pin;

	pinMode(in1_pin_, OUTPUT);
	pinMode(in2_pin_, OUTPUT);

	digitalWrite(in1_pin_, LOW);
	digitalWrite(in2_pin_, LOW);

	ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);

	ledcAttachPin(pwm_pin_, PWM_CHANNEL);

	ledcWrite(PWM_CHANNEL, 0);
}

void set_motor_set_pwm(int16_t pwm_value)
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
		digitalWrite(in1_pin_, HIGH);
		digitalWrite(in2_pin_, LOW);

		ledcWrite(PWM_CHANNEL, pwm_value);
	}
	else if (pwm_value < 0)
	{
		digitalWrite(in1_pin_, LOW);
		digitalWrite(in2_pin_, HIGH);

		ledcWrite(PWM_CHANNEL, -pwm_value);
	}
	else
	{
		digitalWrite(in1_pin_, LOW);
		digitalWrite(in2_pin_, LOW);

		ledcWrite(PWM_CHANNEL, 0);
	}
}