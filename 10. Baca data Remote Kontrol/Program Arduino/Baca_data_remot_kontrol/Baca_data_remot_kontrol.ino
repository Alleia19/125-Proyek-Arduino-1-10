/*************************************
 * Program : Project 10. Baca data Remote Kontrol
 * Input   : Sensor Remot 
 * Output  : LCD 2x16
 * 125 Proyek Arduino Inkubatek
 * www.tokotronik.com
 * ***********************************/
#include <IRremote.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

int IR_PIN = 8;
IRrecv irrecv(IR_PIN);
decode_results results;

void setup(){
  lcd.begin(16, 2);
  irrecv.enableIRIn();
}

void loop(){
  lcd.setCursor(0,0);
  lcd.print("BacaData IRremot");
  lcd.setCursor(0,1);
  lcd.print("Data:");
  if (irrecv.decode(&results)) {
    irrecv.resume();
    if(results.value != 0xFFFFFFFF){
      lcd.print(results.value,HEX);
      lcd.print("    ");
    }
  }
}
