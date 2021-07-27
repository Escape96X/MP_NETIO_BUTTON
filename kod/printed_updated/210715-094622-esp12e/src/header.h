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

void saveToEEPROMContent(String sToSave, int startPosition, int jmp);

int countContent(int offset, int posB, int jmp);

String readContent(int i, int offset, int posB, int jmp);

void deleteContent(int i, int offset, int posB, int jmp);



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
