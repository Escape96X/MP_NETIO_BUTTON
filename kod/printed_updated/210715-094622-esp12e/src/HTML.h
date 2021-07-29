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
            background-color: #005F41; /* Green */
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
            background-color: #005F41; /* Green */
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

        .center {
            text-align: center;
        }
    </style>
</head>
<body>
<center>
    <h1>NETIO BUTTON</h1>
    <button onclick="window.location.href=window.location.href">Scan networks</button>
    <button onclick="location.href = '/netioProduct';">Actions</button>
    <button onclick="location.href = '/deepsleep';">Disable conf-mode</button>
    <button id ="FR" onclick="location.href = '/disconnect';">Factory reset</button>
    <button onclick="location.href = '/manual';">Manual</button>
    <p>Scan can takes a while</p>

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
                    currentWiFi(data.ssid, data.ip);
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

        function currentWiFi(ssid, ip) {
          var desWiFi = document.createElement("h3");
          desWiFi.innerHTML = "Current SSID:";
          desWiFi.className = "center";
          document.body.appendChild(desWiFi);

          var ssidwifi = document.createElement("p");
          ssidwifi.innerHTML = ssid;
          ssidwifi.className = "center";
          document.body.appendChild(ssidwifi);

          var desIP = document.createElement("h3");
          desIP.innerHTML = "Current IP: ";
          desIP.className = "center";
          document.body.appendChild(desIP);

          var ipwifi = document.createElement("p");
          ipwifi.innerHTML = ip;
          ipwifi.className = "center";
          document.body.appendChild(ipwifi);

          var desWiFi1 = document.createElement("h1");
          desWiFi1.innerHTML = "Select your new Network:";
          desWiFi1.className = "center";
          document.body.appendChild(desWiFi1);
            
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
            background-color: #005F41; /* Green */
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
        border-collapse: collapse;
        width: 100%;
    }

th, td {
  padding: 8px;
  text-align: left;
  border-bottom: 1px solid #ddd;
}

    </style>
    <title>NETIO Button</title>
</head>
<body style="text-align: center;">
<h1>IP Adresses of NETIO sockets</h1>
<button type="button" onclick="location.href = '/netioProduct/add';">ADD</button>
<button onClick="location.href = '/';">Return</button>
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

    function tableCreation(numbOfIp, IP_address, groupRecog) {
        let table = document.createElement('table');
        if (groupRecog === "A") {
            table.id = "tableA";
        } else {
            table.id = "tableB";
        }
        table.border = "1";
        let headerRow = table.insertRow(0)
        let headerCell1 = document.createElement("th");
        headerCell1.innerHTML = "#";
        let headerCell2 = document.createElement("th");
        headerCell2.innerHTML = "JSON";
        let headerCell3 = document.createElement("th");
        headerCell3.innerHTML = "IP Address";
        let headerCell4 = document.createElement("th");
        headerCell4.innerHTML = "Action";

        headerRow.appendChild(headerCell1);
        headerRow.appendChild(headerCell2);
        headerRow.appendChild(headerCell3);
        headerRow.appendChild(headerCell4);

        for (let i = 0; i < numbOfIp; i++) {

            let row = table.insertRow(i+1);
            if(groupRecog === "A"){
                row.id = "rowA" + i;
            } else {
                row.id = "rowB" + i;
            }
            let numberOfIp = row.insertCell(0);
            getHTTP(i, groupRecog);
            numberOfIp.innerHTML = i;
            let IP = row.insertCell(1);
            IP.innerHTML = IP_address[i];

            let deleteButtonCell = row.insertCell(2);
            let deleteButton = document.createElement('button');
            deleteButton.type = 'submit';
            deleteButton.value = i;
            deleteButton.innerHTML = "Delete";

            let IPNumber = document.createElement("input");
            IPNumber.type = "hidden";
            IPNumber.name = "IPNumber";
            IPNumber.value = i;

            let group = document.createElement("input");
            group.type = "hidden";
            group.name = "group";
            if (groupRecog === "A") {
                group.value = "true";
            } else {
                group.value = "false";
            }


            let form = document.createElement('form');
            form.method = 'GET'
            form.action = '/netioProduct/delete'
            document.body.appendChild(table);
            deleteButtonCell.appendChild(form);
            form.appendChild(deleteButton);
            form.appendChild(group);
            form.appendChild(IPNumber);
        }
    }

    function getHTTP(i, groupRecog) {
        let httpRequest = new XMLHttpRequest();
        let urlHTTP = "0";
        if(groupRecog === "A"){
            urlHTTP = "http_planeA";
        } else {
            urlHTTP = "http_planeB";
        }
        httpRequest.onreadystatechange = function () {
            if (this.readyState === 4 && this.status === 200) {
                let httpstring = this.responseText.split("!");
                let nameRow = "";
                if(groupRecog === "A"){
                    nameRow = "rowA";
                } else {
                    nameRow = "rowB";
                }       
                let row1 = document.getElementById(nameRow + i);
                let HTTP = row1.insertCell(1);
                HTTP.innerHTML = httpstring[i];
            }
        };
        httpRequest.open("GET", urlHTTP, true);
        httpRequest.send();
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
        body {
            font-family: "Helvetica";
        }

        button {
            background-color: #005F41;
            color: white;
            margin: 10px;
            padding: 15px 32px;
            text-align: center;
            font-size: 16px;
            display: block;
            border-radius: 4px;
            width: 150px;
            text-decoration: none;
            transition: 0.3s;
        }

        button:hover {
            background-color: #3d8b40;
        }

        input[type=text] {
            width: 30%;
            height: 40px;
            margin: 10px;
            font-size: large;
            border: 2px solid black;
            border-radius: 4px;
        }

        input:invalid {
            border: 2px solid red;
            border-radius: 4px;
        }

        label {
            font-size: 20px;
        }

        select {
            margin: 20px;
            font-size: 20px;
        }
    </style>

</head>
<body>
<center>
    <h1>Add Netio device</h1>
    <form method="GET" action="/netioProduct/check">
        <label htmlFor="addIP">IP of Netio socket</label> <br>
        <input type="text" pattern="[0-9.]{7,15}" required="required" name="addIP"
               title="Invalid IP address" id="addIP"><br>
        <label htmlFor="http">JSON String</label><br>
        <input type="text" required="required" maxlength="149" name="http"><br>
        <label htmlFor="group">Button selection</label><br>
        <select name="group" , id="group">
            <option value=true>Button #1</option>
            <option value=false>Button #2</option>
        </select><br>
        <button type="submit">Submit</button>
    </form>
    <button onClick="location.href = '/';">Return</button>
</center>
</body>
</html>
)rawliteral";

