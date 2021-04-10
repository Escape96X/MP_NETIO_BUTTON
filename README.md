# Obsah
📦MP_NETIO_BUTTON  
 ┣ 📂code - - **Finální program**  
┃ ┣ 📂ESP8266   
 ┃ ┗ 📂ESP32     
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
- je vytvořeno uživatelské rozhraní pro konfiguraci ovladač**
- bylo provedeno měření spotřeby a doby odezvy pro oba typy ovladače

## Výstupy
- srovnání platforem ESP32 a ESP8266 s ohledem na požadované funkce
- návrh požadovaných funkcí ovladače
- návrh variant ovladače s platformou ESP32 a ESP8266 ve formě ověření konceptu
- zdrojové kódy v repozitáři GIT (GitHub nebo GitLab)
- schéma zapojeni všech zkoušených variant v řešení
- srovnání spotřeby a reakční doby funkčních vzorků
