#ifndef _basic_functions_h_
#define _basic_functions_h_
#include <WString.h>

void feedback_timer(int duration, int quantity);
void ESPSleep();
int dBmtoPercentage(int dBm);
String ipToString(IPAddress ip);

#endif