#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <EEPROM.h>

// Vytvorene headery
#include "web.h"
#include "HTML.h"

const char *OWN_SSID = "NETIO_BUTTON";
// Konstanty pro ulozeni do EEPROM
#define IP_POS1 0;
#define IP_POS2 15;
#define HTTP_POS1 30;
#define HTTP_POS2 130;
#define SSID_POS 230;
#define PASSWORD_POS 294;



const int RSSI_MAX = -40;
const int RSSI_MIN = -100;
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
        networks += (WiFi.encryptionType(i) == 0) ? "None" : "Encrypted";
        networks += "\" ";
        networks += (i + 1 == numberOfNetworks) ? " " : ", ";
    }
    networks += "]}";
    return networks;
}

// Nastavení AP a přípojení k WiFi
void connectToWiFi() {

    char *s = const_cast<char *>(readEEPROM(SSID_POS, 64).c_str());
    char *p = const_cast<char *>(readEEPROM(PASSWORD_POS, 100).c_str());
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
void handleNetioDevice1() {
    if (server.hasArg("ip")) {
        NetioIP1 = server.arg("ip");
        saveToEEPROM(NetioIP1, IP_POS1);
        String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
        server.send(200, "text/html", html);
    } else {
        server.send(403);
    }
}

void handleNetioDevice2() {
    if (server.hasArg("ip")) {
        saveToEEPROM(server.arg("ip"), 15);
        String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
        server.send(200, "text/html", html);
    } else {
        server.send(403);
    }
}

void handleWiFiPasswordRedirect() {
    if (server.hasArg("ssid")) {
       saveToEEPROM(server.arg("ssid"),SSID_PO);
        if (server.hasArg("password")) {
           saveToEEPROM(server.arg("password"),PASSWORD_POS);
        } else
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
        String badPassword = "Nesprávné heslo nebo chyba připojení <br><button onClick=\"location.href = '/wifi?wifi=";
        badPassword += ssid;
        badPassword += "';\">Return</button>";
        server.send(200, "text/html", badPassword);
    }
}

void handleConfigCheck1() {
    if (server.hasArg("button1"))
        HTTPRequest1 = server.arg("button1");

    saveToEEPROM(HTTPRequest1, 30);
    HTTPRequest1 = readEEPROM(30, 100);

    //else if(server.hasArg("button2")
    //doplnit;
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
    server.send(200, "text/html", html);
}

void handleConfigCheck2() {
    if (server.hasArg("button2"))
        HTTPRequest2 = server.arg("button2");

    saveToEEPROM(HTTPRequest2, 130);
    HTTPRequest2 = readEEPROM(130, 100);

    //else if(server.hasArg("button2")
    //doplnit;
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
    server.send(200, "text/html", html);
}

void handleDeepSleep() {
    feedback_timer(400, 1);
    esp_deep_sleep_start();
}

void serversOn() {
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/", handleRoot);
    server.on("/wifi", handleWiFiConnect);
    server.on("/netioProduct", handleNetioProduct);
    server.on("/wifi/redirect", HTTP_POST, handleWiFiPasswordRedirect);
    server.on("/wifi/check", handleWiFiApprove);
    server.on("/netioProduct/check1", HTTP_POST, handleNetioDevice1);
    server.on("/netioProduct/check2", HTTP_POST, handleNetioDevice2);
    server.on("/buttonConfigure", handleConfig);
    server.on("/buttonConfigure/check1", HTTP_POST, handleConfigCheck1);
    server.on("/buttonConfigure/check2", HTTP_POST, handleConfigCheck2);
    server.on("/deepsleep", handleDeepSleep);
    server.begin();
    Serial.println("AP has been turned on");
}

void setWiFiServer() {
    APSet();
    delay(500);
    serversOn();
}

void handleServer() {
   server.handleClient();
}
