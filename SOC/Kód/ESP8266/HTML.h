#ifndef _HTML_h
#define _HTML_h_
#include <Arduino.h>

const char indexHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
        <style>
          input[type="submit"] {
            background-color: #4CAF50; /* Green */
            color: white;
            width: 200px;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            display: block;
            margin: 0 auto;
          }
          button {
            background-color: #4CAF50; /* Green */
            color: white;
            width: 150px;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            display: block;
            margin: 0 auto;
          }
          .rssi{
            color: green;
           text-align: center;
          }
          .encrypt1{
            color: green;            
            text-align: center;
          }
        </style>
    </head>
    <body>
    <center>
        <h1>NETIO BUTTON</h1>
        <button onclick="window.location.href=window.location.href">Scan</button>
        <button onclick="location.href = '/netioProduct';">NETIOIP</button>
        <button onclick="location.href = '/buttonConfigure';">Configure</button>
        <button onclick="location.href = '/settings';">Settings</button>
        <button onclick="location.href = '/deepsleep';">Deep sleep</button>
        <button onclick="location.href = '/disconnect';">Disconnect</button>
        <p>Scan can takes a while</p>
        <h1>WIFIs</h1>
        
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
                button.class = 'button';
                button.type = 'submit';
                button.value = networks[i];

                var form = document.createElement('form');
                if(protection[i] == "None"){
                    form.method = 'POST';
                    form.action = '/wifi/redirect'
                } else{
                    form.method = 'GET'
                    form.action = '/wifi';
                }
                

                var inputs = document.createElement('input');
                inputs.type = 'hidden';
                inputs.name = 'ssid';
                inputs.value = networks[i];
                var rssi1 = document.createElement("p");
                rssi1.innerHTML = rssi[i];
                rssi1.className = 'rssi';
                var encrypt = document.createElement("p");
                encrypt.innerHTML = protection[i];
                encrypt.className = 'encrypt1';

                document.body.appendChild(form);
                form.appendChild(inputs);
                form.appendChild(button);
                form.appendChild(rssi1);
                form.appendChild(encrypt);
            
            }
        </script>
    </body>
</html>
)rawliteral";

const char passwordHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
    <meta charset="UTF-8">
    <style>
          input[type=password], select {
            width: 50%;
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
}
          button {
            background-color: #4CAF50; /* Green */
            color: white;
            width: 150px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
          }

        </style>
    </head>
        <center>
        <form method ="POST" action ="/wifi/redirect">
            <label for = "password">WiFi password</label>
            <br>
            <input type="password" name="password" id="password">
            <button type="submit">submit</button>
            <p>WiFi name: </p>
        </form>
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
<html lang="cs">
<head>
    <style>
        button {
            background-color: #4CAF50; /* Green */
            color: white;
            width: 200px;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 0 auto;
        }
        input[type=text] {
            width: 50%;
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }
    </style>
    <title>NETIO Button</title>
</head>
<body>
<center>
    <meta charset="UTF-8">
    <form method ="POST" action ="/netioProduct/check">
        <label for = "ip1">IP OF NETIO PRODUCT</label>
        <input type="input" name="ip1" id="ip1">
        <button type="submit">submit</button>
    </form>
    <form method ="POST" action ="/netioProduct/check">
        <label for = "ip2">IP OF NETIO PRODUCT</label>
        <input type="input" name="ip2" id="ip2">
        <button type="submit">submit</button>
    </form>
    <button onClick="location.href = '/';">Return</button>
</center>
<script>
            const queryString = window.location.search;
            const urlParams = new URLSearchParams(queryString);

            var input1 = document.getElementsByTagName('input')[0];
            wifiname.value = urlParams.get('ssid');
            form.appendChild(wifiname);
            form.appendChild(document.createTextNode(urlParams.get('ssid')));
</script>
</body>
</html>
)rawliteral";


const char configHTML[] PROGMEM = R"rawliteral(
<html>
    <head>
        <meta charset="UTF-8">
        <style>
        button {
            background-color: #4CAF50; /* Green */
            color: white;
            width: 200px;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            display: block;
            margin: 0 auto;
          }
          input[type=text]{
            width: 50%;
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
}
          </style>
    </head>
    <body>
    <center>
        <form method ="POST" action ="/buttonConfigure/check">
            <label for = "button1">Config 1st button</label>
            <input type="input" name="button1" id="button1">
            <button type="submit">submit button 1</button>
        </form>
        <form method ="POST" action ="/buttonConfigure/check">
            <label for = "button2">Config 2nd button</label>
            <input type="input" name="button2" id="button2">
            <button type="submit">submit button 2</button>
        </form>
        <button onClick="location.href = '/';">Return</button>
    </body>
</html>
)rawliteral";

#endif
