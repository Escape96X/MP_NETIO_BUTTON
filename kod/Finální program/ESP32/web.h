#ifndef _web_h_
#define _web_h_

#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

void setWiFiServer();
String readEEPROM(int numberOfStart, int len);
extern String ssid;
extern String password;
extern String NetioIP1;
extern String NetioIP2;
extern String HTTPRequest1;
extern String HTTPRequest2;

void handleServer();
void serversOn();
void feedback_timer(int duration, int quantity);
#endif
