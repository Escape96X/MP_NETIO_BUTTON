# Maturitní práce
## Zadání
- V dokumentaci popište technologie NETIO, ESP8266 a ESP32 s ohledem na požadované funkce ovladače
- Navrhněte vhodné uživatelsky přívětivé rozhraní pro konfiguraci ovladače a jeho
připojení do WiFi sítě
- Realizujte požadované funkce na ovladač:
	- vstup na 2 nezávislá tlačítka pro standardní dvojitý přepínač/spínač dvou samostatných zásuvek jednoho zařízení NETIO, eliminace rychlých mačkání (nervózní uživatel)
	- nepodařený příkaz/přepnutí vypínače (nepodaří se odeslat příkaz http) je indikován bliknutím LED a pípnutím buzzeru. Kontroluje se odpověď http serveru
(např. 200, 404 návratové hodnoty)
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
