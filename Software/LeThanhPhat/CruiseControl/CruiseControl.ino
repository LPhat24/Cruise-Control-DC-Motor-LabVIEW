#include <Arduino.h>
#include "Encoder.h"
#include "PedalRead.h"
#include "ReadUltrasonic.h"
#include "SendToLabVIEW.h"
#include "ReceiveFromLabVIEW.h"
#include "SetMotor.h"

static const uint8_t PIN_ENCODER_A = 16;
static const uint8_t PIN_ENCODER_B = 17;

static const uint8_t PIN_GAS_PEDAL = 1;
static const uint8_t PIN_BRAKE_PEDAL = 2;

static const uint8_t PIN_MOTOR_PWM = 40;
static const uint8_t PIN_MOTOR_IN1 = 38;
static const uint8_t PIN_MOTOR_IN2 = 39;

static const uint8_t PIN_ULTRASONIC_TRIG = 9;
static const uint8_t PIN_ULTRASONIC_ECHO = 10;

static const uint16_t ENCODER_PULSES_PER_CHANNEL = 11;
static const float GEAR_RATIO = 9.6f;

void setup()
{
	Serial.begin(115200);

	Encoder_begin(PIN_ENCODER_A, PIN_ENCODER_B, ENCODER_PULSES_PER_CHANNEL, GEAR_RATIO);

	pedal_read_begin(PIN_GAS_PEDAL, PIN_BRAKE_PEDAL);

	read_ultrasonic_begin(PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO);

	send_to_labview_begin();

	receive_from_labview_begin();

	set_motor_begin(PIN_MOTOR_PWM, PIN_MOTOR_IN1, PIN_MOTOR_IN2);

	Serial.println("Cruise Control Ready");
}

void loop()
{
	Encoder_update();

	pedal_read_update();

	read_ultrasonic_update();

	receive_from_labview_update();

	set_motor_set_pwm(receive_from_labview_get_pwm());

	send_to_labview_update(
		Encoder_getRPM(),
		pedal_read_get_gas_value(),
		pedal_read_get_brake_value(),
		read_ultrasonic_get_distance_cm());
}