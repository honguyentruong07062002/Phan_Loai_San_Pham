#include <Servo.h>
 #include <Q2HX711.h>
 #include <LiquidCrystal_I2C.h>
 LiquidCrystal_I2C lcd(0x27, 16, 2); // địa chỉ i2c của lcd là 0x27, 16 dòng 2 cột
int Lenh_Dieu_Khien=0;
int NgatPhanLoai = 0 ;
Servo myservo1;
Servo myservo2;
Q2HX711 hx771(34,32);
int ThongSo[3]={20,80,200};  // Chọn thông số của 3 mode
int Mode=0;                 // Biển kiểm soát 3 mode hoạt động
int Start=0;                // báo là đã nhấn start hay chưa
int SanPhamLoai1=0;        // đếm sp
int SanPhamLoai2=0;
int SanPhamLoai3=0;
float KhoiLuong;          // biens lưu khối lượng
int Loai=0;               // lưu trữ loại cả chua đã phân loại
int PhanLoai=0;           // để lưu trư đã có ngắt phân loại sản phảm chưa, tại vì trong ngắt không trục tiếp dùng hàm   PhanLoaiSanPham() được
long kl;    // biens lưu khối lượng
 long last; //lưu thời gian cpu hoạt đông, láy dựa trên  hàm  millis()
 int demupdate=0;     //
 int Update=0;    // chọn trường hợp có thay đổi trên lcd hay khônng
#define S0 22
#define S1 24
#define S2 26
#define S3 28
#define BangTai 30
#define sensorOut A0
int R=0;
int G=0;
int B=0;
int colour=0;
int PW=0;
int readColour()
{
  // Setting red photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  int R = PW;
  // Printing the value on the serial monitor
  Serial.print("R = ");         //printing name
  Serial.print(PW);             //printing RED color pulse width
   Serial.println("  ");
  delay(50);
  // Setting Green photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  int G = PW;
  // Printing the value on the serial monitor
  Serial.print("G = ");          //printing name
  Serial.print(PW);              //printing GREEN color pulse width
  Serial.println("  ");
  delay(50);
  // Setting Blue photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  PW = pulseIn(sensorOut, LOW);  // Reading the output pulse width
  int B = PW;
  // Printing the value on the serial monitor
  Serial.print("B = ");          //printing name
  Serial.print(PW);              //printing BLUE color pulse width
  Serial.print("   colour: ");   
   Serial.println("  ");
  delay(50);
  if(R < 115 && R > 50 && G < 260 && G> 160 && B < 240 && B > 140 ) //// thay so
  {
    colour = 1;                  Serial.prrintln("Mau Do"); 
  }
  else
  {
    colour = 2;     Serial.prrintln("Mau khac"); 
    }
  return colour;
  }
 void PhanLoaiSanPham()
 {
  lcd.clear();
  digitalWrite(BangTai,0);
  delay(500);
  
     DocTrungBinh(); 
   Serial.println(kl);
   lcd.setCursor(0, 0);
    lcd.print("KHOI LUONG:");
    lcd.setCursor(12,0);
    lcd.print(kl);
    lcd.setCursor(0,1);
    lcd.print("LOAI: ");
    lcd.setCursor(12,1);
    
  if(kl<ThongSo[Mode-1])
  {
    Loai=3;
     Serial.print("Loai: ");
  Serial.println(Loai);
  lcd.print(Loai);
  
   }
   else
   {
    if(readColour()==1)
    {
    Loai=1;
      Serial.print("Loai: ");
       Serial.println(Loai);
       lcd.print(Loai);
    }
    else 
    {
      Loai=2;
      Serial.print("Loai: ");
      Serial.println(Loai);
      lcd.print(Loai);
     }
   }
   delay(2000);
   lcd.clear();
   UpdateSanPham();
 digitalWrite(BangTai,1);
  }
 void UpdateSanPham()
 {
  Serial.println("DA Update");
  demupdate++;
   lcd.setCursor(0, 0);
    lcd.print("  L1   L2   L3  ");
    lcd.setCursor(2, 1);
    lcd.print(SanPhamLoai1);
    lcd.setCursor(7, 1);
    lcd.print(SanPhamLoai2);
    lcd.setCursor(12, 1);
    lcd.print(SanPhamLoai3);
 }
 void Chon_Mode()
 {
  if(Start==0)
  { digitalWrite(BangTai,0);
    lcd.print("CHON MODE: ");
  while(1)
  {
    if(digitalRead (4)==0)
    {
      delay(30);
      if(digitalRead (4)==0)
    {
      while(digitalRead (4)==0);
         lcd.setCursor(12, 0);
         lcd.print("1");
       Serial.println("Mode 1");
      Mode=1;
      break;
      }   
    }

    else if(digitalRead (5)==0)
    {
      delay(30);
      if(digitalRead (5)==0)
    {
      while(digitalRead (5)==0);
      Serial.println("Mode 2");
        lcd.setCursor(12, 0);
         lcd.print("1");
      Mode=2;
      break;
      }   
    }
    else if(digitalRead (6)==0)
    {
      delay(30);
      if(digitalRead (6)==0)
    {
      while(digitalRead (6)==0);
      Serial.println("Mode 3");
        lcd.setCursor(11, 0);
         lcd.print("1");
      Mode=3;
      break;
      }   
    }
    }
       lcd.setCursor(0, 1);
         lcd.print("    START");
   while(digitalRead(7)==1);
   lcd.clear();
   UpdateSanPham();
    digitalWrite(BangTai,1);
   Start=1;
    while(digitalRead(7)==0);
  }
  }

