#include<LiquidCrystal.h>

LiquidCrystal lcd(1,2,4,5,6,7);
const int pinSwitch = 8;
int StatuSwitch = 0;
const int pinSensorTemp=0;
int valueSensorTemp=0;
float celcius;
unsigned long tempoAnterior;
unsigned long tempoAtual;
const long pamt = 8000;

void setup(){
   lcd.begin(16,2);
   pinMode(pinSensorTemp,INPUT);
   pinMode(pinSwitch, INPUT);
   
}

void loop(){
  StatuSwitch = digitalRead(pinSwitch);
  valueSensorTemp = analogRead(pinSensorTemp);
  celcius = (valueSensorTemp-32)/1.8;

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print(celcius,1);
  lcd.print((char)223);
  lcd.print("C");
  delay(1000);
  tempoAtual = millis(); 
  
  if(StatuSwitch == LOW){ 
    if (tempoAtual-tempoAnterior>=pamt){      
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Porta 1 ");
      lcd.setCursor(1,1);
      lcd.print("PAMT");
      delay(1000);
      }
    else{
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Porta");
      lcd.setCursor(1,1);
      lcd.print("Aberta");
      delay(1000);
      }
  }
  else{
    tempoAnterior=tempoAtual;
    }
}
