/*
  Encoder.cpp
  Implementation of Encoder module.

  - Sử dụng two-pin quadrature decoding (interrupt CHANGE trên cả A và B)
  - Bộ đệm trung bình cho RPM
  - Không dùng magic numbers: mọi hằng số cấu hình ở trên cùng

  Chú thích phương pháp đếm xung (chi tiết):
  - Khi sử dụng quadrature AB, có thể đếm 4 cạnh (X4 decoding) để tăng độ phân giải.
    Cách đơn giản và hiệu quả cho Arduino:
      + Đăng ký interrupt CHANGE cho cả A và B.
      + Trong ISR đọc trạng thái mới của A và B, so sánh với nhau để quyết định tăng hay giảm.
    Quy tắc được dùng ở đây:
      - Trên cạnh thay đổi của A: nếu A != B sau thay đổi => +1, else -1
      - Trên cạnh thay đổi của B: nếu A == B sau thay đổi => +1, else -1

  Công thức sử dụng:
  - pulses_per_motor_rev = pulses_per_channel * 4
  - pulses_per_output_rev = pulses_per_motor_rev * gear_ratio
  - revolutions = pulse_count / pulses_per_output_rev
  - angle_deg = fmod(revolutions, 1.0) * 360
  - rpm = (delta_pulses / pulses_per_output_rev) * (60 / dt_seconds)

*/

#include "Encoder.h"

// File-scope static state (not exported). Đặt static để không phải biến global không giới hạn.
namespace {
// Configuration defaults (will be set in begin())
uint8_t pinA_ = 2;
uint8_t pinB_ = 3;
uint16_t pulsesPerChannel_ = 11; // xung trên 1 vòng motor trên 1 kênh
float gearRatio_ = 46.8f;

// Derived values
float pulsesPerMotorRev_ = 0.0f;   // pulses per motor rev (after X4)
float pulsesPerOutputRev_ = 0.0f;  // pulses per output rev (after gearing)
// Integer pulses per output revolution used for angle/rev calculation (calibrated/rounded)
unsigned long pulsesPerOutputRevCount_ = 0;

// Volatile state updated in ISR
volatile long pulseCountMotor_ = 0; // xung tích luỹ trên motor (signed)

// RPM calculation buffers
const uint8_t RPM_SAMPLE_COUNT = 5; // dùng cho moving average
float rpmSamples_[RPM_SAMPLE_COUNT];
uint8_t rpmIndex_ = 0;
bool rpmFilled_ = false;

// Timing for RPM calculation (microseconds)
const unsigned long RPM_SAMPLE_INTERVAL_US = 100000UL; // 100 ms
unsigned long lastRpmCalcTimeUs_ = 0;
long lastPulseCountSnapshot_ = 0;

} // anonymous namespace

// Forward declarations for ISRs
void isrA();
void isrB();

// Singleton instance definition
EncoderClass Encoder;

void EncoderClass::begin(uint8_t pinA, uint8_t pinB, uint16_t pulsesPerChannel, float gearRatio) {
  pinA_ = pinA;
  pinB_ = pinB;
  pulsesPerChannel_ = pulsesPerChannel;
  gearRatio_ = gearRatio;

  // Derived
  pulsesPerMotorRev_ = (float)pulsesPerChannel_ * 4.0f; // X4 decoding
  pulsesPerOutputRev_ = pulsesPerMotorRev_ * gearRatio_;
  // Use rounded integer for final per-output count to avoid fractional-pulse issues
  pulsesPerOutputRevCount_ = (unsigned long)(pulsesPerOutputRev_ + 0.5f);

  // Configure pins
  pinMode(pinA_, INPUT_PULLUP);
  pinMode(pinB_, INPUT_PULLUP);

  // Attach interrupts on both channels using CHANGE for quadrature decoding
  attachInterrupt(digitalPinToInterrupt(pinA_), isrA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB_), isrB, CHANGE);

  // Initialize RPM buffer
  for (uint8_t i = 0; i < RPM_SAMPLE_COUNT; i++) rpmSamples_[i] = 0.0f;
  rpmIndex_ = 0;
  rpmFilled_ = false;
  lastRpmCalcTimeUs_ = micros();
  lastPulseCountSnapshot_ = pulseCountMotor_;
}

