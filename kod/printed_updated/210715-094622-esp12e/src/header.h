#ifndef _header_h_
#define _header_h_

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>


void setWiFiServer();

void setWiFiServer2();

void ESPSleep();

// memory.cpp

String readEEPROM(int numberOfStart, int len);

void saveToEEPROM(String sTosave, int startPosition, int len);

String readIP(int i, int offset);

void saveToEEPROMIP(String sToSave, int startPosition);

void deleteIP(int i, int offset);

int countIP(int offset);

// json.cpp

String jsonOfNetworks();

String jsonOfIP();

// web.cpp

void handleServer();

void serversOn();

void feedback_timer(int duration, int quantity);

int dBmtoPercentage(int dBm);

String ipToString(IPAddress ip);

#endif
