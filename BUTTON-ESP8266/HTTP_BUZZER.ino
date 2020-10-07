#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


// konstanty pripojeni a json zpravy
const char* HTTP_CONNECTION = "http://192.168.0.111/netio.json";
const char* HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";
const int button = 0;
const int buzzer = 4;
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
  const char* ssid = "#####";
  const char* password = "#####";
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
  }
}

void buttonCheck(){
  // aby pri delsim podrzenim stav nepreskakoval
  buttonState = digitalRead(button);
  if(buttonState == HIGH){
    check = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT); // nastaveni bzucaku na out
  pinMode(button, INPUT); // nastaveni flash tlacitka na in
  wifiWPS(); // zapnuti wifi
  //wifiNormal();
}

void loop() {
  buttonState = digitalRead(button); // cteni stavu tlacitka
  if(buttonState == LOW && check){
    wifiCheck(); // kontrola wifi zda jsme stale pripojeni
    check = false; // promenna pro kontrolu dlouheho mackani tlacitka
  }
  buttonCheck(); // zmeneni stavu promenne check
}
