#include <Servo.h>
Servo myservo;

int y=0; 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int place[]={13,12,11,10,9,8};
byte val[6];
const int in = 7;
const int out = 6;
int servopin = 5;
int count = 0;
int valin = 0;
int valout = 0;
int pos = 0; 
int cnt;

void setup() {
  Serial.begin(9600);
  for(int i=0;i<5;i++){
    pinMode(place[i], INPUT);
  }  
     
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  myservo.attach(servopin); 
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Auto Parking");
  lcd.setCursor(0, 1);
  lcd.print("Lot System");
  lcd.setCursor(0, 0);
  
  myservo.write(0);

  for (pos = 0; pos <= 90; pos += 1){
    myservo.write(pos);
    delay(15);
  }
  for (pos = 90; pos >= 0; pos -= 1){
    myservo.write(pos);
    delay(15);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  count=0;
}

void loop() {
 //******************************************************* 
  valin=digitalRead(in);
  valout=digitalRead(out);
 //********************************************************
  if(valout==LOW){
    myservo.attach(servopin);
    for (pos = 0; pos <= 90; pos += 1){
      myservo.write(pos);
      delay(15);
    }
    while(valout==LOW){
      valout=digitalRead(out);
    }
    count--;
    if(count<=0){
      count=0;
    }
    delay(1000);
    for (pos = 90; pos >= 0; pos -= 1){
      myservo.write(pos);
      delay(15);
    }
  }
//********************************************************
  if(valin ==LOW){
    if(count<6){
      myservo.attach(servopin);
      for (pos = 0; pos <= 90; pos += 1){
        myservo.write(pos);
        delay(15);
      }
      while(valin ==LOW){
        valin=digitalRead(in);
      }
      count++;
      delay(1000);      
      for (pos = 90; pos >= 0; pos -= 1){
        myservo.write(pos);
        delay(15);
      }
    }
    else if(count>=6){
      count=6;
      myservo.write(0);
      myservo.detach();
    }
  }
//********************************************************
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
    lcd.print("Parking lot is full");
    lcd.print("             ");
  }
  lcd.setCursor(0, 1);
  lcd.print("Parked car: ");
  lcd.print(count);
//********************************************************
}
