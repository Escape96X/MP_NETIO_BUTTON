#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "web.h"
// piny

const int WAKEUP_PIN1 = 5;
const int WAKEUP_PIN2 = 13;
const int BUZZER_PIN = 4;
const int LED_PIN = 12;
const int ENPin =  2;      // the number of the Enable pin
bool BUTTONSTATE1 = false;
bool BUTTONSTATE2 = false;
WiFiClient wificlient;
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

void http_post(String HTTP_CONNECTION) {
    if (http.begin(wificlient, HTTP_CONNECTION)) {
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

void parsingIP(){
    int offset = (pin_pressed()) ? 0 : 188;
    for(int i=0+offset; i<171+offset; i+=16){
        char character = EEPROM.read(i);
        if (character == 35) {
            String HTTP_CONNECTION = "http://";
            HTTP_CONNECTION += readEEPROM(i+1, 15);
            HTTP_CONNECTION += "/netio.json";
            http_post(HTTP_CONNECTION);
            
        }
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
       // digitalWrite(ENPin, LOW);
       ESP.deepSleep(0);
       return false;
    } else
        return false;
}


void send_message() {
    wifi_setup();
    parsingIP();
}
//OTESTOVAT
// void test(){
//     for(int i=0; i<= IP_LEN; i++){
//         if(i ==0 || i%16==0)
//             EEPROM.write(i,'#');
//         else
//             EEPROM.write(i, '$');
//         if(i-IP_LEN ==0 || i-IP_LEN%16==0)
//             EEPROM.write(i,'#');
//         else
//             EEPROM.write(i + IP_LEN, '$');
//     }
//     EEPROM.commit();
// }

void setup_boot() {
    pinMode(ENPin, OUTPUT);
    digitalWrite(ENPin, HIGH);
    pinMode(WAKEUP_PIN1, INPUT_PULLUP);
    pinMode(WAKEUP_PIN2, INPUT_PULLUP);
    BUTTONSTATE1 = digitalRead(WAKEUP_PIN1);
    BUTTONSTATE2 = digitalRead(WAKEUP_PIN2);
    Serial.begin(74880);
    EEPROM.begin(1024);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Načtení pinu");
    //test
    //test();
    //test
}

void debug() { // debug zpravy z pameti
    Serial.println(readEEPROM(SSID_POS, 64));
    Serial.println(readEEPROM(PASSWORD_POS, 64));
    //Serial.println(readEEPROM(IP_POS1, 15));
    Serial.println(readEEPROM(HTTP_POS1, 100));
    //Serial.println(readEEPROM(IP_POS2, 15));
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
        // digitalWrite(ENPin, LOW);
        ESP.deepSleep(0);
       
    }
}

void loop() {
    delay(1);
    handleServer();
    if (!digitalRead(WAKEUP_PIN1)&&!digitalRead(WAKEUP_PIN2)){
      //digitalWrite(ENPin, LOW);
      ESP.deepSleep(0);
      }
    
}
