#include <Arduino.h>

#include "header.h"
#include "define.h"

String jsonOfIP() {
    // json pro IP adresy
    int offset = IP_POSA;
    int numberOfIP = countIP(offset);
    String IPs = "{\"numOfIPA\": \"";
    IPs += numberOfIP;
    IPs += "\", \"IP_adressA\": [";
    for (int i = 0; i < numberOfIP; i++) {
        IPs += "\"";
        IPs += readIP(i, offset);
        IPs += "\"";
        IPs += (i + 1 == numberOfIP) ? "" : ", ";
    }
    offset = IP_POSB;
    numberOfIP = countIP(offset);
    IPs += "], \"numOfIPB\": \"";
    IPs += numberOfIP;
    IPs += "\", \"IP_addressB\": [";
    for (int i = 0; i < numberOfIP; i++) {
        IPs += "\"";
        IPs += readIP(i, offset);
        IPs += "\"";
        IPs += (i + 1 == numberOfIP) ? "" : ", ";
    }
    IPs += "]}";
    Serial.println(IPs);
    return IPs;
}

String jsonOfNetworks() {
    // json pro site
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
    networks += "], \"ssid\": \"";
    if(WiFi.status() == WL_CONNECTED)
        networks += readEEPROM(SSID_POS, SSID_LEN);
    else
        networks += "DISCONNECTED";
    networks += "\", \"ip\": \"";
    if (WiFi.status() == WL_CONNECTED)
        networks += ipToString(WiFi.localIP());
    else
        networks += "EMPTY";
    networks += "\"}";
    return networks;
}
