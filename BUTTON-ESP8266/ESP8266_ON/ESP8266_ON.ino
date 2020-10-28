#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// konstanty pripojeni a json zpravy
const char* HTTP_CONNECTION = "http://192.168.0.106/netio.json";
const char* HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";

//const int buttton = 16;
const int buzzer = 13;
const int LED = 12;
const int wakePin = 5;
// promenne
int buttonState = 0;
bool check = true;

HTTPClient http;


void buzzerTimer(int duration){ 
  // Bzucak na oznameni stavu
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(buzzer, LOW);
  delay(100);
}

void wifiNormal(){
  // funkce pro pouziti klasicke wifi na testovani
  const char* ssid = "Jiříčkovi_secured";
  const char* password = "19192020";
  WiFi.begin(ssid, password);
}

void wifiWPS(){
  // Nastaveni WiFi a pripojeni WPS
  WiFi.mode(WIFI_STA); // nastaveni wifi do static modu
  delay(500);
  WiFi.begin("","");
  delay(2000);
  if(WiFi.status() != WL_CONNECTED){ // kontrola zda wifi je pripojena
    Serial.println("Connecting...");
    WiFi.beginWPSConfig(); // pokus pripojeni pomoci wps
    delay(3000);
    if(WiFi.status() == WL_CONNECTED){
      buzzerTimer(500);
      Serial.println("Connected");
      Serial.println(WiFi.localIP()); // vypsani localip na serial
      Serial.println(WiFi.SSID());
    }
    else{
      Serial.println("Connection failed");
      buzzerTimer(2000); // bzucak zabzuci ze se nepripojil k wifi
    }
  }
  buzzerTimer(500); // bzucak zabzuci kratkou dobu ze se spojeni povedlo
}

void httpPost(){
  // odesilani requestu
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
    for(int i - 0; i < 3; i++){
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
  pinMode(buzzer, OUTPUT); // nastaveni bzucaku na out
  pinMode(wakePin, OUTPUT);
  digitalWrite(wakePin, HIGH);
  //wifiWPS(); // zapnuti wifi
  wifiNormal();
  wifiCheck();
  digitalWrite(wakePin, LOW);
}

void loop() {
  // buttonState = digitalRead(button); // cteni stavu tlacitka
  // if(buttonState == LOW && check){
  //  wifiCheck(); // kontrola wifi zda jsme stale pripojeni
  //  check = false; // promenna pro kontrolu dlouheho mackani tlacitka
  //}
  //buttonCheck(); // zmeneni stavu promenne check
}
