/*
    This sketch demonstrates how to scan WiFi networks.
    The API is almost the same as with the WiFi Shield library,
    the most obvious difference being the different file you need to include:
*/
#include "ESP8266WiFi.h"

//IPAddress local_IP(192, 168, 0, 166);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  Serial.println(WiFi.localIP());
  //WiFi.config(local_IP, gateway, subnet);
  delay(100);
  WiFi.begin("Jirickovi_secured");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  //Serial.println(WiFi.localIP());

  ESP.deepSleep(5e6);
}

void loop() {

}
