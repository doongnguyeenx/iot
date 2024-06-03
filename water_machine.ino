#include <ezButton.h> 
#include <Ultrasonic.h>
#include <LCD-I2C.h>


ezButton button1(3); // khai báo nút 100
ezButton button2(4); // khai báo nút 50%

Ultrasonic ultrasoniCup(10, 11); // Khai báo chân echo và trig, siêu âm này sẽ được dùng để kiểm tra xem có đặt cốc vào chưa

Ultrasonic ultrasonicWater(12, 13); // Khai báo chân echo và trig, siêu âm có chức năng kiểm tra mức nước 

LCD_I2C lcd(0x27, 16, 2); // khai báo lcd , địa chỉ i2c là 0x27 

int BOM = 2; // Led được khai báo thay thế cho Bơm

int distanceWater; // khai báo giá trị mức nước
int distanceCup; // khai báo khoảng cách cốc 

int waterPercent = 0; // khai báo % mức nước mong muốn 

void setup() {

  pinMode(BOM, OUTPUT);

  Serial.begin(9600);

  lcd.begin();
  lcd.display();
  lcd.backlight();
}

void loop() {
  button1.loop(); // do sử dụng function button nên phải khai báo 2 hàm này, Nếu thầy hỏi thì bảo là em làm theo hướng dẫn trong thư viện
  button2.loop(); 
  if(button1.isReleased()){ // nếu button 1 được ấn thì đặt % nước là 100 
    waterPercent =100;
  }
  if(button2.isReleased()){// nếu button 2 được ấn thì đặt % nước là 50 . Có thể thay đổi là 70 
    waterPercent =50;
  }
  if(waterPercent != 0){// Nếu giá trị % mức nước đc chọn thì bắt đầu chương trình. Giá trị mặc định là 0 nhưng khi ấn 1 trong 2 nút giá trị waterPercent sẽ đc gán là 50 hoặc 100 
      lcd.clear();// xóa dữ liệu trên màn hình
      distanceCup = ultrasoniCup.read(); // đọc giá trị từ siêu âm Cup
      // Serial.println(distanceCup);
      if(distanceCup < 5){ // nếu giá trị nhỏ hơn 5 cm tức là có cốc đặt vào vị trí thì bắt đầu mở nước 
        distanceWater = ultrasonicWater.read(); // đọc giá trị siêu âm mức nước 
        int distance = (waterPercent*25)/100; // tính giá trị distance. Là giá trị mức nước sẽ được đổ vào cốc. 25 là giá trị nước tối đa trong cốc nước. 100 là 100% 
        Serial.println(distanceWater); // In giá trị distanceWater đo được 
        lcd.setCursor(0, 0);
        lcd.print("Mode: ");
        lcd.setCursor(6, 0);
        lcd.print(waterPercent);// Hiển thị mode được chọn là 50 hoặc 100 
        if(distanceWater <= 30-distance){// nếu mức nước nhỏ hơn 30-distance thì dừng lại. 30 là khoảng cách từ siêu âm Water đến vị trí để cốc(đáy cốc)
          digitalWrite(BOM, LOW);
          waterPercent=0;
          lcd.clear();
        }
        else{ // nếu mức nước > 30-distance thì bật bơm 
          digitalWrite(BOM, HIGH);
        }
      }
      else{ // Nếu giá trị siêu âm Cup > 5cm thì hiển thị lcd là Pull a cup và để bơm ở chế độ tắt 
        digitalWrite(BOM, LOW); // tắt bơm 
        lcd.setCursor(0, 1); // cài đặt tọa độ cho lcd. 0,1 là vị trí 0. Hàng 1. Giá trị hàng đếm từ 0 -15. Cột có 2 cột là 0,1 
        lcd.print("Pull a cup");// Hiển thị lcd 
      }
  }else{
    digitalWrite(BOM, LOW);
    lcd.setCursor(1, 0);
    lcd.print("Water Machine");
  }
  delay(100);
}
