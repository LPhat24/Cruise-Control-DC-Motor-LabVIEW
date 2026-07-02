#include "Encoder.h"

namespace
{
const uint32_t MICROS_PER_SECOND = 1000000UL;
const uint32_t SIGNAL_TIMEOUT_US = 200000UL;

const uint8_t CYCLE_AVERAGE_COUNT = 2;

uint8_t pinA_ = 4;
uint8_t pinB_ = 5;

uint16_t pulsesPerChannel_ = 11;
float gearRatio_ = 9.6f; // Loại 620RPM thì TST=9.6; Loại 130RPM thì TST=46.8

float pulsesPerOutputRev_ = 1.0f;

volatile long pulseCount_ = 0;

volatile uint32_t currentTimeUs_ = 0;
volatile uint32_t previousTimeUs_ = 0;

volatile uint32_t cycleUs_ = 0;
volatile uint32_t cycleSumUs_ = 0;
volatile uint8_t cycleCounter_ = 0;

volatile int8_t direction_ = 0;

float frequencyHz_ = 0.0f;
float rpm_ = 0.0f;
}

static void Encoder_isrA(void);

void Encoder_begin(uint8_t pinA, uint8_t pinB, uint16_t pulsesPerChannel, float gearRatio)
{
	pinA_ = pinA;
	pinB_ = pinB;

	pulsesPerChannel_ = pulsesPerChannel;
	gearRatio_ = gearRatio;

	pulsesPerOutputRev_ = (float)pulsesPerChannel_ * gearRatio_;

	pinMode(pinA_, INPUT_PULLUP);
	pinMode(pinB_, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(pinA_), Encoder_isrA, RISING);
}

void Encoder_update(void)
{
	uint32_t cycleLocal;
	uint32_t currentTimeLocal;
	int8_t directionLocal;

	noInterrupts();

	cycleLocal = cycleUs_;
	currentTimeLocal = currentTimeUs_;
	directionLocal = direction_;

	interrupts();

	if (((micros() - currentTimeLocal) <= SIGNAL_TIMEOUT_US) && (cycleLocal > 0))
	{
		frequencyHz_ = (float)MICROS_PER_SECOND / (float)cycleLocal;
		// CycleUs has units of us. To convert to s, multiply by 10^-6. Therefore, f = 1/cycleUs * 10^-6 = 10^6/cycleUs
	}
	else
	{
		frequencyHz_ = 0.0f;
		directionLocal = 0;
	}

	rpm_ = (frequencyHz_ * 60.0f) / pulsesPerOutputRev_;
	// Pulses/second divided by pulses/revolutions = revolutions/second. Multiply by 60 -> revolutions/second * seconds/minute = RPM
	rpm_ *= directionLocal;
}

uint32_t Encoder_getCycleUs(void)
{
	noInterrupts();

	uint32_t value = cycleUs_;

	interrupts();

	return value;
}

float Encoder_getFrequencyHz(void)
{
	return frequencyHz_;
}

float Encoder_getRPM(void)
{
	return rpm_;
}

int8_t Encoder_getDirection(void)
{
	noInterrupts();

	int8_t value = direction_;

	interrupts();

	return value;
}

long Encoder_getPulseCount(void)
{
	noInterrupts();

	long value = pulseCount_;

	interrupts();

	return value;
}

static void Encoder_isrA(void)
{
	previousTimeUs_ = currentTimeUs_;
	currentTimeUs_ = micros();

	uint32_t cycle = currentTimeUs_ - previousTimeUs_;

	cycleSumUs_ += cycle;
	cycleCounter_++;

	if (cycleCounter_ >= CYCLE_AVERAGE_COUNT)
	{
		cycleUs_ = cycleSumUs_ / CYCLE_AVERAGE_COUNT;

		cycleSumUs_ = 0;
		cycleCounter_ = 0;
	}

	bool b = digitalRead(pinB_);

	if (b == LOW)
	{
		pulseCount_++;
		direction_ = 1;
	}
	else
	{
		pulseCount_--;
		direction_ = -1;
	}
}