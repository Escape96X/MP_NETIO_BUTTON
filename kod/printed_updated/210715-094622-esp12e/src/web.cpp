#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

// Vytvorene headery
#include "web.h"
#include "HTML.h"
const int ENPin =  2; //deklarovano take v hlavnim programu
const char *OWN_SSID = "NETIO_BUTTON";
// Konstanty pro ulozeni do EEPROM

bool APWork = true;

ESP8266WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);

// Prace s pameti

void saveToEEPROM(String sToSave, int startPosition, int len) {
    char arr[sToSave.length() + 1];
    strcpy(arr, sToSave.c_str());
    for (int i = 0; i < sToSave.length(); i++) {
        EEPROM.write(startPosition + i, arr[i]);
        if(i == len-1)
            break;
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


//NOVE
void saveToEEPROMIP(String sToSave, int startPosition){
    for(int i = 0 + startPosition; i<=170 + startPosition; i+=16){
        char isItIp = EEPROM.read(i);
        if(isItIp == 255){
            saveToEEPROM(sToSave, i, 16);
        }
    }
}


// Funkce pro prevod

int dBmtoPercentage(int dBm) {
    int quality = 2 * (dBm + 100);
    if (quality > 100)
        quality = 100;
    return quality;
}

// prevod ip adresy na string
String ipToString(IPAddress ip) {
    String s = "";
    for (int i = 0; i < 4; i++)
        s += i ? "." + String(ip[i]) : String(ip[i]);
    return s;
}

int countIP(int offset){
    int numberOfIP = 0;
    for(int i = offset; i < 172 + offset; i += 16){
        char character = EEPROM.read(i);
        if(character == 255)
            numberOfIP++;
    }
    return numberOfIP;
}

String readIP(int i, int offset) {
    int position = (i * (IP_LEN + 1)) + offset;
    if(EEPROM.read(position) == 35)
        readEEPROM(position + 1, IP_LEN);
    else {
        for(position, position < IP_POSB + offset; position +=16;) {
            char character = EEPROM.read(position);
            if(character == 35)
                readEEPROM(position +1, IP_LEN);
        }
    }
}

// Vytvoření JSON
String jsonOfIP(bool group){
    int offset = (group) ? IP_POSA : IP_POSB;
    int numberOfIP = countIP(offset);
    String IPs = "{\"numOfIP\": \"";
    IPs += numberOfIP;
    for (int i = 0; i < numberOfIP; i++) {
        IPs += "\"";
        IPs += readIP(i, offset);

    }
}

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
    if (server.hasArg("addIP")) {
        if(server.arg("group") == 0)
            saveToEEPROMIP(server.arg("addIP"), IP_POSA);
        else
            saveToEEPROMIP(server.arg("addIP"),IP_POSB);
        //saveToEEPROM(server.arg("ip1"), IP_POSA, IP_LEN);
        //server.send(200, "text/html", html);
    }
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
        String currentIP = "ESP IP: ";
        currentIP += ipToString(WiFi.localIP());
        currentIP += "<button style=\"background-color: #4CAF50;color: white;width: 200px;padding: 15px 32px;\" onClick=\"location.href = '/'\">Return</button>";
        server.send(200, "text/html", currentIP);
    } else {
        String badPassword = "Nesprávné heslo nebo chyba připojení <br><button onClick=\"location.href = '/wifi?ssid=";
        badPassword += readEEPROM(SSID_POS, SSID_LEN);
        badPassword += "';\">Return</button>";
        server.send(200, "text/html", badPassword);
    }
}

void handleConfigCheck() {
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
    if (server.hasArg("button1")) {
        saveToEEPROM(server.arg("button1"), HTTP_POS1, HTTP_LEN);
        server.send(200, "text/html", html);
    }
    if (server.hasArg("button2")) {
        saveToEEPROM(server.arg("button2"), HTTP_POS2, HTTP_LEN);
        server.send(200, "text/html", html);
    }
}

void handleDeepSleep() {
    feedback_timer(400, 1);
    //digitalWrite(ENPin, LOW);
    ESP.deepSleep(0);
}

void handleSettings() {
    String html = "<head><style>body{font-family: \"Helvetica\";}</style></head><center><h1>Settings</h1><br>";
    html += "<h4>IP address #1</h4>";
    html += readEEPROM(IP_POSA, IP_LEN);
    html += "<br>";
    html += "<h4>IP address #2</h4>";
    html += readEEPROM(IP_POSB, IP_LEN);
    html += "<br>";
    html += "<h4>JSON #1</h4>";
    html += readEEPROM(HTTP_POS1, IP_LEN);
    html += "<br>";
    html += "<h4>JSON #2</h4>";
    html += readEEPROM(HTTP_POS2, IP_LEN);
    html += "<br>";
    html += "<h4>SSID</h4>";
    html += readEEPROM(SSID_POS, SSID_LEN);
    html += "<br></center>";
    server.send(200, "text/html", html);
}

void handleDisconnect() {
    EEPROM.write(SSID_POS, -1);
    EEPROM.write(PASSWORD_POS, -1);
    EEPROM.commit();
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
    if (WiFi.status() != WL_CONNECTED) {
        feedback_timer(200, 2);
        setWiFiServer();
    } else
        serversOn();
}

void handleServer() {
    server.handleClient();
}
