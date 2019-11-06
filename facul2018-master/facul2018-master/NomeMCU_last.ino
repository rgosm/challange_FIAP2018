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
const int pinTemp = 0;
float corrente = 0;
const int pinSensor = 12;
int porta = 0;
int StatusPorta = 0;
const int pinPorta = 13; 
int tempoAnterior;
int tempoAtual;
int tempoP;
int tempo;
const long pamt = 8000;
int teste;


float lertemperatura(void){
  float valorTemp;
  float celcius;
  valorTemp = analogRead(pinTemp);
  celcius = (valorTemp-32)/1.8;
  return celcius; 
}

//float lercorrente(void){

  
//  }
 
int lerporta(void){
  StatusPorta = digitalRead(pinPorta);
  return StatusPorta;
}

int tempoPorta(void){
  if(StatusPorta == HIGH){
    tempoP = tempoAtual-tempoAnterior;
    }
  else{
    if(StatusPorta == LOW && tempoP > 0){
      tempoP = 0;
      }
      else{
    tempoAnterior = tempoAtual;
      }
  }
  return tempoP;
  }
 
//strings para envio dos dados no JSON
String Str1 = "temperatura";
String Str2 = "corrente";
String Str3 = "porta";
String Str4 = "tporta";

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
void enviaDado(float dado1,float dado2, int dado3, int dado4){
String payload = "{\"d\":{\"" + Str1 + "\":";
 payload += dado1;
 payload += ", \"" + Str2 + "\":";
 payload += dado2;
 payload += ", \"" + Str3 + "\":";
 payload += dado3;
 payload += ", \"" + Str4 + "\":";
 payload += dado4;
 payload += "}}";

 Serial.print("Enviando payload: ");
Serial.println(payload);
 
if (client.publish(topic, (char*) payload.c_str())) {
   Serial.println("Pubicado");
 } else {
   Serial.println("Não Publicou");
 }
}
 
void loop() {
 //Verifica conexão com IBM
 if (!!!client.connected()) {
   Serial.print("Reconectando ");
   Serial.println(server);
   while (!!!client.connect(clientId, authMeth, token)) {
     Serial.print(".");
     delay(500);
   }
   Serial.println();
 }
  tempoAtual = (millis()/1000);

 temperatura = lertemperatura();
// corrente = lercorrente();
 porta = lerporta();
 tempo = tempoPorta();
 // tempo = 3;
 enviaDado(temperatura,corrente,porta,tempo);
  
 //Faz o envio a cada 10 segundos.
 
 delay(10000);
 
}
