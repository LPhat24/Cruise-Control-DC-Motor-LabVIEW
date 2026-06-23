# Cruise Control — LabVIEW Project Scaffold

Mục tiêu: tạo Front Panel LabVIEW phong cách Dark/Glassy/Metallic (DSC Image Navigator) cho hệ thống điều khiển quán tính động cơ DC (Encoder backend có sẵn). Repository này chứa mockup và hướng dẫn để bạn dựng VI bằng LabVIEW.

Nội dung:
- `UI_mockup.svg`: mockup giao diện (đã tạo).
- `frontpanel_steps.md`: hướng dẫn chi tiết từng bước dựng Front Panel.
- `labview_integration.md`: cách tích hợp dữ liệu từ `Encoder` (UART) vào VI, mẫu giao thức và kiểm thử.

Yêu cầu phần mềm:
- LabVIEW (phiên bản hỗ trợ DSC Image Navigator — NI Vision/DSC Module nếu dùng các DSC controls).
- NI-VISA (nếu dùng Serial/VISA để đọc dữ liệu từ board).

Tiến trình khuyến nghị:
1. Mở LabVIEW, tạo project mới (.lvproj).
2. Tạo một VI chính `CruiseControl.vi` — Front Panel sẽ là màn hình hợp nhất theo mockup.
3. Thực hiện theo `frontpanel_steps.md` để thêm controls/indicators và áp style.
4. Dùng `labview_integration.md` để nối dữ liệu UART từ `Encoder.printStatusToSerial()` vào VI.
5. Kiểm thử bằng simulator VI (có sẵn trong `labview_integration.md`).

Muốn mình tạo thêm: file VI mẫu (.vi) skeleton, hoặc xuất `UI_mockup.svg` sang PNG để chèn làm background trong Front Panel? Trả lời "VI mẫu" hoặc "PNG" hoặc "Cả hai".