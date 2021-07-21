#ifndef _HTML_h
#define _HTML_h_

#include <Arduino.h>

const char indexHTML[]
PROGMEM = R"rawliteral(
<html>
<head>
    <meta charset="UTF-8">
    <style>
        body{
            font-family: "Helvetica";

        }
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
            border-radius: 4px;
            font-size: 16px;
            display: block;
            margin: 0 auto;
            transition: 0.3s;
        }
        button:hover {
            background-color: #3d8b40;
            color: white;
        }

        .rssi {
            color: green;
            text-align: center;
        }

        .encrypt1 {
            color: green;
            text-align: center;
        }
    </style>
</head>
<body>
<center>
    <h1>NETIO BUTTON</h1>
    <button onclick="window.location.href=window.location.href">Scan</button>
    <button onclick="location.href = '/netioProduct';">NETIO IP</button>
    <button onclick="location.href = '/buttonConfigure';">Configure</button>
    <button onclick="location.href = '/settings';">Settings</button>
    <button onclick="location.href = '/deepsleep';">Deep sleep</button>
    <button onclick="location.href = '/disconnect';">Disconnect</button>
    <p>Scan can takes a while</p>
    <h1>WIFIs</h1>

    <script>
        var getJSON = function (url, callback) {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', url, true);  // Otevre http request
            xhr.responseType = 'json';
            xhr.onload = function () {
                var status = xhr.status; // zkontroluje se status http
                if (status === 200) {
                    callback(null, xhr.response);
                } else {
                    callback(status, xhr.response);
                }
            };
            xhr.send();
        };
        getJSON('scannedWiFi.json', // pracuje s ipAdresa/scannedWiFi.json
            function (err, data) {
                if (err !== null) { // kontrola zda se stal error
                    alert('Something went wrong: ' + err);
                } else { // vezme data z jsonu a ulozi se do pole
                    for (i = 0; i < data.numOfNetworks; i++) {
                        buttons(i, data.networks, data.strengh, data.protection);
                    }
                }
            });

        function buttons(i, networks, rssi, protection) {
            // vytvori tlacitka na zaklade json zpravy
            var button = document.createElement('input');
            button.class = 'button';
            button.type = 'submit';
            button.value = networks[i]; // hodnota se nastavi na SSID site

            var form = document.createElement('form'); // vytvori se formular do ktereho se tlacitko vlozi
            // pokud sit je zabezpecena, odkaze na overeni s heslem
            if (protection[i] == "None") {
                form.method = 'POST';
                form.action = '/wifi/redirect'
            } else {
                form.method = 'GET'
                form.action = '/wifi';
            }


            var inputs = document.createElement('input');
            inputs.type = 'hidden';
            inputs.name = 'ssid';
            inputs.value = networks[i]; // skryty input s ssid
            var rssi1 = document.createElement("p");
            rssi1.innerHTML = rssi[i]; // text s rrsi konkretni site
            rssi1.className = 'rssi';
            var encrypt = document.createElement("p");
            encrypt.innerHTML = protection[i];
            encrypt.className = 'encrypt1';
            // sestaveni html stromu
            document.body.appendChild(form);
            form.appendChild(inputs);
            form.appendChild(button);
            form.appendChild(rssi1);
            form.appendChild(encrypt);

        }
    </script>
</center>
</body>
</html>
)rawliteral";

const char passwordHTML[]
PROGMEM = R"rawliteral(
<html>
<head>
    <meta charset="UTF-8">
    <style>
        body {
            font-family: "Helvetica";

        }

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
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            border-radius: 4px;
            font-size: 16px;
            display: block;
            margin: 0 auto;
            transition: 0.3s;
        }

        button:hover {
            background-color: #3d8b40;
        }

    </style>
</head>
<center>
    <h1>Security</h1>
    <form method="POST" action="/wifi/redirect">
        <label for="password">WiFi password</label>
        <br>
        <input type="password" name="password" id="password">
        <button type="submit">submit</button>
        <p>WiFi name: </p>
    </form>
    <body>
    <script>
        const queryString = window.location.search; // slouzi k ziskani argumentu z url
        const urlParams = new URLSearchParams(queryString);

        var form = document.getElementsByTagName('form')[0]; // najdu formular na strance a ulozim do promenne
        // vytvorim skryty vstup a ulozim do nej ssid argument z url
        var wifiname = document.createElement('input');
        wifiname.type = "hidden";
        wifiname.name = "ssid";
        wifiname.value = urlParams.get('ssid');
        form.appendChild(wifiname);
        form.appendChild(document.createTextNode(urlParams.get('ssid')));


    </script>
    <button onClick="location.href = '/';">Return</button>
    </body>
</center>
</html>
)rawliteral";

