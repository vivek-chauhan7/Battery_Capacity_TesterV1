/*
 *  DIY Battery Capacity tester v1.0 
 *  Auther: ER BROS LAB
 *  YouTube: https://www.youtube.com/channel/UCY6KA8oFk1s36fyPiFEmedQ
 *  Instagram: https://instagram.com/erbroslab?igshid=yuf7g4tevsro
 *  Email: erbrother1@gmail.com
 */

#include <LiquidCrystal.h>

const int rs = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define vo A0
#define am A1
#define buzz 2
#define en 3
#define enout 4
// -------variables and constants----------- //
float amp = 0.00;
float temp = 0.00;
float batv = 0.00;
float r1 = 21660.0; //voltage devider for measurement 
float r2 = 9770.0;  
float eref = 2.517; //voltage referance tl431
float a = 0.00;
float b = 0.00;
int c = 0;
int count = 0;
bool lg1 = false;
bool lg2 = false;
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
float Capacity = 0.0;   //Variable to define the battery Capacity
float mA;

void setup() {
  analogReference(EXTERNAL);
  //Serial.begin(9600);  
  lcd.begin(16, 2);
  pinMode(vo, INPUT);
  pinMode(am, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(enout, OUTPUT);
  pinMode(en, INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Battery Capacity");
  lcd.setCursor(0, 1);
  lcd.print("Tester");
  delay(2000);
  lcd.clear();
}

void loop() {
  //...........voltage measurements............
  a = analogRead(vo);
  temp = (a * eref) / 1024;
  //voltr = volt - 0.01;  // voltage adjestment if you need then uncomment and adjest it 
  batv = temp / (r2 / (r1 + r2));
  //Serial.print("vol: "); 
  //Serial.println(volt);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(batv);
  //............current measurements...............
  b = analogRead(am);
  amp = ((b * eref) / 1024) / 1.036;
  //Serial.print("amp: "); 
  //Serial.println(amp);
  lcd.setCursor(0, 1);
  lcd.print("A: ");
  lcd.print(amp);
  //-----capacity print on display---
  //Serial.print("Bat. capacity in mAh: "); 
  //Serial.println(Capacity);
  lcd.setCursor(8, 0);
  lcd.print("mAh");
  lcd.setCursor(8, 1);
  lcd.print(Capacity);

  //------- controls-----------
  c = digitalRead(en);
  if (c == HIGH) {
    bz();
    count++;
    delay(200);
  }
  if (count == 1) {
    lg1 = true; // testing is on
  }
  else if (count == 2) {
    lg1 = false; // testing is off
    digitalWrite(enout, LOW);
    count = 0;
  }
  if ( batv >= 2.8 && batv <= 4.50) {
    lg2 = true;
  }
  else {
    lg2 = false;
  }
  if (lg1 == true && lg2 == true) { 
    digitalWrite(enout, HIGH);
    millisPassed = millis() - previousMillis;
    mA = amp * 1000.0 ;
    Capacity = Capacity + (mA * (millisPassed / 3600000.0)); // 1 Hour = 3600000ms to convert it into mAh units
    previousMillis = millis();
  }


  if (batv < 2.8) { // low voltage function
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Done");
    lcd.setCursor(0, 1);
    lcd.print("LOW volt");
    lcd.setCursor(8, 0);
    lcd.print("mAh");
    lcd.setCursor(8, 1);
    lcd.print(Capacity);
    count = 0;
    digitalWrite(enout, LOW);
    buzzlowbat();
    lcd.clear();
  }
  delay(200);
}

void bz() {
  digitalWrite(buzz, HIGH);
  delay(80);
  digitalWrite(buzz, LOW);
}
void buzzlowbat() {
  digitalWrite(buzz, HIGH);
  delay(100);
  digitalWrite(buzz, LOW);
  delay(100);
  digitalWrite(buzz, HIGH);
  delay(100);
  digitalWrite(buzz, LOW);
  delay(1000);
}
