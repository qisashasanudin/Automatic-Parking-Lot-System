#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//===============================================================
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

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
  myservo.write(0);
  for (pos = 0; pos <= 90; pos += 1){
    myservo.write(pos);
    delay(15);
  }
  myservo.detach();
}
//====================================
void closeGate(int gate){
  myservo.attach(gate);
  for (pos = 90; pos >= 0; pos -= 1){
    myservo.write(pos);
    delay(15);
  }
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
  if(count>0){
    openGate(gate_out);
    while(valout==LOW){
      valout=digitalRead(ir_out);
    }
    count--;
    delay(1000);
    closeGate(gate_out);
    }
    else if(count<=0){
      count=0;
    }
  }
}
//====================================
void lcd_in(){
  if(count<6){
    lcd.setCursor(0, 0);
    lcd.print("Empty: ");
    for( y=0;y<=5;y++){
      val[y]=digitalRead(place[y]);
      if(val[y]==1){
        lcd.print(y+1);
        lcd.print(",");
      }
    }
    lcd.print("             ");
  }
  else if(count>=6){
    lcd.setCursor(0, 0);
    lcd.print("Parking lot is full.");
    lcd.print("             ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Parked car: ");
  lcd.print(count);
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
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Auto Parking");
  lcd.setCursor(0, 1);
  lcd.print("Lot System");
  lcd.setCursor(0, 0);
  
  openGate(gate_in);
  openGate(gate_out);
  closeGate(gate_in);
  closeGate(gate_out);

  lcd.clear();
  lcd.setCursor(0, 0);
  count=0;
}
//===============================================================
void loop() {
  check_in();
  check_out();
  lcd_in();
  lcd_out();
}
//===============================================================
