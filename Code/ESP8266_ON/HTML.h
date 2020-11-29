#ifndef _HTML_h
#define _HTML_h_
#include <Arduino.h>

const char indexHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
    </head>
    <body>
        <h1>WIFI</h1>
        
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
                button.type = 'submit'
                button.value = networks[i];

                var form = document.createElement('form');
                if(protection[i] == "None"){
                    form.method = 'POST';
                    form.action = '/wifi/check'
                } else{
                    form.method = 'GET'
                    form.action = '/wifi';
                }
                

                var inputs = document.createElement('input');
                inputs.type = 'hidden';
                inputs.name = 'ssid';
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
        <button onclick="window.location.href=window.location.href">Scan</button>
        <button onclick="location.href = '/netioProduct';">NETIOIP</button>
        <p>Scan take a while</p>
    </body>
</html>
)rawliteral";

const char passwordHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
        <form method ="POST" action ="/wifi/check">
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
            wifiname.value = urlParams.get('ssid');
            form.appendChild(wifiname);
            form.appendChild(document.createTextNode(urlParams.get('ssid')));


        </script>
        <button onClick="location.href = '/';">Return</button>
    </body>
</html>
)rawliteral";

const char NetioHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
        <form method ="POST" action ="/netioProduct/check">
            <label for = "ip">IP OF NETIO PRODUCT</label>
            <input type="input" name="ip" id="ip">
            <button type="submit">submit</button>
        </form>
    </head>
    <body>
        <button onClick="location.href = '/';">Return</button>
    </body>
</html>
)rawliteral";

#endif
