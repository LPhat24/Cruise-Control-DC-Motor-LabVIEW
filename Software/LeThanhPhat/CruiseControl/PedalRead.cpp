#include "PedalRead.h"

namespace
{
const uint8_t FILTER_SIZE = 16;

uint8_t gas_pin_ = 0;
uint8_t brake_pin_ = 0;

uint16_t gas_value_ = 0;
uint16_t brake_value_ = 0;

uint16_t gas_buffer_[FILTER_SIZE];
uint16_t brake_buffer_[FILTER_SIZE];

uint32_t gas_sum_ = 0;
uint32_t brake_sum_ = 0;

uint8_t filter_index_ = 0;
bool filter_ready_ = false;
}

void pedal_read_begin(
	uint8_t gas_pin,
	uint8_t brake_pin)
{
	gas_pin_ = gas_pin;
	brake_pin_ = brake_pin;

	pinMode(gas_pin_, INPUT);
	pinMode(brake_pin_, INPUT);

	analogReadResolution(12);

	for (uint8_t i = 0; i < FILTER_SIZE; i++)
	{
		gas_buffer_[i] = 0;
		brake_buffer_[i] = 0;
	}

	gas_sum_ = 0;
	brake_sum_ = 0;

	filter_index_ = 0;
	filter_ready_ = false;
}

void pedal_read_update(void)
{
	uint16_t gas_raw_value =
		analogRead(gas_pin_);

	uint16_t brake_raw_value =
		analogRead(brake_pin_);

	gas_sum_ -=
		gas_buffer_[filter_index_];

	brake_sum_ -=
		brake_buffer_[filter_index_];

	gas_buffer_[filter_index_] =
		gas_raw_value;

	brake_buffer_[filter_index_] =
		brake_raw_value;

	gas_sum_ +=
		gas_raw_value;

	brake_sum_ +=
		brake_raw_value;

	filter_index_++;

	if (filter_index_ >= FILTER_SIZE)
	{
		filter_index_ = 0;
		filter_ready_ = true;
	}

	uint8_t sample_count =
		filter_ready_ ?
		FILTER_SIZE :
		filter_index_;

	if (sample_count == 0)
	{
		return;
	}

	gas_value_ =
		gas_sum_ /
		sample_count;

	brake_value_ =
		brake_sum_ /
		sample_count;
}

uint16_t pedal_read_get_gas_value(void)
{
	return gas_value_;
}

uint16_t pedal_read_get_brake_value(void)
{
	return brake_value_;
}