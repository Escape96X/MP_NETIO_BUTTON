#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include "basic_functions.h"
#include "define.h"
#include "memory.h"


String jsonOfIP() {
    // json pro IP adresy
    int offset = IP_POSA;
    int numberOfIP = countContent(offset, IP_POSB, IP_JMP);
    String IPs = "{\"numOfIPA\": \"";
    IPs += numberOfIP;
    IPs += "\", \"IP_adressA\": [";
    for (int i = 0; i < numberOfIP; i++) {
        IPs += "\"";
        IPs += readContent(i, offset, IP_POSB, IP_JMP);
        IPs += "\"";
        IPs += (i + 1 == numberOfIP) ? "" : ", ";
    }
    offset = IP_POSB;
    numberOfIP = countContent(offset, IP_POSB, IP_JMP);
    IPs += "], \"numOfIPB\": \"";
    IPs += numberOfIP;
    IPs += "\", \"IP_addressB\": [";
    for (int i = 0; i < numberOfIP; i++) {
        IPs += "\"";
        IPs += readContent(i, offset, IP_POSB, IP_JMP);
        IPs += "\"";
        IPs += (i + 1 == numberOfIP) ? "" : ", ";
    }
    IPs += "]}";
    //Serial.println(IPs);
    return IPs;
}

String planeOfHTTP(bool group) {
    // json pro HTTP adresy
    int offset = (group) ? HTTP_POSA : HTTP_POSB;
    int numberOfHTTP = countContent(offset, HTTP_POSB, HTTP_JMP);
    String HTTPs = "";
    for (int i = 0; i < numberOfHTTP; i++) {
        HTTPs += readContent(i, offset, HTTP_POSB, HTTP_JMP);
        HTTPs += (i + 1 == numberOfHTTP) ? "" : "!";
    }
    return HTTPs;
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
    if (WiFi.status() == WL_CONNECTED)
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


File open_file(String file_name, char* action) {
    Serial.println("Oteviram soubor");
    File file = LittleFS.open(file_name, action);
    if(!file) {
        Serial.println("File cannot be opened");
        feedback_timer(300, 5);
        //ESPSleep();
    }
    return file;
}


String json_read(String table_name, int index) {
    File file = LittleFS.open(table_name, "r");
    Serial.println("soubor otevren");
    StaticJsonDocument <512> docu;
    Serial.println("doc vytvoren");
    deserializeJson(docu, file);
    Serial.println("deserializovano");
    JsonObject object = docu[index];
    String result = object["ip"];
    file.close();
    docu.garbageCollect();
    //Serial.println(data[0]);
    return result;

}

size_t json_count(String table_name) {
    File table_file = open_file(table_name, "r");
    DynamicJsonDocument doc(8192);
    deserializeJson(doc, table_file);
    return doc.size();
}

void json_upload(String table_name, String ip_address, String json, String username, String password) {
    Serial.println("Uploading Json");
    File table_file = LittleFS.open(table_name, "r");
    
    DynamicJsonDocument doc(8192);
    deserializeJson(doc, table_file);
    JsonObject object = doc.createNestedObject();
    object["ip"] = ip_address;
    object["json"] = json;
    object["username"] = username;
    object["password"] = password;
    table_file.close();
    table_file = LittleFS.open(table_name, "w");
    Serial.printf(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, table_file);
    Serial.println(doc.size());
    table_file.close();

    
}


void json_delete(String table_name, int position){
    Serial.println("Deleting Json");
    File table_file = LittleFS.open(table_name, "r");
    DynamicJsonDocument doc(8192);
    deserializeJson(doc, table_file);
    doc.remove(position);
    table_file.close();
    table_file = LittleFS.open(table_name, "w");
    Serial.printf(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, table_file);
    Serial.println(doc.size());
    table_file.close();

}

// zapracovat na erroru
String read_json_string(String element, String file_name) {
    Serial.println("Reading json");
    File file = LittleFS.open(file_name, "r");
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, file);
    if (error)
        Serial.println("DEserialization failed");
    file.close();
    return doc[element];
}

void write_json_string(String element, String value, String file_name) {
    Serial.println("Write json");
    File file = LittleFS.open(file_name, "r");
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, file);
    if(error) 
        Serial.println("Deserialization failed");
    doc[element] = value;
    file.close();
    file = LittleFS.open(file_name, "w");
    serializeJson(doc, file);
    file.close();
}