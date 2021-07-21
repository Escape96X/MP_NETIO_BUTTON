#ifndef _web_h_
#define _web_h_

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// delky jsou vcetne 1 mezery pro ukonceni zapisu
#define IP_POSA 0
#define IP_POSB 188
#define IP_LEN 187

// PREDELAT
#define HTTP_POS1 400
#define HTTP_LEN 100
#define HTTP_POS2 501
#define SSID_POS 602
#define SSID_LEN 33
#define PASSWORD_POS 636
#define PASSWORD_LEN 100

void setWiFiServer();

void setWiFiServer2();

String readEEPROM(int numberOfStart, int len);

String readIP(int i, int offset);

int countIP(int offset);


void handleServer();

void serversOn();

void feedback_timer(int duration, int quantity);

#endif
