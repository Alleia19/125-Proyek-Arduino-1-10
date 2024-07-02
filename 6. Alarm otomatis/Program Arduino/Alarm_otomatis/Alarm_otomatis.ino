/*************************************
 * Program : Project 6. Alarm otomatis
 * Input   : 4 Tombol push on, Module RTC DS1307 
 * Output  : LCD 2x16, BUZZER
 * 125 Proyek Arduino Inkubatek
 * www.tokotronik.com
 * ***********************************/
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "Wire.h"
#define DS1307_ADDRESS 0x68
byte zero = 0x00;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

byte second ,minute,hour, weekDay;
byte monthDay,month,year;
byte minuteOn,hourOn,minuteOff,hourOff;
byte f_tombol, f_alarm, f_key, menu;
byte keluar=1;
byte setRTC;

boolean up=false;
boolean down=false;

void setup(){
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,HIGH); 
  digitalWrite(11,HIGH); 
  pinMode(12,OUTPUT);   
  lcd.begin(16, 2);
  lcd.print(" Alarm ARDUINO");
  delay(2000);
  lcd.clear();
  Wire.begin();
  //-----------baca alarm
  hourOn = EEPROM.read(0);
  minuteOn = EEPROM.read(1);
  hourOff = EEPROM.read(2);
  minuteOff = EEPROM.read(3);
  //-----------jika belum ada alarm
  if(hourOn==255)hourOn=0;
  if(minuteOn==255)minuteOn=0;
  if(hourOff==255)hourOff=0;
  if(minuteOff==255)minuteOff=0;

 /* 
 //-------------- seting jam
  hour=12;
  minute=30;
  second=0;
  weekDay=1;
  monthDay=29;
  month=4;
  year=16;
  setingRTC();
  */
  
}
void loop(){
  bacaRTC();
  lcdDisplay();
  lcd.setCursor(0,1);
  lcd.print("Alarm ");
  lcd.print(hourOn);
  lcd.print(":");
  lcd.print(minuteOn);
  lcd.print(":0  ");
  //---------- jika waktu sama dengan jam alarm ON maka alarm ON
  if(hourOn==hour && minuteOn==minute && f_alarm==0){
    digitalWrite(12,HIGH);
    f_key=1;
  }
  //---------- jika waktu sama dengan jam alarm OFF maka alarm OFF
  else if(hourOff==hour && minuteOff==minute){
    digitalWrite(12,LOW);
    f_alarm=0;
    f_key=0;
  }
  //---------- jika Tombol matikan alarm di tekan maka alarm OFF
  if(digitalRead(11)==0){
    digitalWrite(12,LOW);
    f_alarm=1;
    f_key=0;
  }
  delay(200);
  if(f_key==0){
    cekTombol();
  }
}

