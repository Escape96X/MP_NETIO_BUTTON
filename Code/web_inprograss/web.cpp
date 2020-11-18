#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiScan.h>
#include <web.h>

const char *OWN_SSID = "NETIO_BUTTON";
const char indexHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
    </head>
    <body>
        <script>
            var getJSON = function(url, callback) {
                var xhr = new XMLHttpRequest();
                xhr.open('GET', url, true);
                xhr.responseType = 'json';
                xhr.onload = function() {
                    var status = xhr.status;
                    if (status === 200) {
                     callback(null, xhr.response);
                    } else {
                        callback(status, xhr.response);
                    }
                };
                xhr.send();
            };
            getJSON('scannedWiFi.json',
            function(err, data) {
                if (err !== null) {
                    alert('Something went wrong: ' + err);
                } else {
                    for(i=0; i < data.numOfNetworks; i++){
                        buttons(i, data.networks, data.strengh, data.protection);
                    }
                }
            });

            function buttons(i, networks, rssi, protection){

                var button = document.createElement('input');
                button.innerHTML = networks[i];
                button.type = 'submit'

                var form = document.createElement('form');
                form.method = 'GET';
                form.action = '/wifi';

                var inputs = document.createElement('input');
                inputs.type = 'hidden';
                inputs.name = 'wifi';
                inputs.value = networks[i];
                var rssi1 = document.createTextNode(rssi[i]);
                var encrypt = document.createTextNode(protection[i])
                document.body.appendChild(form);
                form.appendChild(inputs);
                form.appendChild(document.createElement('br'));
                form.appendChild(button);
                form.appendChild(document.createElement('br'));
                form.appendChild(rssi1);
                form.appendChild(document.createElement('br'));
                form.appendChild(encrypt);
            
            }

        </script>
    </body>
</html>
)rawliteral";

const char paswordHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
        <form method ="POST" action ="/wifi/aprove">
            <label for = "password">WiFi password</label>
            <input type="password" name="password" id="password">
            <button type="submit">submit</button>
        </form>
    </head>
    <body>
        <script>
            const queryString = window.location.search;
            const urlParams = new URLSearchParams(queryString);

            var form = document.getElementsByTagName('form')[0];
            var wifiname = document.createElement('input');
            wifiname.type = "hidden";
            wifiname.name = "ssid";
            wifiname.value = urlParams.get('wifi');
            form.appendChild(wifiname);
            form.appendChild(document.createTextNode(urlParams.get('wifi')));


        </script>
    </body>
</html>
)rawliteral";



String ssid = "";
String password = "";

ESP8266WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);



String scanNetwork(){
  int numberOfNetworks = WiFi.scanNetworks();
  String networks = "{\"numOfNetworks\": \"";
  networks += numberOfNetworks;
  networks += "\", \"networks\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += WiFi.SSID(i);
    networks +="\"";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks +="], \"strengh\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += WiFi.RSSI(i);
    networks +="\" ";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks += "], \"protection\": [";
  for(int i = 0; i < numberOfNetworks; i++){
    networks += "\"";
    networks += (WiFi.encryptionType(i) == ENC_TYPE_NONE)? "None": "Encrypted";
    networks +="\" ";
    networks += (i+1 == numberOfNetworks) ? " ":", ";
  }
  networks += "]}";
  return networks;
}


void connectToWiFi(){
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.println(WiFi.status());   
}


void APSet(){
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(OWN_IP, OWN_IP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(OWN_SSID);
}

void test(){
  server.send(200, "text/html", indexHTML);
}

void handleWiFiConnect(){
    server.send(200, "text/html", paswordHTML);
}

void handleScanWiFi(){
    server.send(200, "application/json", scanNetwork());
}

void handleSetPasword(){
    Serial.println("jsem v handle");
    if(server.hasArg("ssid")){
        ssid = server.arg(0);
        Serial.println("mam heslo");
        password = server.arg(1);
        connectToWiFi();
    }
    server.send(200);
}

void setWiFiServer(){
  APSet();
  delay(500);
  server.on("/scannedWiFi.json", handleScanWiFi);
  server.on("/", test);
  server.on("/wifi", handleWiFiConnect);
  server.on("/wifi/aprove", HTTP_POST, handleSetPasword);
  server.begin();
}


void handleServer(){
    server.handleClient();
}