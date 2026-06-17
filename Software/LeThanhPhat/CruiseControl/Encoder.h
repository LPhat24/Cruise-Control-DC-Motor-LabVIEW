/*
  Encoder.h
  Encapsulates encoder handling for Cruise Control project.

  Quy ước và tính năng:
  - Đếm xung theo quadrature (kênh A/B) bằng 2 interrupt (CHANGE)
  - Tính số xung tích luỹ (pulse count)
  - Tính số vòng (revolutions) của trục đầu ra (sau tỉ số truyền)
  - Tính góc quay (degrees) tương ứng với vị trí
  - Tính RPM bằng phương pháp lấy delta xung theo khoảng mẫu (non-blocking)
  - Lọc trung bình cho RPM (moving average)

  Thiết kế API tuân thủ yêu cầu: tất cả xử lý nằm trong Encoder.*; .ino chỉ chứa include, setup(), loop().

  Ghi chú kỹ thuật (tóm tắt):
  - Encoder pulses per channel (P): số xung trên 1 vòng motor cho 1 kênh (A hoặc B).
  - Quadrature X4 decoding: Pulses per motor revolution = P * 4 (đếm cả 4 cạnh).
  - Pulses per output revolution = (P * 4) * gear_ratio.
  - Revolutions = pulse_count / pulses_per_output_rev.
  - Angle (degrees) = fmod(revolutions, 1.0) * 360.
  - RPM computation: rpm = (delta_pulses / pulses_per_output_rev) * (60 / dt_seconds)

  Tất cả hàm công khai có comment mô tả chức năng.
*/


#ifndef ENCODER_MODULE_H
#define ENCODER_MODULE_H

#include <Arduino.h>

// Class-based API: tạo một instance `Encoder` và gọi bằng dấu chấm `.`
// Ví dụ: Encoder.begin(...); Encoder.update(); Encoder.getRPM();

class EncoderClass {
public:
  // Khởi tạo module encoder
  // pinA, pinB: chân nối encoder (phù hợp với board; trên Uno thường dùng 2 và 3)
  // pulsesPerChannel: số xung trên 1 vòng motor cho 1 kênh (ví dụ 11)
  // gearRatio: tỉ số truyền motor:output (ví dụ 46.8)
  void begin(uint8_t pinA, uint8_t pinB, uint16_t pulsesPerChannel, float gearRatio);

  // Cập nhật module (gọi trong loop(), non-blocking).
  // Thực hiện tính toán RPM trung bình theo khoảng mẫu.
  void update();

  // Trả về số xung tích luỹ (signed: có thể âm nếu đổi chiều)
  long getPulseCount();

  // Trả về số vòng (revolutions) ở trục đầu ra (sau tỉ số truyền)
  double getRevolutions();

  // Trả về góc quay tính bằng độ [0..360)
  double getAngularPositionDeg();

  // Trả về RPM tuyệt đối (>=0)
  double getRPM();

  // In trạng thái (pulse count, rev, angle, rpm) ra Serial (human readable)
  void printStatusToSerial();
  
    // Set pulses counted for one full output-shaft revolution (calibration).
    // Use this if the mechanical gear ratio is not exact or has rounding.
    // Call with the integer pulse count observed when rotating the output shaft exactly 360 degrees.
    void setPulsesPerOutputRev(unsigned long pulses);
};

// Một instance toàn cục (file-scope) để dùng dấu chấm.
extern EncoderClass Encoder;

#endif // ENCODER_MODULE_H
