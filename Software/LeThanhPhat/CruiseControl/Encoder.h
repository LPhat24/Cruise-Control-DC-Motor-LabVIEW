#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

void Encoder_begin(uint8_t pinA, uint8_t pinB, uint16_t pulsesPerChannel, float gearRatio);

void Encoder_update(void);

uint32_t Encoder_getCycleUs(void);

float Encoder_getFrequencyHz(void);

float Encoder_getRPM(void);

int8_t Encoder_getDirection(void);

long Encoder_getPulseCount(void);

#endif