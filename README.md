# Manual
## How to install on esp8266 with Netio ESP flasher
Best method is to use Visual studio code with plugin Platformio.  
1. Download 'Kód' folder of this repository
2. After installation of VS code and Platformio override esptool.py in platformio
	- Windows path: C:\Users\\**username**\\.platformio\packages\tool-esptoolpy\esptool.py
3. Connect
	- Rx to Tx
	- Tx to Rx
	- GND to GND
	- PROG to PIO0
	- <a href = 'https://github.com/jiricmi/MP_NETIO_BUTTON/issues/14#issuecomment-888953459'>3,3 V to 3,3 V (You have to solder that to R2)</a>
	- <a href = 'https://github.com/jiricmi/MP_NETIO_BUTTON/issues/14#issuecomment-888953459'>RST to ESPEN (solder to RST button)</a>
4. Upload with platformio

<h2>NETIO button tutorial</h2>
<h3>How to set it up</h3>
<ol>
<li>Turn it on with ON/OFF switch</li>
<li>Hold both buttons until red light blinks once.</li>
<li>Open WiFi manager and find <b>NETIO-BUTTON</b> network and search <b>192.168.4.1</b> in internet browser</li>
<li>Under WiFis find your WiFi and click on it after that enter your password</li>
<li>After connecting, copy IP address of the device, reconnect to your network and enter new IP to your browser</li>
</ol>
<h3>Connect to Netio socket</h3>
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

# Obsah
📦MP_NETIO_BUTTON  
 ┣ 📂kod - - **Finální program**  
 ┣ 📂dokumentace - **Dokumentace s latex soubory**  
 ┃ ┗ 📜document.pdf   
 ┣ 📂mereni - **Waveform a screenshoty**    
 ┃ ┣ 📂esp32  
 ┃ ┃ ┣ 📂operace  
 ┃ ┃ ┣ 📂ustalene_stavy  
 ┃ ┃ ┗ 📜vysledky_mereni_esp32.xlsx  
 ┃ ┗ 📂esp8266  
 ┃ ┃ ┣ 📂operace  
 ┃ ┃ ┣ 📂ustalene_stavy  
 ┃ ┃ ┗ 📜vysledky_mereni_esp8266.xlsx  
 ┣ 📂navrh_vzorku - **Eagle**    
 ┃ ┣ 📂esp32  
 ┃ ┗ 📂esp8266  
 ┣ 📜.gitignore  
 ┗ 📜README.md  
# Poděkování
*Chtěl bych poděkovat panu učiteli Ing. Břetislavu Bakalovi za odborné vedení práce
a cenné rady, které mi pomohly tuto práci zkompletovat. Rád bych také poděkoval technickému řediteli Ing. Břetislavu Bakalovi ml. společnosti NETIO products a.s. za cenné
rady, věcné připomínky a vstřícnost při konzultacích a vypracování maturitní práce*.
# Úvod maturitní práce
V této maturitní práci porovnám platformy ESP8266 a ESP32 z hlediska spotřeby a reakčního času v takových podmínkách, aby bylo možné určit platformu, která je vhodnější pro funkční vzorek ovladače zásuvek od společnosti NETIO Products a. s. Jelikož je cílem vytvořit co nejvíce úsporné zařízení, jak je jen možné, budu porovnávat jednotlivé ustálené stavy. Zařízení také musí být uživatelsky přívětivé, tzn. není ideální, když
uživatel musí čekat na reakci několik sekund.

Dále na každé platformě porovnám rychlost a spotřebu připojení k WiFi síti a následné odeslání HTTP requestu.
Po vybrání vhodné metody ustáleného stavu, vytvořím konfigurační rozhraní pro editaci nastavení sítě a HTTP requestu, do kterého se bude přistupovat přes access point zařízení ESP.  

Ve finále vytvořím dva funkční koncepty ovladačů pro každou platformu. Tento koncept
bude vycházet z předchozího měření a zjištění. Funkčním vzorkem bude možné ovládat,
buď dvě různé zásuvky, nebo jednu zásuvku z dvěma výstupy. V závěru práce zhodnotím,
zda je možné vytvořit plnohodnotný produkt, či nikoliv.

# Zadání maturitní práce

- V dokumentaci popište technologie NETIO, ESP8266 a ESP32 s ohledem na požadované funkce ovladače
- Navrhněte vhodné uživatelsky přívětivé rozhraní pro konfiguraci ovladače a jeho
připojení do WiFi sítě
- Realizujte požadované funkce na ovladač:
	- vstup na 2 nezávislá tlačítka pro standardní dvojitý přepínač/spínač dvou samostatných zásuvek jednoho zařízení NETIO, eliminace rychlých mačkání (nervózní uživatel)
	- nepodařený příkaz/přepnutí vypínače (nepodaří se odeslat příkaz http) je indikován bliknutím LED a pípnutím buzzeru. Kontroluje se odpověď http serveru (např. 200, 404 návratové hodnoty)
	- Zvolte vhodnou technologii výroby funkčních vzorků (nepájivé pole, DPS) a vyrobte jeden funkční vzorek na bázi ESP8266 a druhý na bázi ESP32 s možností měření odebíraného proudu
	- Určete vhodnou metodu měření dynamické spotřeby energie v čase (reakční doby) pro různé režimy připojení, způsobu komunikace a platformy procesoru:
		- ESP je vypnuto a vzbudí se nějakým externím obvodem přes enable/reset vstup po stisku tlačítka
		- ESP je v deep sleep módu - low power režimu a vzbudí se tlačítkem
 		- ESP je kontinuálně zapnuto
		- Kolik energie spotřebují uvedené režimy při napájení z baterie. Výsledky měření názorně porovnejte
	- Celou dokumentaci včetně výpisů zdrojového programu s komentářem veďte ve verzovacím systému Git a elektronicky doložte k tištěné podobě dokumentace
## Kritéria funkčnosti
- vyrobené vzorky ovladače (ESP8266 a ESP32) jsou funkční
- je vytvořeno uživatelské rozhraní pro konfiguraci ovladač
- bylo provedeno měření spotřeby a doby odezvy pro oba typy ovladače

## Výstupy
- srovnání platforem ESP32 a ESP8266 s ohledem na požadované funkce
- návrh požadovaných funkcí ovladače
- návrh variant ovladače s platformou ESP32 a ESP8266 ve formě ověření konceptu
- zdrojové kódy v repozitáři GIT (GitHub nebo GitLab)
- schéma zapojeni všech zkoušených variant v řešení
- srovnání spotřeby a reakční doby funkčních vzorků
