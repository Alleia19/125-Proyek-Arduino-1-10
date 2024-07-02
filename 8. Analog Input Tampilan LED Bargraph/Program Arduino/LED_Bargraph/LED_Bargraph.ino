/*************************************
 * Program : Project 8. Analog Input Tampilan Bargraph LED 
 * Input   : Potensiometer
 * Output  : LED Bargraph 8 LED
 * 125 Proyek Arduino Inkubatek
 * www.tokotronik.com
 * ***********************************/
const int ledCount = 10;   
int adc0; 

int ledPins[] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};   


void setup() {  
  for (int Led = 0; Led < ledCount; Led++) {
    pinMode(ledPins[Led], OUTPUT);
  }
 
}

void loop() {  
  adc0 = analogRead(A0); 
  int ledLevel = map(adc0, 0, 1023, 0, ledCount);
  for (int Led = 0; Led < ledCount; Led++) {   
    if (Led < ledLevel) {
      digitalWrite(ledPins[Led], LOW);
    }
   
    else {
      digitalWrite(ledPins[Led], HIGH);
    }
  }  
}
