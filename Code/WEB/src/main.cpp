#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiScan.h>
#include <web.h>


extern String ssid;
extern String password;
extern String NetioIP;


// void setup(){
//   Serial.begin(9600);
//   setWiFiServer();
// }

// void loop() {
//     handleServer();
// }


//#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <web.h>


// konstanty pripojeni a json zpravy
//const char* HTTP_CONNECTION = "http://192.168.0.106/netio.json";
const char* HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";

//const int buttton = 16;
const int buzzer = 13;
const int LED = 12;
const int button = 5;
// promenne
int buttonState = 0;
bool check = true;

HTTPClient http;


void WiFiConnect(){
    Serial.println("WIfiIII");
    if(WiFi.status() != WL_CONNECTED)
    setWiFiServer();
}


void buzzerTimer(int duration){ 
  // Bzucak na oznameni stavu
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(buzzer, LOW);
  delay(100);
}

void httpPost(){
  // odesilani requestu
  String HTTP_CONNECTION = "http://";
  HTTP_CONNECTION += NetioIP;
  HTTP_CONNECTION += "/netio.json";
  if(http.begin(HTTP_CONNECTION)){
      http.addHeader("Content-Type", "text/plain"); // nastaveni headeru na klasicky test
      int httpCode = http.POST(HTTP_REQUEST); // odeslani json requestu
      String payload = http.getString(); // zjisteni zpetne vazby
      
      if(payload =="\"errors\":[{\"code\":400,\"message\":\"Bad request\"}"){
        for(int i=0; i<2;i++){
          buzzerTimer(200); // json byl odeslan ve spatnem formatu, 2 kratke zabzuceni
        }
      }
      Serial.println(payload);
      http.end(); 
  } else{
     for(int i=0; i<2;i++){
          buzzerTimer(200); // ESP se nepripojilo k zasuvce
        }
    }
}

void wifiCheck(){
  // kontrola zda v dobe requestu jsme pripojeni
  if(WiFi.status() == WL_CONNECTED){
   httpPost(); 
  } else {
    for(int i = 0; i < 3; i++){
      buzzerTimer(300);
    }
  }
}

void buttonCheck(){
  // aby pri delsim podrzenim stav nepreskakoval
  //buttonState = digitalRead();
  if(buttonState == HIGH){
    check = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  for(int i =0; i < 100000; i++)
  Serial.print("test");
  Serial.println(ssid);
  Serial.println(NetioIP);
  WiFi.disconnect();
  pinMode(buzzer, OUTPUT); // nastaveni bzucaku na out
  pinMode(button, INPUT);
  //wifiWPS(); // zapnuti wifi
  WiFiConnect();
  wifiCheck();
}

void loop() {
   //handleServer();
   buttonState = digitalRead(button); // cteni stavu tlacitka
   if(buttonState == LOW && check){
    wifiCheck(); // kontrola wifi zda jsme stale pripojeni
    check = false; // promenna pro kontrolu dlouheho mackani tlacitka
  }
  buttonCheck(); // zmeneni stavu promenne check
}