#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiScan.h>
#include <EEPROM.h>

// Vytvorene headery
#include "web.h"
#include "HTML.h"


String ssid = "";
String password = "";
String NetioIP = "0";

const char *OWN_SSID = "NETIO_BUTTON";
const int RSSI_MAX = -40;
const int RSSI_MIN = -100;
bool APWork = true;

ESP8266WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);


// Fuknce pro výpočet

void saveIPtoEEPROM(){
  char arr[NetioIP.length() +1];
  strcpy(arr, NetioIP.c_str());
  Serial.print("v charu ");
  Serial.println(arr);
  for(int i = 0; i < NetioIP.length(); i++){
    EEPROM.write(0 + i, arr[i]);
  }
  EEPROM.commit();
}

String writeSAVED(){
  String tempNetio ="";
  int len = 15;
  for(int i = 0; i <len; i++){
    char netio = EEPROM.read(i);
    if(netio == ';')
      break;
    tempNetio += netio;
    Serial.print(netio);
      
  }
  Serial.println(" ");
  return tempNetio;
}
int dBmtoPercentage(int dBm) {
    int quality;
    if (dBm <= RSSI_MIN) {
        quality = 0;
    } else if (dBm >= RSSI_MAX) {
        quality = 100;
    } else {
        quality = 2 * (dBm + 100);
    }
    return quality;
}

String ipToString(IPAddress ip) {
    String s = "";
    for (int i = 0; i < 4; i++)
        s += i ? "." + String(ip[i]) : String(ip[i]);
    return s;
}

// Vytvoření JSON
String jsonOfNetworks() {
    int numberOfNetworks = WiFi.scanNetworks();
    String networks = "{\"numOfNetworks\": \"";
    networks += numberOfNetworks;
    networks += "\", \"networks\": [";
    for (int i = 0; i < numberOfNetworks; i++) {
        networks += "\"";
        networks += WiFi.SSID(i);
        networks += "\"";
        networks += (i + 1 == numberOfNetworks) ? " " : ", ";
    }
    networks += "], \"strengh\": [";
    for (int i = 0; i < numberOfNetworks; i++) {
        networks += "\"";
        networks += dBmtoPercentage(WiFi.RSSI(i));
        networks += " %\" ";
        networks += (i + 1 == numberOfNetworks) ? " " : ", ";
    }
    networks += "], \"protection\": [";
    for (int i = 0; i < numberOfNetworks; i++) {
        networks += "\"";
        networks += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "None" : "Encrypted";
        networks += "\" ";
        networks += (i + 1 == numberOfNetworks) ? " " : ", ";
    }
    networks += "]}";
    return networks;
}

// Nastavení AP a přípojení k WiFi
void connectToWiFi() {
    WiFi.begin(ssid, password);
    for (int i = 0; i < 5; i++) {
        if (WiFi.status() == WL_CONNECTED)
            break;
        delay(1000);
    }
}

void APSet() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(OWN_IP, OWN_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(OWN_SSID);
}

void disconnectAP() {
    APWork = false;
    WiFi.softAPdisconnect(true);
}

// Server - staticke stranky
void handleRoot() {
    server.send(200, "text/html", indexHTML);
}

void handleWiFiConnect() {
    server.send(200, "text/html", passwordHTML);
}

void handleScanWiFi() {
    server.send(200, "application/json", jsonOfNetworks());
}

void handleNetioProduct() {
  server.send(200, "text/html", NetioHTML);
}

// Server - dynamické stránky
void handleNetioDevice() {
    if (server.hasArg("ip")) {
        NetioIP = server.arg("ip");
        NetioIP += ";";
        saveIPtoEEPROM();
        writeSAVED();
        disconnectAP();
    } else {
        server.send(403);
    }
}

void handleCheckWiFiPassword() {
    if (server.hasArg("ssid")) {
        ssid = server.arg("ssid");
        if (server.hasArg("password"))
            password = server.arg("password");
        else
            password = "";
        connectToWiFi();
        Serial.println("wifi");
        if (WiFi.status() == WL_CONNECTED) {
            String currentIP = "ESP IP: ";
            currentIP += ipToString(WiFi.localIP());
            currentIP += "<button onClick=\"location.href = '/'\"></button>";
            server.send(200, "text/html", currentIP);
        } else {
            String badPassword = "Nesprávné heslo nebo chyba připojení <br><button onClick=\"location.href = '/wifi?wifi=";
            badPassword += ssid;
            badPassword += "';\">Return</button>";
            server.send(200, "text/html", badPassword);
        }
    } else {
        server.send(200, "plain/text", "neplatne url");
    }
}

void serversOn(){
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/", handleRoot);
    server.on("/wifi", handleWiFiConnect);
    server.on("/netioProduct", handleNetioProduct);
    server.on("/wifi/check", HTTP_POST, handleCheckWiFiPassword);
    server.on("/netioProduct/check", HTTP_POST, handleNetioDevice);
    server.begin();
    Serial.println("AP SERVER");
}

void setWiFiServer() {
    //WiFi.disconnect();
    APSet();
    delay(500);
    serversOn();
}

void handleServer() {
    //if(APWork)
      server.handleClient();
}
