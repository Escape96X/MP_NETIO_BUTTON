#ifndef _web_h_
#define _web_h_

#include <Arduino.h>
#include <ESP8266WebServer.h>

void setWiFiServer();
String readEEPROM(int numberOfStart, int len);
extern String ssid;
extern String password;
extern String NetioIP;
extern String HTTPRequest1;
void handleServer();
void serversOn();
#endif
