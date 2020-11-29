#ifndef _web_h_
#define _web_h_


#include <Arduino.h>
#include <ESP8266WebServer.h>

//extern ESP8266WebServer server(80);
//bool APWork;

void setWiFiServer();
String writeSAVED();

extern String ssid;
extern String password;
extern String NetioIP;

void handleServer();
void serversOn();
#endif
