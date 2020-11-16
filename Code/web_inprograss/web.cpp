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
                    buttons(i, data.networks);
                }
            }
            });

            function buttons(i, networks){
                var button = document.createElement('input');
                button.innerHTML = networks[i];
                button.type = 'submit'
                var form = document.createElement('form');
                form.method = 'POST';
                form.action = '/wifi';
                var inputs = document.createElement('input');
                inputs.type = 'hidden';
                inputs.name = 'wifi';
                inputs.value = networks[i];
            document.body.appendChild(form);
            form.appendChild(inputs);
            form.appendChild(button);
            }

        </script>
    </body>
</html>
)rawliteral";

String ssid = "";

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
  networks += "]}";
  return networks;
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
    if(server.hasArg("wifi")){
        ssid = server.arg(0);
        server.send(200, "text/plain", "ahoj");
    }
}

void handleScanWiFi(){
    server.send(200, "application/json", scanNetwork());
}

void setWiFiServer(){
  APSet();
  delay(500);
  server.on("/scannedWiFi.json", handleScanWiFi);
  server.on("/", test);
  server.on("/wifi",HTTP_POST, handleWiFiConnect);
  server.begin();
}

void handleServer(){
    server.handleClient();
}