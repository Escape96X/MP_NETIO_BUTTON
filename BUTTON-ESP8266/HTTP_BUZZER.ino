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
  const char* ssid = "#####";
  const char* password = "#####";
  WiFi.begin(ssid, password);
}

void wifiWPS(){
  // Nastaveni WiFi a pripojeni WPS
  WiFi.mode(WIFI_STA);
  delay(500);
  WiFi.begin("","");
  delay(2000);
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting...");
    WiFi.beginWPSConfig();
    delay(3000);
    if(WiFi.status() == WL_CONNECTED){
      buzzerTimer(500);
      Serial.println("Connected");
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.SSID());
    }
    else{
      Serial.println("Connection failed");
      buzzerTimer(2000);
    }
  }
  buzzerTimer(500);
}

void httpPost(){
  // odesilani requestu
  if(http.begin(HTTP_CONNECTION)){
      http.addHeader("Content-Type", "text/plain");
      int httpCode = http.POST("{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}");
      String payload = http.getString();
      
      if(payload =="\"errors\":[{\"code\":400,\"message\":\"Bad request\"}"){
        for(int i=0; i<2;i++){
          buzzerTimer(200);
        }
      }
      Serial.println(payload);
      http.end(); 
  } else{
     for(int i=0; i<2;i++){
          buzzerTimer(200);
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
  pinMode(buzzer, OUTPUT);
  pinMode(button, INPUT);
  wifiWPS();
  //wifiNormal();
}

void loop() {
  buttonState = digitalRead(button);
  if(buttonState == LOW && check){
    wifiCheck();
    check = false;
  }
  buttonCheck();
}
