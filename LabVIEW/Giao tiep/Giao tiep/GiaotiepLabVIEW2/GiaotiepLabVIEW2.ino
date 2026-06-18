
String chuoiNhan;
char kytuNhan;

//tên các biến được đặt giống trên LabVIEW
int32_t AguiMT, BguiMT;
int32_t AnhanMT, BnhanMT;

int8_t dem, dem2;
//===========================================
uint32_t tdHientai, tdTruoc, chuky;
uint16_t tanso, demXung;

void ngatngoai1(void)
{
  demXung ++;
  tdTruoc = tdHientai;
  tdHientai = micros();
  chuky = tdHientai - tdTruoc;
}
//===========================================
//hàm gửi 2 biến A B lên labVIEW
//chỉ gửi khi gặp dấu # tức sau khi kết thúc chuỗi nhận
//xem dòng lệnh 41 bên dưới
void guiLV(int32_t soA, int32_t  soB)
{
      Serial.print("a");  
      Serial.print(soA);  
      Serial.print("b");  
      Serial.print(soB);  
      Serial.println("#");  
}
//==================================================
void setup() {
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(3), ngatngoai1, FALLING);
  Serial.begin(115200);
}
//==================================================
void loop() {
  if ((micros()-tdHientai) <= 1000000 && chuky > 0) {tanso = 1000000/chuky;} else {tanso = 0;} 
  AguiMT = analogRead(A6);
  // BguiMT = analogRead(A7);
  BguiMT = tanso;
  analogWrite(5,AnhanMT);
  analogWrite(6,BnhanMT);

  while (Serial.available()>0) {
    kytuNhan = Serial.read();
    if(kytuNhan=='a') {
      chuoiNhan=""; //xóa để chuẩn bị nhận biến A
    } 
    else if(kytuNhan=='b') {
      AnhanMT=chuoiNhan.toInt();
      chuoiNhan=""; //xóa để chuẩn bị nhận biến B
    } 
    else if(kytuNhan=='#') {
      BnhanMT=chuoiNhan.toInt();
      chuoiNhan="";
      guiLV(AguiMT,BguiMT); //gửi sau khi kết thúc chuỗi nhận #
    } 
    else {chuoiNhan += kytuNhan;}
  }

  //------------------------------------------
  //viết các lệnh cập nhật dữ liệu máy tính và xử lý code chương trình
  // AguiMT=AnhanMT; BguiMT=BnhanMT; //ví dụ nhận được gì gửi về đó
  //------------------------------------------

  delay(10); //thời gian này lớn hơn hoặc bằng 10 tùy số byte gửi nhận
}
