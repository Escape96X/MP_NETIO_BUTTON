/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>

//IPAddress local_IP(192, 168, 0, 166);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);

String HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";
String HTTP_CONNECTION = "http://192.168.0.196/netio.json";
HTTPClient http;

void WiFib(){
  Serial.println('\n');
  if(WiFi.status() != WL_CONNECTED){
   Serial.println("Connected");
  }else{
  WiFi.begin("Jirickovi_secured", "19192020");
  Serial.print("Connecting to ");
//  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(++i); Serial.print(' ');
  }
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
}}

void httpPost()
{
  if (http.begin(HTTP_CONNECTION))
  {
    int httpCode = http.POST(HTTP_REQUEST); // odeslani json requestu
    String payload = http.getString();      // zjisteni zpetne vazby
    //Serial.println(payload);
    http.end();
  }
}




void setup() {
  Serial.begin(115200);
  delay(850);
  WiFib();
  httpPost();
  delay(2000);
  ESP.deepSleep(0);
}
void loop() {
  //httpPost();
  //delay(5000);
}
