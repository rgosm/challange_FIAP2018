#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <ESP8266WiFi.h>

//WIFI
const char* ssid = "";
const char* senha = "";

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); //FUNÇÃO DO TIPO "LiquidCrystal_I2C"

void setup()
{
  lcd.init();   
  lcd.backlight();

  WiFi.begin(ssid,senha);
  lcd.setCursor(0,0);
  lcd.print("conectando");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    lcd.setCursor(0,1);
    lcd.print(".");
    }
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Conectado");
    lcd.setCursor(0,1);
    lcd.print(WiFi.localIP());
}


void loop(){}