const char NetioHTML[]
PROGMEM = R"rawliteral(
<html lang="cs">
<head>
    <style>
        body {
            font-family: "Helvetica",serif;

        }

        input[type=input] {
            width: 50%;
            padding: 12px 20px;
            margin: 8px 0;
            display: inline-block;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
        }

        button {
            background-color: #005F41; /* Green */
            color: white;
            width: 150px;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            border-radius: 4px;
            font-size: 16px;
            display: block;
            margin: 0 auto;
            transition: 0.3s;
        }

        button:hover {
            background-color: #3d8b40;
        }

        table {
            margin-left: auto;
            margin-right: auto;
        }

    </style>
    <title>NETIO Button</title>
</head>
<body style="text-align: center;">
<h1>IP Adresses of NETIO sockets</h1>
    <button type="button" onclick="location.href = '/netioProduct/add';">ADD</button>


<script>
    var getJSON = function (url, callback) {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', url, true);  // Otevre http request
            xhr.responseType = 'json';
            xhr.onload = function () {
                var status = xhr.status; // zkontroluje se status http
                if (status === 200) {
                    callback(null, xhr.response);
                } else {
                    callback(status, xhr.response);
                }
            };
            xhr.send();
        };
        getJSON('ip_adress.json',
            function (err, data) {
                if (err !== null) { // kontrola zda se stal error
                    alert('Something went wrong: ' + err);
                } else { // vezme data z jsonu a ulozi se do pole

                    var tableDespA = document.createElement("h3")
                    tableDespA.innerHTML = "Button #1";
                    document.body.appendChild(tableDespA);

                    tableCreation(data.numOfIPA, data.IP_adressA, "A");

                    var tableDespB = document.createElement("h3");
                    tableDespB.innerHTML = "Button #2";
                    document.body.appendChild(tableDespB);

                    tableCreation(data.numOfIPB, data.IP_addressB, "B");
                }
            });
    function tableCreation(numbOfIp, IP_address, groupRecog){
        var table = document.createElement('table');
        for(let i = 0; i < numbOfIp; i++) {

            let row = table.insertRow(i);
            let numberOfIp = row.insertCell(0);
            var IP = row.insertCell(1);
            IP.innerHTML = IP_address[i];
            
            var deleteButtonCell = row.insertCell(2);
            var deleteButton = document.createElement('button');
            deleteButton.type = 'submit';
            deleteButton.value = i;
            deleteButton.innerHTML = "Delete";

            var IPNumber = document.createElement("input");
            IPNumber.type = "hidden";
            IPNumber.name = "IPNumber";
            IPNumber.value = i;

            var group = document.createElement("input");
            group.type = "hidden";
            group.name = "group";
            if (groupRecog === "A") {
                group.value = "true";
            } else {
                group.value = "false";
            }


            var form = document.createElement('form');
            form.method = 'GET'
            form.action = '/netioProduct/delete'
            document.body.appendChild(table);
            deleteButtonCell.appendChild(form);
            form.appendChild(deleteButton);
            form.appendChild(group);
            form.appendChild(IPNumber);
        }
    }
</script>
</body>
</html>
)rawliteral";

const char NetioAddHTML[]
PROGMEM = R"rawliteral(
    <html>
    <head>
        <style>
            button {
                background-color: #005F41;
                color: white;
                margin: 10px;
                padding: 15px 32px;
                text-align: center;
                font-size: 16px;
                display: block;
                border-radius: 4px;
                width:150px;
                text-decoration: none;
                transition: 0.3s;
            }
            button:hover{
                background-color: #3d8b40;
            }
            input[type=text]{
                width: 30%;
                height: 40px;
                margin: 10px;
                font-size: large;
            }
            label{
                font-size: 20px;
            }
            select{
                margin: 20px;
                font-size: 20px;
            }
        </style>

    </head>
    <body>
        <center>
            <h1>Add Netio device</h1>
            <form method="GET" action="/netioProduct/check">
                <label for="addIP">IP of Netio socket</label> <br>
                <input type="text" maxlength="15" name="addIP" id="addIP"><br>
                <label for="group">Button selection</label><br>
                <select name="group", id="group">
                    <option value=true>Button #1</option>
                    <option value=false>Button #2</option>
                </select><br>
                <button type="submit">Submit</button>
            </form>
        </center>
    </body>
</html>
)rawliteral";


const char configHTML[]
PROGMEM = R"rawliteral(
<html>
<head>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: "Helvetica";

    }

    input[type=input] {
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
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      border-radius: 4px;
      font-size: 16px;
      display: block;
      margin: 0 auto;
      transition: 0.3s;
    }

    button:hover {
      background-color: #3d8b40;
    }
  </style>
</head>
<body>
<center>
  <h1>Configuration for JSON API</h1>
  <form method ="POST" action ="/buttonConfigure/check">
    <label for = "button1">Button #1</label><br>
    <input type="input" name="button1" id="button1">
    <button type="submit">Submit</button>
  </form>
  <form method ="POST" action ="/buttonConfigure/check">
    <label for = "button2">Button #2</label><br>
    <input type="input" name="button2" id="button2">
    <button type="submit">Submit</button>
  </form>
  <button onClick="location.href = '/';">Return</button>
</center>
</body>
</html>
)rawliteral";

#endif
