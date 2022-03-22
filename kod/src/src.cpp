#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>

// vlastni headers
#include "web.h"
#include "define.h"
#include "memory.h"
#include "basic_functions.h"
#include "json.h"



// globalni promnene
bool button_state1 = false;
bool button_state2 = false;
bool error_http = false;


WiFiClient wificlient;
HTTPClient http;

 /*Send request to netio socket*/
String http_post(String http_ip, String post_json, String http_uname, String http_pass) {
    char *u = const_cast<char *>(http_uname.c_str()); // prevede string na char*
    char* p = const_cast<char *>(http_uname.c_str());
    Serial.println("predelal jsem na chary");
    if(http.begin(wificlient, http_ip)) {
        Serial.println("jsem pred autorizaci");
        http.setAuthorization(u, p);
        Serial.println("zacal jsem http");
        int http_code = http.POST(post_json);
        if(http_code > 0) {
            Serial.print("HTTP CODE: ");
            Serial.println(http_code);
            String payload = http.getString();
            Serial.print("Feedback: ");
            Serial.println(payload);
            if(payload.indexOf("Errors") > 0 || payload.length() == 0) {
                error_http = true;
            }
            http.end();
            return payload;
        } else {
            String payload = "HTTP failed with code:";
            payload += http_code;
            return payload;
        }
    }
    Serial.println("HTTP begin failed");
    return "HTTP begin failed";
}

String parse_content() {
    Serial.println("jsem v parse");
    String file_name = (button_state1) ? "tableA.json" : "tableB.json";
    int count =(int)json_count(file_name);
    Serial.print("count je");
    Serial.println(count);
    if(count == 0) {
        feedback_timer(200, 3);
        return "TAble of actions is empty.";
    } else {
        String html_http = "<table>";
        Serial.println("jsem pred loopem");
        for(int i = 0; i < count; i++) {
            // TODO: ziskat vsechno z jsonu
            std::vector<String> data;
            data = json_read(file_name, i);
            //String& ip = data[0];
            //Serial.println("precteno");
            html_http += "<th><td>" + data[0] + "</td><td>";
            String http_ip = "http://";
            http_ip += data[0] + "/netio.json";
            //Serial.println(http_ip);
            http_post(data[1], data[0], data[2], data[3]);
            //html_http += "</td></th>";
            //Serial.println(i);

        }
        html_http = "</table>";
        if (error_http) 
            feedback_timer(200, 3);
        return html_http;
    }
}

void wifi_setup() {
    // precte z pameti ssid a password a pripoji se k wifi
    // String ssid = readEEPROM(SSID_POS, SSID_LEN);
    // String password = readEEPROM(PASSWORD_POS, PASSWORD_LEN);
    //String ssid = read_json_string("ssid", "/wifi.json");
    //String password = read_json_string("password", "/wifi.json");
    //Serial.print("SSID and Password:");
    //Serial.println(ssid);
    //erial.println(password);
    //char *s = const_cast<char *>(ssid.c_str()); // prevede string na char*
    //char *p = const_cast<char *>(password.c_str());

    delay(300);
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin("iot-test", "ahoj123456");
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
    if (!button_state1 && !button_state2) {
        feedback_timer(400, 1);
        Serial.println("Config mode enabled");
        setWiFiServer();
        return true;
    } else if (button_state1 && button_state2) {
        feedback_timer(200, 4);
        ESPSleep();
        return false;
    } else
        return false;
}


void send_message() {
    wifi_setup();
    parse_content();
}

void setup_boot() {
    // nastaveni po startu
    pinMode(ENPin, OUTPUT);
    digitalWrite(ENPin, HIGH);
    pinMode(WAKEUP_PIN1, INPUT_PULLUP);
    pinMode(WAKEUP_PIN2, INPUT_PULLUP);
    button_state1 = digitalRead(WAKEUP_PIN1);
    button_state2 = digitalRead(WAKEUP_PIN2);
    Serial.begin(115200);
    EEPROM.begin(4096);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Načtení pinu");
}

void debug() {
    //debug zpravy z pameti
    Serial.println("WiFi login");
    Serial.println("Button state detection:");
    Serial.print("Button S1:");
    Serial.println(button_state1);
    Serial.print("Button S2:");
    Serial.println(button_state2);
    Serial.println("");
}

void setup() {
    setup_boot();
    debug();
      if (!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        feedback_timer(300, 2);
    }
    if (!check_conf_mode()) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Standard mode enabled");
        send_message();
        ESPSleep();

    }
}

void loop() {
    handleServer();
    if (!digitalRead(WAKEUP_PIN1) && !digitalRead(WAKEUP_PIN2)) {
        feedback_timer(400, 1);
        ESPSleep();
    }

}