void cekTombol(){
    if (digitalRead(8)==0){
      do{
        f_tombol++;
        delay(100);
        if(f_tombol>20)goto lanjut;
      }
      while(!digitalRead(8));
      
      lanjut:
      if(f_tombol<20){
        setRTC=1;
        lcd.clear();
        lcd.print("Set Alarm");
        delay(2000);
        lcd.clear();  
      }
      else{
        lcd.clear();
        lcd.print("Seting Jam");
        delay(2000);
        lcd.clear();
        setRTC=0;
      }
      f_tombol=0;
      lcdDisplay();
      menu++;
      lcd.blink();
      lcd.setCursor(7,0);
      do{
        //------------------------next
        if (digitalRead(8)==0){
          delay(300);
          menu++;
        }
        //---------------------------up
        else if(digitalRead(9)==0){
          delay(200);
          up=true;
        }    
    //----------------------------down      
        else if(digitalRead(10)==0){
          delay(200);
          down=true;
        }
   //-------------------------edit data jam       
        if (menu==1){
          if (up==true && hour < 24){
              hour++;
              lcdDisplay();
          }
          else if (down==true && hour>0){
              hour--;
              lcdDisplay();
          }
          up=false;
          down=false;
          if (hour<10){            
            lcd.setCursor(6,0);
          }
          else  {         
            lcd.setCursor(7,0);
          }
        }
          
   //-------------------------edit data menit
        else if (menu==2){      
          if (up==true && minute < 59){   
              minute++;
              lcdDisplay();
            }
          else if (down==true && minute>0){
             minute--;
             lcdDisplay();
          }
             up=false;
             down=false;
             
          if (hour<10 && minute<10){            
            lcd.setCursor(8,0);
          }
          else if (hour<10 && minute>9){          
            lcd.setCursor(9,0);
          }
          else if (hour>9 && minute<10){            
            lcd.setCursor(9,0);
          }
          else if (hour>9 && minute>9){        
            lcd.setCursor(10,0);
          }
        }
          
   //-------------------------edit data detik  
        else if (menu==3){
          if (up==true && second < 59){   
              second++;
              lcdDisplay();
            }
          else if (down==true && second>0){
             second--;
             lcdDisplay();
          }
          up=false;
          down=false;
             
          if (hour<10 && minute<10 && second<10){            
            lcd.setCursor(10,0);
          }
          else if (hour<10 && minute<10 && second>9){          
            lcd.setCursor(11,0);
          }
          else if (hour<10 && minute>9 && second<10){          
            lcd.setCursor(11,0);
          }
          else if (hour>9 && minute<10 && second<10){          
            lcd.setCursor(11,0);
          }
          else if (hour<10 && minute>9 && second>9){          
            lcd.setCursor(12,0);
          }
          else if (hour>9 && minute>9 && second<10){          
            lcd.setCursor(12,0);
          }
          else if (hour>9 && minute<10 && second>9){          
            lcd.setCursor(13,0);
          }
          else if (hour>9 && minute>9 && second>9){          
            lcd.setCursor(13,0);
          }
        }
        
   //-------------------------simpan data Jam  
        if (setRTC==0 && menu==4){
          setingRTC();
          lcd.clear();
          lcd.noBlink();
          lcd.print("Seting Jam OK!");
          delay(1000);
          lcd.clear();
          keluar=0;
        }
   //-------------------------simpan data Alarm
   //----------------------selisih alarm Off adalah 5 menit
        if(menu==3 && setRTC==1){
            hourOn=hour;
            minuteOn=minute;
            hourOff=hour;
            minuteOff=minute+5;   //penambahan jeda alarm off
            if(minuteOff>60){
              hourOff=hourOff+1;
              minuteOff=minuteOff-60;
            }
            lcd.noBlink();
            lcd.clear();
            lcd.print("On =");
            lcd.print(hourOn);
            lcd.print(":");
            lcd.print(minuteOn);
            lcd.setCursor(0,1);
            lcd.print("Off=");
            lcd.print(hourOff);
            lcd.print(":");
            lcd.print(minuteOff);
            //------------simpan EEPROM
            EEPROM.write(0, hourOn);
            EEPROM.write(1, minuteOn);
            EEPROM.write(2, hourOff);
            EEPROM.write(3, minuteOff);
            delay(3000);
            lcd.clear();
            lcd.print("Set Alarm OK!");
            delay(1000);
            lcd.clear();
            setRTC=0;
            keluar=0;
          }
      }
      while(keluar);
      keluar=1;
      menu=0;
    }
}

byte decToBcd(byte val){
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
  return ( (val/16*10) + (val%16) );
}

void bacaRTC(){
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read() & 0b111111);
  weekDay = bcdToDec(Wire.read());
  monthDay = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

void lcdDisplay(){
  if (setRTC==1){    
    lcd.setCursor(0,0);
    lcd.print("Jam   ");
    lcd.print(hour);
    lcd.print(":");
    lcd.print(minute); 
    lcd.print("  ");
  }
  else{
    lcd.setCursor(0,0);
    lcd.print("Jam   ");
    lcd.print(hour);
    lcd.print(":");
    lcd.print(minute);
    lcd.print(":");
    lcd.print(second);
    lcd.print("    ");
  }
} 

void setingRTC(){
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); //stop RTC
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(zero); //start 
  Wire.endTransmission();
}

