#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>

// vlastni headers
#include "web.h"
#include "define.h"
#include "memory.h"
#include "basic_functions.h"

// globalni promnene
bool BUTTONSTATE1 = false;
bool BUTTONSTATE2 = false;
bool errors = false;


WiFiClient wificlient;
HTTPClient http;


bool pin_pressed() {
    // kontrola jake tlacitko bylo zmacknuto
    if (BUTTONSTATE1 == false) {
        return true;
    } else {
        return false;
    }
}

String http_post(String HTTP_CONNECTION, int position, bool button_pressed) {
    // posle http request zasuvce
    int offset = (button_pressed) ? HTTP_POSA : HTTP_POSB;
    if (http.begin(wificlient, HTTP_CONNECTION)) {
        Serial.println("zacal jsem http");
        int httpCode = http.POST(readContent(position, offset, HTTP_POSB, HTTP_JMP));
        if(httpCode > 0) {
            Serial.print("HTTP CODE: ");
            Serial.println(httpCode);
            Serial.print("pozice HTTP:");
            Serial.println(position);
            String payload = http.getString();
            Serial.println("payload");
            Serial.println(payload);

            if (payload.indexOf("Errors") > 0 || payload.length() == 0) {
                Serial.print("jsem v erroru");

                errors = true;
            }
            Serial.println(payload);
            http.end();
            return payload;
        } else {
            String payload = "HTTP failed with code: ";
            payload += httpCode;
            return payload;
        }
    }
    Serial.println("Kod je konec");
    return "Protocol failed";
}

String parsingIP(bool button_pressed) {
    // najde prislusnou ip adresu
    int offset = (button_pressed) ? IP_POSA : IP_POSB;
    int count = countContent(offset, IP_POSB, IP_JMP);
    // Serial.println(count);
    if (count == 0) {
        feedback_timer(200, 3);
        return "Table of actions is empty.";
    } else {
        String end_payload = "<table>";
        for (int i = 0; i < count; i++) {
            String HTTP_CONNECTION = "http://";
            String ip_address = readContent(i, offset, IP_POSB, IP_JMP);
            end_payload += "<th><td>"+ip_address + "</td><td>";

            HTTP_CONNECTION += ip_address;
            HTTP_CONNECTION += "/netio.json";
            end_payload += http_post(HTTP_CONNECTION, i, button_pressed);
            end_payload += "</td></th>";
            Serial.println(i);
            delay(200);
        }
        end_payload += "</table>";
        Serial.print("error:");
        Serial.println(errors);
        if (errors)
            feedback_timer(200, 3);
        return end_payload;
    }
}

void wifi_setup() {
    // precte z pameti ssid a password a pripoji se k wifi
    String ssid = readEEPROM(SSID_POS, SSID_LEN);
    String password = readEEPROM(PASSWORD_POS, PASSWORD_LEN);
    Serial.print("SSID and Password:");
    Serial.println(ssid);
    Serial.println(password);
    char *s = const_cast<char *>(ssid.c_str()); // prevede string na char*
    char *p = const_cast<char *>(password.c_str());
    delay(300);
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(s, p);
        for (int i = 0; i < 50; i++) { // kontroluje zda bylo pripojeno k wifi, po 5s se vypne esp
            if (WiFi.status() == WL_CONNECTED)
                break;
            delay(100);
            Serial.print(".");
        }
        Serial.println(' ');
        if (WiFi.status() != WL_CONNECTED) {// pokud neni pripojen ohlasi chybu
            feedback_timer(200, 2);
            Serial.print("WiFi status: ");
            Serial.println(WiFi.status());
            WiFi.disconnect();
            ESPSleep();
        }
    }
}

bool check_conf_mode() {
    // overi zda jsou oba piny zmackle pro konfiguracni mod
    if (!BUTTONSTATE1 && !BUTTONSTATE2) {
        feedback_timer(400, 1);
        Serial.println("Config mode enabled");
        setWiFiServer();
        return true;
    } else if (BUTTONSTATE1 && BUTTONSTATE2) {
        feedback_timer(200, 4);
        ESPSleep();
        return false;
    } else
        return false;
}


void send_message() {
    wifi_setup();
    parsingIP(pin_pressed());
}

void setup_boot() {
    // nastaveni po startu
    pinMode(ENPin, OUTPUT);
    digitalWrite(ENPin, HIGH);
    pinMode(WAKEUP_PIN1, INPUT_PULLUP);
    pinMode(WAKEUP_PIN2, INPUT_PULLUP);
    BUTTONSTATE1 = digitalRead(WAKEUP_PIN1);
    BUTTONSTATE2 = digitalRead(WAKEUP_PIN2);
    Serial.begin(115200);
    EEPROM.begin(4096);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Načtení pinu");
}

void debug() {
    //debug zpravy z pameti
    Serial.println("WiFi login");
    Serial.println(readEEPROM(SSID_POS, SSID_LEN));
    Serial.println(readEEPROM(PASSWORD_POS, 64));
    Serial.println("Button state detection:");
    Serial.print("Button S1:");
    Serial.println(BUTTONSTATE1);
    Serial.print("Button S2:");
    Serial.println(BUTTONSTATE2);
    Serial.println("");
}

void setup() {
    setup_boot();
    debug();
      if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
    }
    if (!check_conf_mode()) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Standard mode enabled");
        send_message();
        ESPSleep();

    }
}

void loop() {
    delay(1);
    handleServer();
    if (!digitalRead(WAKEUP_PIN1) && !digitalRead(WAKEUP_PIN2)) {
        feedback_timer(400, 1);
        ESPSleep();
    }

}