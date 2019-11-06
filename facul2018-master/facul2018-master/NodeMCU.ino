#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid =     "";
const char* password = "";

// LCD
LiquidCrystal_I2C lcd(0x27,16,2);
 
//variáveis auxiliares
float temperatura = 0;
const int pinTemp = 14;
float corrente = 0;
const int pinSensor = 12;
int porta = 0;
int StatusPorta = 0;
const int pinPorta = 13; 
unsigned long tempoAnterior;
unsigned long tempoAtual;
const long pamt = 8000;

float lertemperatura(void){
  float valorTemp;
  float celcius;
  valorTemp = analogRead(pinTemp);
  celcius = (valorTemp-32)/1.8;
  return celcius; 
}

float lercorrente(void){

  
  }
 
float lerporta(void){
  StatusPorta = digitalRead(pinPorta);
  if(StatusPorta == HIGH){ 
    if (tempoAtual-tempoAnterior>=pamt){      
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Porta ");
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
  return StatusPorta;
}
 
//strings para envio dos dados no JSON
String Str1 = "temperatura";
String Str2 = "corrente";
String Str3 = "porta";

//__ Configurações para o IBM Watson
#define DEVICE_TYPE  "ESP8266"
#define DEVICE_ID    "fiap_2018"
#define ORG     "qls64u"
#define TOKEN   "C25RjYtExv8nMwJ9zs"
char server[]   = ORG ".messaging.internetofthings.ibmcloud.com"; 
char topic[]    = "iot-2/type/ESP8266/id/fiap_2018/evt/1-anl/fmt/json"; 
char authMeth[] = "a-qls64u-jijtatpmup";
char token[]    = "K68MjNgrx79En*_8X1";
char clientId[] = "a:" ORG ":" DEVICE_ID;

// WiFi 
WiFiClient wifiClient; 
PubSubClient client(server, 1883, NULL, wifiClient);
 
void setup() {
 Serial.begin(115200);
 pinMode(pinTemp, INPUT);
 pinMode(pinPorta, INPUT);
 pinMode(pinSensor, INPUT);
 lcd.init();   
 lcd.backlight();
 Serial.println(); 
 Serial.print("Conectando ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }

 Serial.println("");
 Serial.print("Conectado, endereço de IP: ");
 Serial.println(WiFi.localIP()); 
} 
 
//Envia p/ IBM 
void enviaDado(float dado1,float dado2, float dado3){
String payload = "{\"d\":{\"" + Str1 + "\":";
 payload += dado1;
 payload += ", \"" + Str2 + "\":";
 payload += dado2;
 payload += ", \"" + Str3 + "\":";
 payload += dado3;
 payload += "}}";
// Serial.print("Sending payload: ");
//Serial.println(payload);
 
// if (client.publish(topic, (char*) payload.c_str())) {
//   Serial.println("Pubicado");
// } else {
//   Serial.println("Publish failed");
// }
}
 
void loop() {
 //Verifica conexão com IBM
 if (!!!client.connected()) {
   Serial.print("Reconectando");
   Serial.println(server);
   while (!!!client.connect(clientId, authMeth, token)) {
     Serial.print(".");
     delay(500);
   }
   Serial.println();
 }
  
 temperatura = lertemperatura();
 corrente = lercorrente();
 porta = lerporta();
  
 enviaDado(temperatura,corrente,porta);
  
 //__ Faz o envio a cada 10 segundos.
 
 delay(10000);
 
}