void Ngat_Do_Cam_Bien()
{
  
  if(millis()-last>3000)
  {
  if(NgatPhanLoai==0)
    {
    PhanLoai = 1;
    NgatPhanLoai=1;
    }
  
   last=millis();
  }
}
void Ngat_Servo_Loai3() //gạt servo 2 , tăng đếm loại 3 lên 1 và hiện lên lcd
{
  
   if(millis()-last>3000)
  {
     Serial.println("Ngắt loại 3");
     delay(10);
    if(Loai==3)
    {
      Serial.println("Chọn loại 3");
       SanPhamLoai3++;
  Serial.println(SanPhamLoai3); 
     Update=1;// để xét  truonfg họp có  tháy đôi trên lcd hay kh
  
     Lenh_Dieu_Khien=3;  // 
    }
   last=millis();
  }
}
void DocTrungBinh()
{
  kl=0;
  for(int i=0;i<10;i++)
  {
    kl+=hx771.read();
    delay(50);
  }
 kl = (kl/10 - 8038940)/1244;
  
 
}
void Ngat_Servo_Loai2() // gạt servo 2 , tăng đếm loại 2 lên 1 và hiện lên lcd
{
 
   if(millis()-last>3000)
  {
    Serial.println("Ngắt loại 2");
     Lenh_Dieu_Khien=2; 
     
    if(Loai==2)
    {
    Serial.println("Chọn loại 2");
     SanPhamLoai2++;
    
   Update=1;
  
    }
    
    last=millis();
  }
}

void setup()
{
    Serial.begin(9600);
     pinMode(BangTai, OUTPUT);
   pinMode(S0, OUTPUT); 
    pinMode(S1, OUTPUT);
     pinMode(S2, OUTPUT);
      pinMode(S3, OUTPUT);
       pinMode(sensorOut, INPUT);
    digitalWrite(S0,HIGH);
     digitalWrite(S1,LOW);
   pinMode(4, INPUT_PULLUP);
   pinMode(5, INPUT_PULLUP);
   pinMode(7, INPUT_PULLUP);
   pinMode(6, INPUT_PULLUP);
    lcd.init();
  lcd.backlight();
   pinMode(2, INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    attachInterrupt(0, Ngat_Do_Cam_Bien, FALLING);
    
     pinMode(3, INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    attachInterrupt(1, Ngat_Servo_Loai3, FALLING);
    
    pinMode(19, INPUT_PULLUP); // sử dụng điện trở kéo lên cho chân số 2, ngắt 0
    attachInterrupt(4, Ngat_Servo_Loai2, FALLING);
    last=millis();
      myservo1.attach(10);
        myservo2.attach(11);  
      // Cấu hình load cell
 
        
}
// 0- 79575   190 - 77155
void loop()
{
while(1)
{


   Chon_Mode();
 
  // Control(Lenh_Dieu_Khien);
  if(PhanLoai==1)
  {
    
     PhanLoaiSanPham();//trả về biên Loai = 1,2,3
     PhanLoai=0;
     }
     
   if(Update==1)
   {
        UpdateSanPham();
      Serial.println(demupdate);
      Update=0;
   }

   
   if(Lenh_Dieu_Khien==2)
   { NgatPhanLoai=0;
     if( Loai==1)
    {
        Serial.println("Update loại 1");
        SanPhamLoai1++;
        UpdateSanPham();
        Loai=0;
        Lenh_Dieu_Khien=0;
      }
      else if (Loai==2)
      {
     myservo1.write(0);             // Đưa servo về góc 90 độ
  delay(4000);    
  myservo1.write(90);// Chờ 1 giây
 Lenh_Dieu_Khien=0;
    Loai=0;
   }
    }
    else if(Lenh_Dieu_Khien==3)
   {
     NgatPhanLoai=0;
     myservo2.write(45);             // Đưa servo về góc 90 độ
  delay(4000);    
  myservo2.write(90);// Chờ 1 giây
 Lenh_Dieu_Khien=0;
    Loai=0;
    }
    if(digitalRead(7)==0)   /// nút start / stop
    {
       NgatPhanLoai=0;
    delay(30);
    if(digitalRead(7)==0)
    { while(digitalRead(7)==0);
      Start=0;
      lcd.clear();
      }
    }
  
    
}
}