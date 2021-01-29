#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "web.h"

extern String ssid;
extern String password;
extern String NetioIP;
extern String HTTPRequest1;

// konstanty pripojeni a json zpravy
//char* HTTP_REQUEST1 = "{"Outputs": [{"ID": 1,"Action": 4}]}";

//const int buttton = 16;
const int buzzer = 13;
const int LED = 12;
const int button = 2;
// promenne
int buttonState = 0;
int buttonState2 = 0;
bool checkButtonState1 = true;
bool checkButtonSteate2 = true;

HTTPClient http;

void WiFiConnect()
{
  //if(WiFi.status() != WL_CONNECTED)
  Serial.println("WIfiIII");
  setWiFiServer();
}

void buzzerTimer(int duration)
{
  // Bzucak na oznameni stavu
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(buzzer, LOW);
  delay(100);
}

void LEDTimer(int duration)
{
  digitalWrite(LED, LOW);
  delay(duration);
  digitalWrite(LED, HIGH);
  delay(100);
}

void httpPost()
{
  // odesilani requestu
  String HTTP_CONNECTION = "http://";
  HTTP_CONNECTION += NetioIP;
  HTTP_CONNECTION += "/netio.json";
  if (http.begin(HTTP_CONNECTION))
  {
    http.addHeader("Content-Type", "text/plain");
    Serial.println(HTTPRequest1);           // nastaveni headeru na klasicky test
    int httpCode = http.POST(HTTPRequest1); // odeslani json requestu
    String payload = http.getString();      // zjisteni zpetne vazby

    if (payload.indexOf("errors") > 0)
    {
      for (int i = 0; i < 2; i++)
      {
        buzzerTimer(200); // json byl odeslan ve spatnem formatu, 2 kratke zabzuceni
        LEDTimer(1000);
      }
    }
    Serial.println(payload);
    http.end();
    LEDTimer(200);
  }
  else
  {
    for (int i = 0; i < 2; i++)
    {
      buzzerTimer(200); // ESP se nepripojilo k zasuvce
    }
  }
}

void wifiCheck()
{
  // kontrola zda v dobe requestu jsme pripojeni
  if (WiFi.status() == WL_CONNECTED)
  {
    httpPost();
  }
  else
  {
    for (int i = 0; i < 3; i++)
    {
      buzzerTimer(300);
      LEDTimer(300);
      Serial.println("nepripojeni");
    }
  }
}

void buttonCheck()
{
  // aby pri delsim podrzenim stav nepreskakoval
  buttonState = digitalRead(button);
  if (buttonState == HIGH)
  {
    checkButtonState1 = true;
  }
}

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(512);
  delay(2000);
  pinMode(buzzer, OUTPUT); // nastaveni bzucaku na out
  pinMode(button, INPUT);
  pinMode(LED, OUTPUT);
  WiFiConnect();
  wifiCheck();
  NetioIP = readEEPROM(0, 15);
  HTTPRequest1 = readEEPROM(60, 50);
}

void loop()
{
  handleServer();
  buttonState = digitalRead(button); // cteni stavu tlacitka
  if (buttonState == LOW && checkButtonState1)
  {
    Serial.println(readEEPROM(0, 15));
    Serial.println(readEEPROM(60, 50));
    wifiCheck();               // kontrola wifi zda jsme stale pripojeni
    checkButtonState1 = false; // promenna pro kontrolu dlouheho mackani tlacitka
  }
  //Serial.println(checkButtonState1);
  buttonCheck(); // zmeneni stavu promenne check
  // Serial.println(NetioIP);
  // Serial.println(HTTPRequest1);
}
