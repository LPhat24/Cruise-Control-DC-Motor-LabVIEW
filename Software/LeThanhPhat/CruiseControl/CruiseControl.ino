#include <Arduino.h>
#include "Encoder.h"
#include "SendToLabVIEW.h"

static const uint8_t PIN_ENCODER_A = 16;
static const uint8_t PIN_ENCODER_B = 17;

static const uint16_t ENCODER_PULSES_PER_CHANNEL = 11;
static const float GEAR_RATIO = 46.8f;

void setup()
{
	Serial.begin(115200);

	Encoder_begin(
		PIN_ENCODER_A,
		PIN_ENCODER_B,
		ENCODER_PULSES_PER_CHANNEL,
		GEAR_RATIO);

	send_to_labview_begin();

	Serial.println("Encoder Frequency Mode Ready");
}

void loop()
{
	Encoder_update();

	send_to_labview_update(
		Encoder_getRPM());
}