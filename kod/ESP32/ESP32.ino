#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "web.h"
// piny
#define WAKEUP_PIN2_BITMASK 0x000000004
const int WAKEUP_PIN1 = 15;
const int WAKEUP_PIN2 = 2;
const int BUZZER_PIN = 18;
const int LED_PIN = 14;
const int HOLD_PIN = 32;
esp_sleep_wakeup_cause_t wakeup_reason;
esp_reset_reason_t reset_reason;
bool button1State = false;
bool button2State = false;

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

bool pin_pressed(esp_sleep_wakeup_cause_t wakeup_reason, esp_reset_reason_t reset_reason) {
    // dle boolean promennych vrati true nebo false dle zmacknuteho tlacitka
    if (reset_reason == 1) {
        if (button1State) {
            return true;
        } else if (button2State) {
            return false;
        } else {
            WiFi.disconnect();
            delay(200);
            digitalWrite(HOLD_PIN, LOW);
            esp_deep_sleep_start();
        }
    } else {
        if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
            return true;
        } else {
            return false;
        }
    }
}

void http_post() {
    digitalWrite(HOLD_PIN, HIGH);
    // sestavi url pro komunikaci
    String HTTP_CONNECTION = "http://";
    HTTP_CONNECTION += (pin_pressed(wakeup_reason, reset_reason)) ? readEEPROM(0, 15) : readEEPROM(15,
                                                                                                   15); // nacte z pameti prislusnou ip zasuvky
    HTTP_CONNECTION += "/netio.json";
    if (http.begin(HTTP_CONNECTION)) { // zahaji se komunikace
        int httpCode = http.POST((pin_pressed(wakeup_reason, reset_reason)) ? readEEPROM(30, 100) : readEEPROM(130,
                                                                                                               100)); // nacte z pameti prislusnou JSON zpravu
        String payload = http.getString(); // ziska feedback

        if (payload.indexOf("errors") > 0 || payload == "") {
            // pokud se nachazi ve feedbacku chyba, oznami ji
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
    digitalWrite(32, HIGH);
    String ssid = readEEPROM(230, 64);
    String password = readEEPROM(294, 100);
    char *s = const_cast<char *>(ssid.c_str()); // prevede string na char*
    char *p = const_cast<char *>(password.c_str());
    WiFi.begin(s, p);
    for (int i = 0; i < 50; i++) {
        if (WiFi.status() == WL_CONNECTED) // kontroluje zda bylo pripojeno k wifi, po 5s se vypne esp
            break;
        delay(100);
        digitalWrite(HOLD_PIN, HIGH);
        Serial.print(".");
    }
    if (WiFi.status() != WL_CONNECTED) {// pokud neni pripojen ohlasi chybu
        feedback_timer(200, 2);
        Serial.println("Nepripojeno");
        WiFi.disconnect();
        digitalWrite(HOLD_PIN, LOW);
        esp_deep_sleep_start();
    }
    digitalWrite(32, HIGH);
}

bool check_conf_mode() {
    // overi zda jsou oba piny zmackle
    button1State = digitalRead(WAKEUP_PIN1);
    button2State = digitalRead(WAKEUP_PIN2);
    if (button1State && button2State) {
        feedback_timer(400, 1);
        delay(1000);
        if (digitalRead(WAKEUP_PIN1) && digitalRead(WAKEUP_PIN2)) {
            feedback_timer(400, 1);
            setWiFiServer();
        } else
            setWiFiServer2();
        return true;
    } else
        return false;
}

void deep_sleep_set() {
    // zjisti jaky pin probudil ESP a nastavi probouzeci piny na dalsi deep sleep

    wakeup_reason = esp_sleep_get_wakeup_cause();
    reset_reason = esp_reset_reason();
    Serial.println(wakeup_reason);

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_15, 1);
    esp_sleep_enable_ext1_wakeup(WAKEUP_PIN2_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
}

void send_message() {
    wifi_setup();
    http_post();
}

void setup_boot() {
    // nastaveni pinu pro dalsi postupy
    pinMode(HOLD_PIN, OUTPUT);
    digitalWrite(HOLD_PIN, HIGH);
    Serial.begin(115200);
    EEPROM.begin(512);
    pinMode(WAKEUP_PIN1, INPUT);
    pinMode(WAKEUP_PIN2, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Načtení pinu");
}

void debug() { // slouzi jako debug pameti
    Serial.println(readEEPROM(SSID_POS, 64));
    Serial.println(readEEPROM(PASSWORD_POS, 64));
    Serial.println(readEEPROM(IP_POS1, 15));
    Serial.println(readEEPROM(HTTP_POS1, 100));
    Serial.println(readEEPROM(IP_POS2, 15));
    Serial.println(readEEPROM(HTTP_POS2, 100));
}

void setup() {
    setup_boot();
    debug();
    deep_sleep_set();
    if (!check_conf_mode()) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("nekonf");
        send_message();
        WiFi.disconnect();
        delay(200);
        digitalWrite(HOLD_PIN, LOW);
        esp_deep_sleep_start();
    }
}

void loop() {
    delay(1);
    handleServer();
}
