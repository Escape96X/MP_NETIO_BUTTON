#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* HTTP_CONNECTION = "http://192.168.0.106/netio.json";
const char* HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";

const int buzzer = 13;
const int LED = 12;
const int wakePin = 5;

IPAddress local_IP(192, 168, 0, 107);
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);

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
  WiFi.config(local_IP, subnet, gateway);
  WiFi.begin(ssid, password);
  Serial.println("POKUS O PRIP");
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
    Serial.println("nepripojeno");
     for(int i=0; i<2;i++){
          buzzerTimer(200); // ESP se nepripojilo k zasuvce
        }
    }
}


void setup() {
  Serial.begin(115200);
  pinMode(wakePin, OUTPUT);
  digitalWrite(wakePin, HIGH);
  pinMode(buzzer, OUTPUT);
  wifiNormal();
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  httpPost();
  Serial.println("TEST");
  digitalWrite(5, LOW);
}

void loop() {
}
