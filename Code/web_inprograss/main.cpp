#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiScan.h>
#include <web.h>


void setup(){
  Serial.begin(9600);
  setWiFiServer();
}

void loop() {
  handleServer();
}