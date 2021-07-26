#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

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
    // Serial.println(s);
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
    server.send(200, "text/html", indexHTML);
}

void handleWiFiConnect() {
    server.send(200, "text/html", passwordHTML);
}

void handleScanWiFi() {
    server.send(200, "application/json", jsonOfNetworks());
}

void handleIPAddress() {
    server.send(200, "application/json", jsonOfIP());
}

void handleNetioProduct() {
    server.send(200, "text/html", NetioHTML);
}

void handleNetioAdd() {
    server.send(200, "text/html", NetioAddHTML);
}

void handleManual() {
    server.send(200, "text/html", tutorialHTML);
}

void handleConfig() {
    String HTML = configHTML;
    HTML += "<br> <h3>Button #1:</h3><br>";
    HTML += readEEPROM(HTTP_POS1, HTTP_LEN);
    HTML += "<br> <h3>Button #2:</h3><br>";
    HTML += readEEPROM(HTTP_POS2, HTTP_LEN);
    HTML += "</center></body></html>";

    server.send(200, "text/html", HTML);
}

// Server - dynamické stránky
void handleNetioDevice() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    if (server.hasArg("addIP")) {
        if (server.arg("group") == "true")
            saveToEEPROMIP(server.arg("addIP"), IP_POSA);
        else
            saveToEEPROMIP(server.arg("addIP"), IP_POSB);
        //saveToEEPROM(server.arg("ip1"), IP_POSA, IP_LEN);
        server.send(200, "text/html", html);
    }
}

void handleNetioDelete() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    if (server.hasArg("IPNumber")) {
        int offset = (server.arg("group") == "true") ? IP_POSA : IP_POSB;
        deleteIP(server.arg("IPNumber").toInt(), offset);
    }
    server.send(200, "text/html", html);
}


void handleWiFiPasswordRedirect() {

    if (server.hasArg("ssid")) {
        saveToEEPROM(server.arg("ssid"), SSID_POS, HTTP_LEN);
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
    if (server.arg("button1")!= NULL) {
        saveToEEPROM(server.arg("button1"), HTTP_POS1, HTTP_LEN);
        server.send(200, "text/html", html);
    }
    if (server.arg("button2")!= NULL) {
        saveToEEPROM(server.arg("button2"), HTTP_POS2, HTTP_LEN);
        server.send(200, "text/html", html);
    }
}

void handleDeepSleep() {
    feedback_timer(400, 1);
    ESPSleep();
}

void handleDisconnect() {
    EEPROM.write(SSID_POS, -1);
    EEPROM.write(PASSWORD_POS, -1);
    EEPROM.commit();
    WiFi.disconnect();

    server.send(200, "text/html", "<meta http-equiv = \"refresh\" content = \"2; url = /\" />");
}

// void handledebug() {
//     String pes = "test: ";
//     for (int i = 0; i < 400; i++) {
//         if (i == 188)
//             pes += "---";
//         else if (EEPROM.read(i) == 255)
//             pes += "+";
//         else
//             pes += EEPROM.read(i);
//         pes += ";";
//     }
//     server.send(200, "text/html", pes);
// }

void serversOn() {
    server.on("/scannedWiFi.json", handleScanWiFi);
    server.on("/ip_adress.json", handleIPAddress);
    server.on("/", handleRoot);
    server.on("/wifi", handleWiFiConnect);
    server.on("/netioProduct", handleNetioProduct);
    server.on("/netioProduct/add", handleNetioAdd);
    server.on("/netioProduct/delete", handleNetioDelete);
    server.on("/wifi/redirect", HTTP_POST, handleWiFiPasswordRedirect);
    server.on("/wifi/check", handleWiFiApprove);
    server.on("/netioProduct/check", HTTP_GET, handleNetioDevice);
    server.on("/buttonConfigure", handleConfig);
    server.on("/buttonConfigure/check", HTTP_POST, handleConfigCheck);
    server.on("/deepsleep", handleDeepSleep);
    server.on("/disconnect", handleDisconnect);
    server.on("/manual", handleManual);
    // server.on("/debug", handledebug);
    server.begin();
}

void setWiFiServer2() {
    APSet();
    delay(500);
    serversOn();
}

void handleServer() {
    server.handleClient();
}
