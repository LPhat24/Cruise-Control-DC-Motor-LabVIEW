#include <Arduino.h>
#include "Encoder.h"

static const uint8_t PIN_ENCODER_A = 4;
static const uint8_t PIN_ENCODER_B = 5;

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

    Serial.println("Encoder Frequency Mode Ready");
}

void loop()
{
    Encoder_update();

    static uint32_t lastPrintMs = 0;
    const uint32_t PRINT_INTERVAL_MS = 100;

    if (millis() - lastPrintMs >= PRINT_INTERVAL_MS)
    {
        lastPrintMs = millis();

        Serial.print("Cycle:");
        Serial.print(Encoder_getCycleUs());

        Serial.print(" Frequency:");
        Serial.print(Encoder_getFrequencyHz(), 2);

        Serial.print(" RPM:");
        Serial.print(Encoder_getRPM(), 2);

        Serial.print(" Direction:");

        if (Encoder_getDirection() > 0)
        {
            Serial.println("CW");
        }
        else if (Encoder_getDirection() < 0)
        {
            Serial.println("CCW");
        }
        else
        {
            Serial.println("STOP");
        }
    }
}