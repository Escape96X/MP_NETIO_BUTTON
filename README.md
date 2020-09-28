# Průběh
- ESP8266 WPS connection - funguje
- ESP8266 MQTT connection with cloud - in progress

# Zadání
## Popis finálního výrobku:
 - napajeni z baterie
 - vstup na 2 tlacitka (pro standardni dvojity prepinac/spinac do zabudovane instalace)
 - piezo buzzer (logicky rizeny, ne PWM), ktery pipne v pripade nepodareneho prikazu/prepnuté vypínace
 - vymenitelne baterie (asi AAA)
 - prevence zbesileho mačkani nejakeho tlacitka (usecase: netrpelivy uzivatel)
 - LED, ktera indikuje, ze bylo stiknuto nejake tlacitko a ze se podarilo odeslat prikaz pres HTTP (hlavne kvuli vyvoji, nemusi byt ve finalnim produktu pouzita)
 - pripojeni pres wifi
 - zarizeni je mozne jednoduse konfigurovat a pripojit do WIFI pres AP mod a jendoduchy web nebo URL API
 - zarizeni umi ovladat zasuvky NETIO pres HTTP JSON API
 - v zasade temer totozny produkt jako je WiFi Button od firmy MyStrom (https://mystrom.com/wifi-button/) - firma je ze svycarska

 ## Cíl maturitní práce:
 - zjistit možnosti a vlastnosti pro bateriovy provoz (odezva od sepnuti tlacitka, low power rezimy, rezim probuzeni/zapnuti ESP po stiku tlacitka)
 - porovnat platformy ESP8266 a ESP32 z hlediska: 
    - reakce systemu (probuzeni+pripojeni k wifi+odeslani prikazu pres HTTP)
    - spotreby a doby vydrze na baterii
 - na zaklade predchozi reserse vybrat po konzultaci s NETIO jedno reseni a realizovat proof of concept tlacitka na vývojové desce/nepájivém poli

 ## Výstupem práce:
 - rešerže a srovnání platforem ESP32 a ESP8266 z hlediska spotreby a reakcni doby
 - funkcni vzorek tlacitka ve forme proof of concept
 - zdrojove kody v repositari GIT (GitHub nebo GitLab)
 - schema zapojeni vsech zkousenych variant v resersi

## Konzultace a podpora ze strany NETIO:
 - nakup potrebneho HW
 - výstup bude majetkem firmy NETIO
 - veci okolo HW a optimalizace spotreby, hlavne oponentura - Martin Sejnoha
 - veci okolo SW a funcionality - Bretislav Bakala
