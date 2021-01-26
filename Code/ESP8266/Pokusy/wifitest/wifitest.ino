/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"

IPAddress local_IP(192, 168, 0, 166);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.config(local_IP, gateway, subnet);
  //delay(500);
  if(WiFi.status() == WL_CONNECTED)
    ESP.deepSleep(5e6);
  WiFi.begin("Jirickovi_secured");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.RSSI());

  ESP.deepSleep(5e6);
}

void loop() {

}
