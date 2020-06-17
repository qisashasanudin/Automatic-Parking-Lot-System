#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
//===============================================================
byte val[6];
const int addr_lcd1 = 0x27;
const int addr_lcd2 = 0x26;
const int addr_kpad = 0x25;
const int place[]={13,12,11,10,9,8};
const int ir_in = 7;
const int ir_out = 6;
const int gate_in = 5;
const int gate_out = 4;

long int timestamp[]={0,0,0,0,0,0};
long int price[]={0,0,0,0,0,0};

int count = 0;
int valin = 0;
int valout = 0;
int pos = 0; 
int cnt;
int y=0;

char key;
const byte ROWS = 4;
const byte COLS = 3;
byte rowPins[ROWS] = {0,1,2,3};
byte colPins[ROWS] = {4,5,6};
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

Servo myservo;
LiquidCrystal_I2C lcd1(addr_lcd1, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(addr_lcd2, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
Keypad_I2C kpad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, addr_kpad, PCF8574);

//===============================================================
void openGate(int gate){
  myservo.attach(gate);
  myservo.write(90);
  delay(100);
  myservo.detach();
}
//====================================
void closeGate(int gate){
  myservo.attach(gate);
  myservo.write(0);
  delay(100);
  myservo.detach();
}
//====================================
void intro(){
  lcd1.setCursor(0, 0);
  lcd1.print("Auto Parking");
  lcd1.setCursor(0, 1);
  lcd1.print("Lot System");
  
  lcd2.setCursor(0, 0);
  lcd2.print("SBK Modul 10");
  lcd2.setCursor(0, 1);
  lcd2.print("Kelompok 10B");
  
  closeGate(gate_in);
  closeGate(gate_out);
  delay(2000);
  
  lcd1.clear();
  lcd1.setCursor(0, 0);
  
  lcd2.clear();
  lcd2.setCursor(0, 0);
}
//====================================
void scan_kpad(){
  lcd2.setCursor(0, 0);
  lcd2.print("Lot Number: ");
  lcd2.setCursor(0, 1);
  while(key != '#'){
    key = kpad.getKey();
    switch(key){
      case '1':
        lcd2.print(key);
        lcd_out(0);
        break;
      case '2':
        lcd2.print(key);
        lcd_out(1);
        break;
      case '3':
        lcd2.print(key);
        lcd_out(2);
        break;
      case '4':
        lcd2.print(key);
        lcd_out(3);
        break;
      case '5':
        lcd2.print(key);
        lcd_out(4);
        break;
      case '6':
        lcd2.print(key);
        lcd_out(5);
        break;
      case '7':
        lcd2.print(key);
        lcd_out(10);
        break;
      case '8':
        lcd2.print(key);
        lcd_out(10);
        break;
      case '9':
        lcd2.print(key);
        lcd_out(10);
        break;
      case '0':
        lcd2.print(key);
        break;
      case '*':
        lcd2.print(key);
        lcd_out(10);
        break;
      default:
        break;
    }
  }
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
      
      scan_kpad();
      lcd2.clear();
      lcd2.setCursor(0, 0);
      lcd2.print("Thank you.");
  
      openGate(gate_out);
      while(valout==LOW){
        valout=digitalRead(ir_out);
      }
      count--;
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
void lcd_out(int index){
  if(index == 10){
    lcd2.setCursor(5,0);
    lcd2.print("Invalid Input");
    delay(2000);
    lcd2.clear();
    check_out();
  }
  else{
    if(price[index]==0){
      lcd2.setCursor(5,0);
      lcd2.print("Invalid Input");
      delay(2000);
      lcd2.clear();
      check_out();
    }
    else{
      lcd2.setCursor(5,0);
      lcd2.print(price[index]);
      scan_kpad();
    }
  }
}
//=============================================================== buat cek dan transfer harga
void ir_patrol(){
  for( y=0;y<=5;y++){
    if(digitalRead(place[y]) == 0 and timestamp[y] == 0){
      timestamp[y] = millis();
    }
    else if(digitalRead(place[y])== 1 and timestamp[y]>0){
      price[y] = (timestamp[y]/1000)*100;
      timestamp[y] = 0;
    }
    else{
      continue;
    }
  }
}
//================================================================
void setup() {
  Serial.begin(9600);
  for(int i=0;i<5;i++){
    pinMode(place[i], INPUT);
  }
  pinMode(ir_in, INPUT);
  pinMode(ir_out, INPUT);
  
  lcd1.begin(16, 2);
  lcd2.begin(16, 2);
  kpad.begin();
  
  intro();
}
//===============================================================
void loop() {
  check_in();
  check_out();
  lcd_in();
  ir_patrol();
}
//===============================================================
