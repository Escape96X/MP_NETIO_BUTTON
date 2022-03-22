#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>

#include "define.h"



void feedback_timer(int duration, int quantity) {
    // feedback platformy
    for (int i = 0; i < quantity; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(duration);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
    }
}


void ESPSleep() {
    Serial.println("sleepuji");
    delay(100);
    digitalWrite(ENPin, LOW);
}

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