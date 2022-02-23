#ifndef _web_h_
#define _web_h_

#include <WString.h>
#include <IPAddress.h>

void handleServer();

void serversOn();

void feedback_timer(int duration, int quantity);

int dBmtoPercentage(int dBm);

String ipToString(IPAddress ip);

void setWiFiServer();


#endif
