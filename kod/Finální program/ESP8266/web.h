#ifndef _web_h_
#define _web_h_

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>


#define IP_POS1 0
#define IP_POS2 15
#define HTTP_POS1 30
#define HTTP_POS2 130
#define SSID_POS 230
#define PASSWORD_POS 294

void setWiFiServer();
void setWiFiServer2();
String readEEPROM(int numberOfStart, int len);

void handleServer();
void serversOn();
void feedback_timer(int duration, int quantity);
#endif
