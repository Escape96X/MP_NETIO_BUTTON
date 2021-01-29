#include <Arduino.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <EEPROM.h>

// Vytvorene headery
#include "web.h"
#include "HTML.h"

String ssid = "";
String password = "";
String NetioIP = "";
String HTTPRequest1 = "";

const char *OWN_SSID = "NETIO_BUTTON";
const int RSSI_MAX = -40;
const int RSSI_MIN = -100;
bool APWork = true;

WebServer server(80);
IPAddress OWN_IP(192, 168, 4, 1);

// Fuknce pro výpočet

void saveToEEPROM(String sToSave, int startPosition)
{
  char arr[sToSave.length() + 1];
  strcpy(arr, sToSave.c_str());
  for (int i = 0; i < sToSave.length(); i++) {
    EEPROM.write(startPosition + i, arr[i]);
  }
  delay(500);
  Serial.println(EEPROM.commit());
  delay(500);
}

String readEEPROM(int numberOfStart, int len)
{
  String tempNetio = "";
  for (int i = 0; i < len; i++)
  {
    char netio = EEPROM.read(numberOfStart + i);
    if (netio == '$')
      break;
    tempNetio += netio;
    Serial.print(netio);
  }
  Serial.println(" ");
  return tempNetio;
}

int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }
  return quality;
}

String ipToString(IPAddress ip)
{
  String s = "";
  for (int i = 0; i < 4; i++)
    s += i ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

// Vytvoření JSON
String jsonOfNetworks()
{
  int numberOfNetworks = WiFi.scanNetworks();
  String networks = "{\"numOfNetworks\": \"";
  networks += numberOfNetworks;
  networks += "\", \"networks\": [";
  for (int i = 0; i < numberOfNetworks; i++)
  {
    networks += "\"";
    networks += WiFi.SSID(i);
    networks += "\"";
    networks += (i + 1 == numberOfNetworks) ? " " : ", ";
  }
  networks += "], \"strengh\": [";
  for (int i = 0; i < numberOfNetworks; i++)
  {
    networks += "\"";
    networks += dBmtoPercentage(WiFi.RSSI(i));
    networks += " %\" ";
    networks += (i + 1 == numberOfNetworks) ? " " : ", ";
  }
  networks += "], \"protection\": [";
  for (int i = 0; i < numberOfNetworks; i++)
  {
    networks += "\"";
    networks += (WiFi.encryptionType(i) == 0) ? "None" : "Encrypted";
    networks += "\" ";
    networks += (i + 1 == numberOfNetworks) ? " " : ", ";
  }
  networks += "]}";
  return networks;
}

// Nastavení AP a přípojení k WiFi
void connectToWiFi()
{
//  char* cSsid = strcpy(new char[ssid.length() + 1], ssid.c_str());
//  char* cPassword = strcpy(new char[password.length() + 1], password.c_str());
//  WiFi.begin(cSsid, cPassword);
}

void APSet()
{
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(OWN_IP, OWN_IP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(OWN_SSID);
  delay(2000);
}

void disconnectAP()
{
  APWork = false;
  WiFi.softAPdisconnect(true);
}

// Server - staticke stranky
void handleRoot()
{
  server.send(200, "text/html", indexHTML);
}

void handleWiFiConnect()
{
  server.send(200, "text/html", passwordHTML);
}

void handleScanWiFi()
{
  server.send(200, "application/json", jsonOfNetworks());
}

void handleNetioProduct()
{
  server.send(200, "text/html", NetioHTML);
}

void handleConfig()
{
  //String html = configHTML;
  //html += "BUTTON1 CODE: ";
  //html += HTTPRequest1;
  server.send(200, "text/html", configHTML);
}

// Server - dynamické stránky
void handleNetioDevice()
{
  if (server.hasArg("ip"))
  {
    NetioIP = server.arg("ip");
    NetioIP += "$";
    saveToEEPROM(NetioIP, 0);
    NetioIP = readEEPROM(0, 15);
    //disconnectAP();
    String html = "<meta http-equiv = \"refresh\" content = \"2; url = /netioProduct\" />";
    server.send(200, "text/html", html);
  }
  else
  {
    server.send(403);
  }
}

void handleWiFiPasswordRedirect()
{
  if (server.hasArg("ssid"))
  {
    ssid = server.arg("ssid");
    if (server.hasArg("password"))
      password = server.arg("password");
    else
      password = "";
    connectToWiFi();
    Serial.println("wifi");
    String html = "<meta http-equiv = \"refresh\" content = \"5; url = /wifi/check\" />";
    html += "<body><h1>CONNECTING...</h1></body>";
    server.send(200, "text/html", html);
  }
  else
  {
    server.send(200, "plain/text", "neplatne url");
  }
}

void handleWiFiApprove()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    String currentIP = "ESP IP: ";
    currentIP += ipToString(WiFi.localIP());
    currentIP += "<button style=\"background-color: #4CAF50;color: white;width: 200px;padding: 15px 32px;\" onClick=\"location.href = '/'\">Return</button>";
    server.send(200, "text/html", currentIP);
  }
  else
  {
    String badPassword = "Nesprávné heslo nebo chyba připojení <br><button onClick=\"location.href = '/wifi?wifi=";
    badPassword += ssid;
    badPassword += "';\">Return</button>";
    server.send(200, "text/html", badPassword);
  }
}

void handleConfigCheck()
{
  if (server.hasArg("button1"))
    HTTPRequest1 = server.arg("button1");
  HTTPRequest1 += "$";
  saveToEEPROM(HTTPRequest1, 60);
  HTTPRequest1 = readEEPROM(60, 50);

  //else if(server.hasArg("button2")
  //doplnit;
  String html = "<meta http-equiv = \"refresh\" content = \"2; url = /buttonConfigure\" />";
  server.send(200, "text/html", html);
}

void serversOn()
{
  server.on("/scannedWiFi.json", handleScanWiFi);
  server.on("/", handleRoot);
  server.on("/wifi", handleWiFiConnect);
  server.on("/netioProduct", handleNetioProduct);
  server.on("/wifi/redirect", HTTP_POST, handleWiFiPasswordRedirect);
  server.on("/wifi/check", handleWiFiApprove);
  server.on("/netioProduct/check", HTTP_POST, handleNetioDevice);
  server.on("/buttonConfigure", handleConfig);
  server.on("/buttonConfigure/check", HTTP_POST, handleConfigCheck);
  server.begin();
  Serial.println("AP has been turned on");
}

void setWiFiServer()
{
  APSet();
  delay(500);
  serversOn();
}

void handleServer()
{
  //if(APWork)
  server.handleClient();
}
