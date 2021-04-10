#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "web.h"
// piny

const int WAKEUP_PIN1 = 12;
const int WAKEUP_PIN2 = 14;
const int BUZZER_PIN = 5;
const int LED_PIN = 13;
bool BUTTONSTATE1 = false;
bool BUTTONSTATE2 = false;

HTTPClient http;

// feedback platformy
void feedback_timer(int duration, int quantity) {
    // Bzucak na oznameni stavu
    for (int i = 0; i < quantity; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
        delay(duration);
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        delay(100);
    }
}

bool pin_pressed() {
    // kontrola jake tlacitko bylo zmacknuto
    if (BUTTONSTATE1 == false) {
        return true;
    } else {
        return false;
    }
}

void http_post() {
    String HTTP_CONNECTION = "http://";
    HTTP_CONNECTION += (pin_pressed()) ? readEEPROM(0, 15) : readEEPROM(15, 15);// nacte z pameti prislusnou ip zasuvky
    HTTP_CONNECTION += "/netio.json";
    if (http.begin(HTTP_CONNECTION)) {
        int httpCode = http.POST(
                (pin_pressed()) ? readEEPROM(30, 100) : readEEPROM(130, 100));// nacte z pameti prislusnou JSON zpravu
        String payload = http.getString();

        if (payload.indexOf("errors") > 0 || payload == "") {
            feedback_timer(200, 3);
        }
        Serial.println(payload);
        http.end();
    } else {
        feedback_timer(200, 2);
    }

}

void wifi_setup() {
    // precte z pameti ssid a password a pripoji se k wifi
    String ssid = readEEPROM(230, 64);
    String password = readEEPROM(294, 100);
    char *s = const_cast<char *>(ssid.c_str()); // prevede string na char*
    char *p = const_cast<char *>(password.c_str());
    delay(400);
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(s, p);
        for (int i = 0; i < 50; i++) { // kontroluje zda bylo pripojeno k wifi, po 5s se vypne esp
            if (WiFi.status() == WL_CONNECTED)
                break;
            delay(100);
            Serial.print(".");
        }
        if (WiFi.status() != WL_CONNECTED) {// pokud neni pripojen ohlasi chybu
            feedback_timer(200, 2);
            Serial.println("Nepripojeno");
            WiFi.disconnect();
            setWiFiServer2();
        }
    }
}

bool check_conf_mode() {
    // overi zda jsou oba piny zmackle pro konfiguracni mod
    if (!BUTTONSTATE1 && !BUTTONSTATE2) {
        feedback_timer(400, 1);
        Serial.println("Konf");
        setWiFiServer2();
        return true;
    } else if (BUTTONSTATE1 && BUTTONSTATE2) {
        feedback_timer(400, 3);
        ESP.deepSleep(0);
    } else
        return false;
}


void send_message() {
    wifi_setup();
    http_post();
}

void setup_boot() {
    pinMode(16, OUTPUT);
    digitalWrite(16, HIGH);
    pinMode(WAKEUP_PIN1, INPUT);
    pinMode(WAKEUP_PIN2, INPUT);
    BUTTONSTATE1 = digitalRead(WAKEUP_PIN1);
    BUTTONSTATE2 = digitalRead(WAKEUP_PIN2);
    Serial.begin(115200);
    EEPROM.begin(512);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Načtení pinu");
}

void debug() { // debug zpravy z pameti
    Serial.println(readEEPROM(SSID_POS, 64));
    Serial.println(readEEPROM(PASSWORD_POS, 64));
    Serial.println(readEEPROM(IP_POS1, 15));
    Serial.println(readEEPROM(HTTP_POS1, 100));
    Serial.println(readEEPROM(IP_POS2, 15));
    Serial.println(readEEPROM(HTTP_POS2, 100));
    Serial.println(BUTTONSTATE1);
    Serial.println(BUTTONSTATE2);
}

void setup() {
    setup_boot();
    debug();
    if (!check_conf_mode()) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("nekonf");
        send_message();
        digitalWrite(16, LOW);
        ESP.deepSleep(0);
    }
}

void loop() {
    delay(1);
    handleServer();
}
