// CruiseControl.ino
// - Chỉ chứa include các module, setup() và loop()
// - Mọi thuật toán xử lý (đếm xung, tính góc, RPM...) nằm trong Encoder module

#include <Arduino.h>
#include "Encoder.h"

// Pin encoder (tùy theo mạch, trên Arduino Uno chỉ pins 2 và 3 hỗ trợ attachInterrupt trên nhiều bo mạch)
// Người dùng có thể thay đổi nếu dùng board khác.
static const uint8_t PIN_ENCODER_A = 2;
static const uint8_t PIN_ENCODER_B = 3;

// Thông số encoder/ cơ khí
static const uint16_t ENCODER_PULSES_PER_CHANNEL = 11; // xung trên 1 vòng motor trên 1 kênh (A hoặc B)
static const float GEAR_RATIO = 46.8f;                 // tỉ số truyền motor:output

void setup() {
  Serial.begin(115200);
  while (!Serial) { }

  // Khởi tạo module Encoder (gọi một lần duy nhất)
  Encoder.begin(PIN_ENCODER_A, PIN_ENCODER_B, ENCODER_PULSES_PER_CHANNEL, GEAR_RATIO);

  Serial.println("[CruiseControl] Encoder module initialized");
}

void loop() {
  // Gọi cập nhật định kỳ (không chặn) để tính RPM, góc, vòng quay
  Encoder.update();

  // In ra Serial mỗi 100 ms (để hiển thị cho LabVIEW/Serial Monitor)
  static unsigned long lastPrint = 0;
  const unsigned long PRINT_INTERVAL_MS = 100;
  if (millis() - lastPrint >= PRINT_INTERVAL_MS) {
    lastPrint = millis();
    Encoder.printStatusToSerial();
  }
}
