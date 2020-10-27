# Poslední
## Milan 
- pracoval jsem na schéma zapojení UARTU v eaglu
    - prosím o kontrolu
    - schema/UART_enable_switch.png
- zakomponoval jsem můj nápad jak zapínat a vypínat ESP pomocí jednoho tlačítka a JK klopného obvodu









# TODO
## Rozpracované
### Všeobecné
- Rozchodit osciloskop, zatím jako demo, HW bude předán po rozchození zapojení na nepájivém poli
### ESP
- enable pin - napiš jak jsi daleko
- Využít přímo chip - 26.10.2020 mezi 9 až 11hod - mohu ve škole předat napájený ESP-07 na dvě řady pinů.
- převodník také viz bod výše - datasheet CP2102 jksem nahrál minule, obsahuje zdroj 3,3V/100 mA, úrovně signálů jsou rovněž 3,3V
- připojení stabilizatoru na esp 3,3V!!! POZOR NA TO  -  zatím doporučuji zkoušet bez stabilizátoru (ten řešit ve spojení s baterií), použít napájení z USB/UART doplněné paralelně vyhlazovacím elektrolytickým kondenzátorem min 1000 uF/5V - pozor na polaritu.
## 3D1
- tlačítko sepne logicky clen na enable
- nastudovat tuto problematiku - máš nějaký návrh jak to zkusit? Mám návrh (schema/UART,enable switch.png) Pros
#### Milan Aktualizace
- vytvořil jsem schéma ve složce schéma
### 3D2
- nastudovat deep sleep mode - měls možnost?
### Nakreslit schéma (Fritzing, Eagle)
- než to připojíš k USB napájení, pošli mi schema zapojení
## Hotovo
- Zásuvka komunikuje přes HTTP s ESP - verze 8266? vyzkouším zítra sketch
- Základní struktura dokumentu  - první nástřel vypadá dobře (nekontroloval jsem typ písma, řádkování a další podrobnosti v dokumentu k vypracování DMP - ještě kouknu až toho bude víc), doladíme až tam budou třeba nějaké obrázky a tabulky (číslování), také odkazy na citovanou literaturu, poznámky pod čarou. Hlavně aby pak bylo možné styl lehko upravit.


# Seznam zapůjčených věcí
- NETIO COBRA
- ESP32 Modul
- ESP8266
- slot na baterie
- NFC
- Akumulátor
- stabilizator napětí
