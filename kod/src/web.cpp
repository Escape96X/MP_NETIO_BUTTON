#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <LittleFS.h>

// Vytvorene headery
#include "header.h"
#include "HTML.h"
#include "define.h"

const int ENPin = 2; //deklarovano take v hlavnim programu
const char *OWN_SSID = "NETIO_BUTTON";

bool APWork = true;

ESP8266WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);

// Funkce pro prevod

int dBmtoPercentage(int dBm) {
    int quality = 2 * (dBm + 100);
    if (quality > 100)
        quality = 100;
    return quality;
}

String ipToString(IPAddress ip) {
    // prevod ip adresy na string
    String s = "";
    for (int i = 0; i < 4; i++)
        s += i ? "." + String(ip[i]) : String(ip[i]);
    return s;
}

// Nastavení AP a přípojení k WiFi
void connectToWiFi() {
    String ssid = readEEPROM(SSID_POS, SSID_LEN);
    String password = readEEPROM(PASSWORD_POS, PASSWORD_LEN);
    char *s = const_cast<char *>(ssid.c_str());
    char *p = const_cast<char *>(password.c_str());
    Serial.println(s);
    WiFi.begin(s, p);
    delay(1000);
}

void APSet() {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(OWN_SSID);
    delay(2000);
}

void disconnectAP() {
    APWork = false;
    WiFi.softAPdisconnect(true);
}

// Server - staticke stranky
void handleRoot() {
    File file = LittleFS.open("/website/index.html", "r");
    server.streamFile(file, "text/html");
}

void handleWiFiConnect() {
    File file = LittleFS.open("/website/password.html", "r");
    server.streamFile(file, "text/html");
}

void handleScanWiFi() {
    server.send(200, "application/json", jsonOfNetworks());
}

void handleHTTPA() {
    server.send(200, "text/plane", planeOfHTTP(true));
}

void handleHTTPB() {
    server.send(200, "text/plane", planeOfHTTP(false));
}

void handleIPAddress() {
    server.send(200, "application/json", jsonOfIP());
}

void handleNetioProduct() {
    File file = LittleFS.open("/website/netio.html", "r");
    server.streamFile(file, "text/html");
}

void handleNetioAdd() {
    File file = LittleFS.open("/website/netio_add.html", "r");
    server.streamFile(file, "text/html");
}

void handleManual() {
    File file = LittleFS.open("/website/tutorial.html", "r");
    server.streamFile(file, "text/html");
}

// Server - dynamické stránky
void handleNetioDevice() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    if (server.hasArg("addIP")) {
        if (server.arg("group") == "true") {
            String IP_AD = server.arg("addIP");
            saveToEEPROMContent(IP_AD, IP_POSA, IP_JMP);

            saveToEEPROMContent(server.arg("http"), HTTP_POSA, HTTP_JMP);
        } else {
            saveToEEPROMContent(server.arg("addIP"), IP_POSB, IP_JMP);
            saveToEEPROMContent(server.arg("http"), HTTP_POSB, HTTP_JMP);
        }
        server.send(200, "text/html", html);
    }
}

void handleNetioDelete() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    if (server.hasArg("IPNumber")) {
        int offset = (server.arg("group") == "true") ? IP_POSA : IP_POSB;
        deleteContent(server.arg("IPNumber").toInt(), offset, IP_POSB, IP_JMP);
        offset = (server.arg("group") == "true") ? HTTP_POSA : HTTP_POSB;
        deleteContent(server.arg("IPNumber").toInt(), offset, HTTP_POSB, HTTP_JMP);
    }
    server.send(200, "text/html", html);
}


void handleWiFiPasswordRedirect() {

    if (server.hasArg("ssid")) {
        saveToEEPROM(server.arg("ssid"), SSID_POS, SSID_LEN);
        if (server.hasArg("password"))
            saveToEEPROM(server.arg("password"), PASSWORD_POS, PASSWORD_LEN);
        else
            saveToEEPROM("", PASSWORD_POS, PASSWORD_LEN);
        connectToWiFi();
        String html = "<meta http-equiv = \"refresh\" content = \"5; url = /wifi/check\" />";
        html += "<body><h1 style=\"font-family: 'Helvetica';\">CONNECTING...</h1></body>";
        server.send(200, "text/html", html);
    } else {
        server.send(200, "plain/text", "neplatne url");
    }
}

void handleWiFiApprove() {
    if (WiFi.status() == WL_CONNECTED) {
        String currentIP = "<center><h3>ESP IP: </h3><br>";
        currentIP += ipToString(WiFi.localIP());
        currentIP += "<br><button style=\"background-color: #005F41;color: white;width: 200px;padding: 15px 32px;\" onClick=\"location.href = '/'\">Return</button>";
        server.send(200, "text/html", currentIP);
    } else {
        String badPassword = "Password is not correct or device not able to connect <br><button onClick=\"location.href = '/wifi?ssid=";
        badPassword += readEEPROM(SSID_POS, SSID_LEN);
        badPassword += "';\">Return</button>";
        server.send(200, "text/html", badPassword);
    }
}

void handleConfigCheck() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
    if (server.arg("button1") != NULL) {
        //saveToEEPROM(server.arg("button1"), HTTP_POS1, HTTP_LEN);
        server.send(200, "text/html", html);
    }
    if (server.arg("button2") != NULL) {
        // saveToEEPROM(server.arg("button2"), HTTP_POS2, HTTP_LEN);
        server.send(200, "text/html", html);
    }
}


void handleTestConnection() {
    Serial.println("Testing JSON in progress");
    String html = css_head;
    html += "<h1>Table of tests</h1>";
    html += R"rawliteral(<button onClick="location.href = '/';">Return</button>)rawliteral";
    html += "<h2>S1</h2>";
    html += parsingIP(true);
    html += "<h2>S2<h2>";
    html += parsingIP(false);
    Serial.println(html);
    server.send(200, "text/html", html);
}

void handleDeepSleep() {
    feedback_timer(400, 1);
    ESPSleep();
}

void handleFactoryReset() {
    for(int i = 0; i < EEPROM.length(); i++)
        EEPROM.write(i, 255);
    EEPROM.commit();
    WiFi.disconnect();
    server.send(200, "text/html", "<meta http-equiv = \"refresh\" content = \"2; url = /\" />");
}

void serversOn() {
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/ip_adress.json", handleIPAddress);
    server.on("/http_planeA", handleHTTPA);
    server.on("/http_planeB", handleHTTPB);
    server.on("/", handleRoot);
    server.on("/wifi", handleWiFiConnect);
    server.on("/netioProduct", handleNetioProduct);
    server.on("/netioProduct/add", handleNetioAdd);
    server.on("/netioProduct/delete", handleNetioDelete);
    server.on("/netioProduct/testConnection", handleTestConnection);
    server.on("/wifi/redirect", HTTP_POST, handleWiFiPasswordRedirect);
    server.on("/wifi/check", handleWiFiApprove);
    server.on("/netioProduct/check", HTTP_GET, handleNetioDevice);
    server.on("/buttonConfigure/check", HTTP_POST, handleConfigCheck);
    server.on("/deepsleep", handleDeepSleep);
    server.on("/disconnect", handleFactoryReset);
    server.on("/manual", handleManual);
    server.begin();
}

void setWiFiServer2() {
    APSet();
    delay(500);
    serversOn();
    connectToWiFi();
}

void handleServer() {
    server.handleClient();
}