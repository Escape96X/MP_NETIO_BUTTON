#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiScan.h>
#include "web.h"
#include "HTML.h"

const char *OWN_SSID = "NETIO_BUTTON";

String ssid = "";
String password = "";

ESP8266WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);


// Vytvoření JSON
String scanNetwork(){
  int numberOfNetworks = WiFi.scanNetworks();
  String networks = "{\"numOfNetworks\": \"";
  networks += numberOfNetworks;
  networks += "\", \"networks\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += WiFi.SSID(i);
    networks +="\"";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks +="], \"strengh\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += WiFi.RSSI(i);
    networks +="\" ";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks += "], \"protection\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += (WiFi.encryptionType(i) == ENC_TYPE_NONE)? "None": "Encrypted";
    networks +="\" ";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks += "]}";
  return networks;
}


void connectToWiFi(){
    Serial.println(ssid);
    Serial.println(password);
    WiFi.begin(ssid, password);
    delay(2000);
    Serial.println(WiFi.status());
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.localIP());
}


void APSet(){
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(OWN_IP, OWN_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(OWN_SSID);
}

void test(){
  server.send(200, "text/html", indexHTML);
}

void handleWiFiConnect(){
    server.send(200, "text/html", passwordHTML);
}

void handleScanWiFi(){
    server.send(200, "application/json", scanNetwork());
}

void handleSetPasword(){
    Serial.println("jsem v handle");
    if(server.hasArg("ssid")){
        ssid = server.arg("ssid");
        Serial.println("mam heslo");
        password = server.arg("password");
        connectToWiFi();
    }
    server.send(200, "text/plain", (WiFi.status() == WL_CONNECTED)? "Připojeno":"Nepřipojeno");
}

void setWiFiServer(){
    APSet();
    delay(500);
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/", test);
    server.on("/wifi", handleWiFiConnect);
    server.on("/wifi/aprove", HTTP_POST, handleSetPasword);
    server.begin();
    Serial.println(ssid);
    Serial.println(password);
}


void handleServer(){
    server.handleClient();
}