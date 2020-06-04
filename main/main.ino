#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//===============================================================
Servo myservo;
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte val[6];
const int place[]={13,12,11,10,9,8};
const int ir_in = 7;
const int ir_out = 6;
const int gate_in = 5;
const int gate_out = 4;
int count = 0;
int valin = 0;
int valout = 0;
int pos = 0; 
int cnt;
int y=0;
//===============================================================
void openGate(int gate){
  myservo.attach(gate);
  myservo.write(90);
  delay(1000);
  myservo.detach();
}
//====================================
void closeGate(int gate){
  myservo.attach(gate);
  myservo.write(0);
  delay(1000);
  myservo.detach();
}
//====================================
void check_in(){
  valin=digitalRead(ir_in);
  if(valin==LOW){
    if(count<6){
      openGate(gate_in);
      while(valin ==LOW){
        valin=digitalRead(ir_in);
      }
      count++;
      delay(1000);
      closeGate(gate_in);
    }
    else if(count>=6){
      count=6;
      myservo.write(0);
      myservo.detach();
    }
  }
}
//====================================
void check_out(){
  valout=digitalRead(ir_out);
  if(valout==LOW){
    lcd2.print("Parking cost: ");
    if(count>0){
      openGate(gate_out);
      while(valout==LOW){
        valout=digitalRead(ir_out);
      }
      count--;
      delay(1000);
      closeGate(gate_out);
      lcd2.clear();
    }
    else if(count<=0){
      count=0;
    }
  }
}
//====================================
void lcd_in(){
  if(count<6){
    lcd1.setCursor(0, 0);
    lcd1.print("Empty: ");
    for( y=0;y<=5;y++){
      val[y]=digitalRead(place[y]);
      if(val[y]==1){
        lcd1.print(y+1);
        lcd1.print(",");
      }
    }
    lcd1.print("             ");
  }
  else if(count>=6){
    lcd1.setCursor(0, 0);
    lcd1.print("Parking lot is full.");
    lcd1.print("             ");
  }
  lcd1.setCursor(0, 1);
  lcd1.print("Parked car: ");
  lcd1.print(count);
}
//====================================
void lcd_out(){
  
}
//===============================================================
void setup() {
  Serial.begin(9600);
  for(int i=0;i<5;i++){
    pinMode(place[i], INPUT);
  }
  pinMode(ir_in, INPUT);
  pinMode(ir_out, INPUT);
  
  lcd1.begin(16, 2);
  lcd1.setCursor(0, 0);
  lcd1.print("Auto Parking");
  lcd1.setCursor(0, 1);
  lcd1.print("Lot System");
  lcd1.setCursor(0, 0);

  lcd2.begin(16, 2);
  lcd2.setCursor(0, 0);
  lcd2.print("SBK Modul 10");
  lcd2.setCursor(0, 1);
  lcd2.print("Kelompok 10B");
  lcd2.setCursor(0, 0);
  
  closeGate(gate_in);
  closeGate(gate_out);

  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd2.clear();
  lcd2.setCursor(0, 0);
}
//===============================================================
void loop() {
  check_in();
  check_out();
  lcd_in();
  lcd_out();
}
//===============================================================
