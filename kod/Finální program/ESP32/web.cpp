#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <EEPROM.h>

// Vytvorene headery
#include "web.h"
#include "HTML.h"

const char *OWN_SSID = "NETIO_BUTTON";
// Konstanty pro ulozeni do EEPROM

bool APWork = true;

WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);

// Prace s pameti

void saveToEEPROM(String sToSave, int startPosition) {
    char arr[sToSave.length() + 1];
    strcpy(arr, sToSave.c_str());
    for (int i = 0; i < sToSave.length(); i++) {
        EEPROM.write(startPosition + i, arr[i]);
    }
    EEPROM.write(startPosition + sToSave.length(), -1);
    EEPROM.commit();
    delay(500);
}

String readEEPROM(int numberOfStart, int len) {
    String tempNetio = "";
    for (int i = 0; i < len; i++) {
        char netio = EEPROM.read(numberOfStart + i);
        if (netio == 255)
            break;
        tempNetio += netio;
    }
    return tempNetio;
}

// Funkce pro prevod

int dBmtoPercentage(int dBm) {
    int quality;
    if (dBm <= -100) {
        quality = 0;
    } else if (dBm >= -20) {
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
        networks += (WiFi.encryptionType(i) == 0) ? "None" : "Encrypted";
        networks += "\" ";
        networks += (i + 1 == numberOfNetworks) ? " " : ", ";
    }
    networks += "]}";
    return networks;
}

// Nastavení AP a přípojení k WiFi
void connectToWiFi() {
   String ssid = readEEPROM(230, 64);
    String password = readEEPROM(294, 100);
    char *s = const_cast<char *>(ssid.c_str());
    char *p = const_cast<char *>(password.c_str());
    Serial.println(s);
    WiFi.begin(s, p);
}

void APSet() {
    WiFi.mode(WIFI_AP_STA);
    //WiFi.softAPConfig(OWN_IP, OWN_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(OWN_SSID);
    delay(2000);
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

void handleConfig() {
    server.send(200, "text/html", configHTML);
}

// Server - dynamické stránky
void handleNetioDevice() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    if(server.hasArg("ip1")) {
        saveToEEPROM(server.arg("ip1"), IP_POS1);
        server.send(200, "text/html", html);
    }
    if(server.hasArg("ip2")) {
        saveToEEPROM(server.arg("ip2"), IP_POS2);
        server.send(200, "text/html", html);
    }
}


void handleWiFiPasswordRedirect() {
 
    if (server.hasArg("ssid")) {
       saveToEEPROM(server.arg("ssid"),SSID_POS);
        if (server.hasArg("password"))
           saveToEEPROM(server.arg("password"),PASSWORD_POS);
        else
        saveToEEPROM("",PASSWORD_POS);
        connectToWiFi();
        String html = "<meta http-equiv = \"refresh\" content = \"5; url = /wifi/check\" />";
        html += "<body><h1>CONNECTING...</h1></body>";
        server.send(200, "text/html", html);
    } else {
        server.send(200, "plain/text", "neplatne url");
    }
}

void handleWiFiApprove() {
    if (WiFi.status() == WL_CONNECTED) {
        String currentIP = "ESP IP: ";
        currentIP += ipToString(WiFi.localIP());
        currentIP += "<button style=\"background-color: #4CAF50;color: white;width: 200px;padding: 15px 32px;\" onClick=\"location.href = '/'\">Return</button>";
        server.send(200, "text/html", currentIP);
    } else {
        String badPassword = "Nesprávné heslo nebo chyba připojení <br><button onClick=\"location.href = '/wifi?ssid=";
        badPassword += readEEPROM(SSID_POS,64);
        badPassword += "';\">Return</button>";
        server.send(200, "text/html", badPassword);
    }
}

void handleConfigCheck() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
    if(server.hasArg("button1")) {
        saveToEEPROM(server.arg("button1"), HTTP_POS1);
        server.send(200, "text/html", html);
    }
    if(server.hasArg("button2")) {
        saveToEEPROM(server.arg("button2"), HTTP_POS2);
        server.send(200, "text/html", html);
    }
}

void handleDeepSleep() {
    feedback_timer(400, 1);
    WiFi.disconnect();
    esp_deep_sleep_start();
}
void handleSettings(){
  String html = "<center><h1>Settings</h1><br>";
  html += "<h6>IP NETIO 1</h6>:";
  html += readEEPROM(IP_POS1,15);
  html += "<br>";
  html += "<h6>IP NETIO 1</h6>:";
  html += readEEPROM(IP_POS2,15);
  html += "<br>";
  html += "<h6>IP NETIO 1</h6>:";
  html += readEEPROM(HTTP_POS1,100);
  html += "<br>";
  html += "<h6>IP NETIO 1</h6>:";
  html += readEEPROM(HTTP_POS2,100);
  html += "<br>";
  html += "<h6>IP NETIO 1</h6>:";
  html += readEEPROM(SSID_POS,64);
  html += "<br></center>";
  server.send(200, "text/html", html);
}

void handleDisconnect() {
    WiFi.disconnect();
    server.send(200, "text/html", "<meta http-equiv = \"refresh\" content = \"2; url = /\" />");
}

void serversOn() {
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/", handleRoot);
    server.on("/wifi", handleWiFiConnect);
    server.on("/netioProduct", handleNetioProduct);
    server.on("/wifi/redirect", HTTP_POST, handleWiFiPasswordRedirect);
    server.on("/wifi/check", handleWiFiApprove);
    server.on("/netioProduct/check", HTTP_POST, handleNetioDevice);
    server.on("/buttonConfigure", handleConfig);
    server.on("/buttonConfigure/check", HTTP_POST, handleConfigCheck);
    server.on("/settings", handleSettings);
    server.on("/deepsleep", handleDeepSleep);
    server.on("/disconnect", handleDisconnect);
    server.begin();

}

void setWiFiServer() {
    APSet();
    delay(500);
    serversOn();
}
void setWiFiServer2() {
    connectToWiFi();
    delay(1000);
    if(WiFi.status() != WL_CONNECTED){
      WiFi.disconnect();
      feedback_timer(200, 2);
      setWiFiServer();
    }else
    serversOn();
}

void handleServer() {
   server.handleClient();
}