void EncoderClass::setPulsesPerOutputRev(unsigned long pulses) {
  if (pulses > 0) {
    pulsesPerOutputRevCount_ = pulses;
    // keep the floating derived for informational purposes
    pulsesPerOutputRev_ = (float)pulses;
  }
}

// Internal: safe snapshot of volatile pulse count
static long snapshotPulseCount() {
  noInterrupts();
  long val = pulseCountMotor_;
  interrupts();
  return val;
}

void EncoderClass::update() {
  unsigned long nowUs = micros();
  if ((nowUs - lastRpmCalcTimeUs_) >= RPM_SAMPLE_INTERVAL_US) {
    long currentSnapshot = snapshotPulseCount();
    long delta = currentSnapshot - lastPulseCountSnapshot_;
    lastPulseCountSnapshot_ = currentSnapshot;

    float dt = (nowUs - lastRpmCalcTimeUs_) / 1e6f; // seconds
    lastRpmCalcTimeUs_ = nowUs;

    // Compute RPM using pulses counted over dt
    float rpm = 0.0f;
    if (dt > 0.0f) {
      rpm = (delta / pulsesPerOutputRev_) * (60.0f / dt);
    }

    // Store absolute RPM and apply simple moving average
    rpmSamples_[rpmIndex_++] = fabs(rpm);
    if (rpmIndex_ >= RPM_SAMPLE_COUNT) {
      rpmIndex_ = 0;
      rpmFilled_ = true;
    }
  }
}

long EncoderClass::getPulseCount() {
  return snapshotPulseCount();
}

double EncoderClass::getRevolutions() {
  double pulses = (double)snapshotPulseCount();
  if (pulsesPerOutputRevCount_ == 0) return 0.0;
  return pulses / (double)pulsesPerOutputRevCount_;
}

double EncoderClass::getAngularPositionDeg() {
  double rev = getRevolutions();
  double frac = rev - floor(rev);
  double angle = frac * 360.0;
  if (angle < 0) angle += 360.0; // normalize
  return angle;
}

double EncoderClass::getRPM() {
  // average available samples
  uint8_t count = rpmFilled_ ? RPM_SAMPLE_COUNT : rpmIndex_;
  if (count == 0) return 0.0;
  float sum = 0.0f;
  for (uint8_t i = 0; i < count; i++) sum += rpmSamples_[i];
  return (double)(sum / count);
}

void EncoderClass::printStatusToSerial() {
  long pulses = getPulseCount();
  double revs = getRevolutions();
  double angle = getAngularPositionDeg();
  double rpm = getRPM();

  Serial.print("PulseCount:"); Serial.print(pulses);
  Serial.print("\tRevs:"); Serial.print(revs, 6);
  Serial.print("\tAngle(deg):"); Serial.print(angle, 2);
  Serial.print("\tRPM:"); Serial.println(rpm, 2);
}

// ---------------- ISRs ----------------
// Quadrature decoding using CHANGE on both channels.
// Quy tắc:
//  - Trên cạnh thay đổi của A: nếu A != B => +1, else -1
//  - Trên cạnh thay đổi của B: nếu A == B => +1, else -1
// Điều này cho phép đếm X4 và xác định chiều quay.

void isrA() {
  bool a = digitalRead(pinA_);
  bool b = digitalRead(pinB_);
  if (a != b) {
    pulseCountMotor_++;
  } else {
    pulseCountMotor_--;
  }
}

void isrB() {
  bool a = digitalRead(pinA_);
  bool b = digitalRead(pinB_);
  if (a == b) {
    pulseCountMotor_++;
  } else {
    pulseCountMotor_--;
  }
}