const char tutorialHTML[]
PROGMEM = R"rawliteral(
<html>
<head>
<style>
        body {
            font-family: "Helvetica",serif;

        }
    )
</style>
</head>
<body>
<h1>NETIO button tutorial</h1>
<h2>How to set it up</h2>
<ol>
<li>Turn it on with ON/OFF switch</li>
<li>Hold both buttons until red light blinks once.</li>
<li>Open WiFi manager and find <b>NETIO-BUTTON</b> network and search <b>192.168.4.1</b> in internet browser</li>
<li>Under WiFis find your WiFi and click on it after that enter your password</li>
<li>After connecting, copy IP address of the device, reconnect to your network and enter new IP to your browser</li>
</ol>
<h2>Connect to Netio socket</h2>
<ol>
<li>Select <b>Action list</b> on main screen</li>
<li>Click on button <b>add</b></li>
<li>Enter IP adress of Netio socket (only IP without http:// or /netio.json)</li>
<li>Enter JSON string</li>
<li>Choose which button should use this command and submit</li>
<li> You can delete each record in table by clicking on button <b>Delete</b>
<li> On main screen click on button <b>Deep sleep</b> to end <b>conf-mode</b>
</ol>

<h2>Basic info</h2>
<p>
This is prototype of NETIO button that allow you to control the NETIO socket.
It works on sending HTTP request method.
On the other side, this communication is handled by the socket via JSON API. Button is made with small consumption. Button is only up when user sends HTTP request or turns on conf-mode.
</p>
<h3>Device structure</h3>
<p>
Device has 2 buttons with S1 and S2 marks. They are used for <b>JSON configure</b>.
User can program those buttons via conf-mode. <br>
</p>
<h3>LED and buzzer signaling</h3>
<p>
Device has LED with green and red color.
Buzzer is connected with red color of LED.<br>
If <b>green light</b> is on, device is trying to connect to wlan or communicate with the socket. <br>
If <b>red light</b> blinks once for longer time, device entered or quited conf-mode.<br> 
If <b>red light</b> blinks 2x with buzzer signalization, connection to wlan failed. <br>
If <b>red light</b> blinks 3x with buzzer signalization, HTTP communication failed.
Probably IP address of the socket is invalid. <br>
If <b>red light</b> blinks 4x with buzzer signalization, device wasn't able to recognize input. Please hold the button little bit longer.
</p>
<h2>Conf-mode</h2>
<p>
For entering conf-mode, hold both buttons until light turn on for 400ms. After that connect to AP called <b>NETIO-BUTTON</b>. Open browser and search <b>192.168.4.1</b>.
</p>
<h3>WiFi selection</h3>
<p>
On main screen, user is able to connect device to any close-range network without security or with WES, WPA or WPA2-personal. For update WiFi list, click on <b>Scan</b> button. <br>
By clicking on button with your wanted network, you will be transfer to security page (only if network is secured). If password is checked, page will show you IP address of device selected by DHCP server in network. And now you are able to connect to conf-mode via your network and appropriate IP address.
</p>
<h3>IP address of the socket</h3>
<p>
User is able to set 10 IP addresses for each button. So one HTTP request will be send a maximum of 10 devices at once. You can add IP address via form where you also select with button should use this IP address. It is possible to delete each address.
</p>
<h3>JSON message</h3>
<p>
Behind button configure you can find feature to write JSON message to JSON API of socket. Current JSON is below the form.
</p>
<h3>Disable conf-mode button</h3>
<p>This button will put device to sleep. Holding both buttons during conf-mode has same effect.</p>
<h3>Factory reset button</h3>
<p>Deletes whole EEPROM.</p>

</body>
</html>
)rawliteral";

#endif
