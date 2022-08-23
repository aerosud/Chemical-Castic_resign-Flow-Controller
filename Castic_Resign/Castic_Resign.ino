
#include<EEPROM.h>
 volatile int flow_frequency; 
 float total_vol = 0.0,l_minute;
 float batch_vol = 0.0 ;
 float Total =0.0 ;
 unsigned char flowsensor = 2; // Sensor Input
 unsigned long currentTime;
 unsigned long cloopTime;
 #include <LiquidCrystal.h>
 LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
 const int  set_key = 3;
 const int   up_key = 4;
 const int down_key = 5;
 const int      ent = 6;
 const int    relay1 = 7;
 const int power_fail = A0;
 const int rst = A1;
 const int relay2 = A2;
 int  setvalue=750;
 byte setvalue_check =0;
 unsigned long stored_time;
 int setkey_time = 6000;
 
 void flow () // Interrupt function
 {
   flow_frequency++;
}

 void setup()
 {
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   pinMode(relay1,OUTPUT);  
   pinMode(up_key,INPUT);
   pinMode(down_key,INPUT);
   pinMode(set_key,INPUT);
   pinMode(ent,INPUT);
   pinMode(power_fail,INPUT);
   pinMode(rst,INPUT);
   pinMode(relay2,OUTPUT);
   digitalWrite(A1, LOW);
   digitalWrite(A2, LOW);
   
   Serial.begin(9600);
   
   lcd.begin(16, 2);
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print(" Green Energy ");
   lcd.setCursor(0,1);
   lcd.print(" Projects ");
   currentTime = millis();
   cloopTime = currentTime;
   total_vol = EEPROM.get(0,total_vol);
   Serial.print(analogRead(power_fail));
   lcd.setCursor(12,1);
   lcd.print(analogRead(power_fail));
}

void loop ()
{
   if(analogRead(power_fail) < 920) 
   {
      EEPROM.put(0,total_vol); 
      }
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency /0.201); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      lcd.clear();
     // lcd.setCursor(0,0);
     // lcd.print("FL:");
     // lcd.print(l_minute);
     // lcd.print("LPM");
      l_minute = l_minute/60;      
      batch_vol = batch_vol + l_minute;
      total_vol = total_vol +l_minute;
     // Total = total_vol/1000 ;
      lcd.setCursor(0,0);
      lcd.print("Flow:");
      lcd.print(batch_vol);
      lcd.print(" Lt");
      lcd.setCursor(0,1);
      lcd.print("Cum:");
      lcd.print(total_vol,0);
      lcd.print(" Lt");
      flow_frequency = 0; // Reset Counter
 //     Serial.print(l_minute, DEC); // Print litres/hour
      Serial.print(l_minute,3);
      Serial.println(" L/Sec");
      Serial.println(total_vol,3);
    }
    else {
      Serial.println(" flow rate = 0 ");
      Serial.println(total_vol,3);
     lcd.clear();
      //lcd.setCursor(0,0);
      //lcd.print("FL:");
      //lcd.print( flow_frequency );
      //lcd.print("LPM");
      lcd.setCursor(0,0);
      lcd.print("flow:");
      lcd.print(batch_vol);
      lcd.print(" Lt");
      lcd.setCursor(0,1);
      lcd.print("Cum:");
      lcd.print(total_vol,0);
      lcd.print(" Lt");
    }
   }

if(digitalRead(set_key)==LOW){setvalue_check = 0;}
 //Display Set point on LCD
 while(digitalRead(set_key)==HIGH)
 {
  lcd.setCursor(0,1);
  lcd.print("Set Point:");
  lcd.print(setvalue);
  lcd.print(" Lt   ");
  
 if(digitalRead(down_key)==HIGH)
    {  if(setvalue>0)  { setvalue--; }  }

    
  if(digitalRead(up_key)==HIGH)
    {  if(setvalue<2500)  { setvalue++; }  }
  if(digitalRead(set_key)==HIGH)
    { 
      if(setvalue_check == 0){stored_time = millis();}
      setvalue_check = 1;
      if(digitalRead(set_key)==HIGH && millis()-stored_time > setkey_time){
        delay(10);
      }
      else{
        delay(300);
        }
      }
   
}

 if(batch_vol > (setvalue-0.2 ) )
   {
    digitalWrite(relay1,LOW);
    lcd.setCursor(0,0);
    lcd.print("BatchDone:");
    digitalWrite(relay2,HIGH);
    delay (600);
    digitalWrite(relay2,LOW );
    delay (1000);
   }
  else {digitalWrite(relay2,LOW );}
 
 if(digitalRead(ent)==HIGH){
  batch_vol = 0;
  digitalWrite(relay1,HIGH);
  } 
 //if(digitalRead(rst)==HIGH){
 // total_vol = 0;    }
   
  //Serial.print(analogRead(power_fail));
 // delay(1000);

  
}
