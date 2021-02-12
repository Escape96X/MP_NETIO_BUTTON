

#include "WiFi.h"

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5   
/* Time ESP32 will go to sleep (in seconds) */
IPAddress local_IP(192, 168, 0, 184);

IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);
void setup(){
  Serial.begin(115200);
  WiFi.config(local_IP, gateway, subnet);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("Jirickovi_secured");
      for(int i = 0; i < 5; i++){
        delay(100);
        Serial.print(".");
        if(WiFi.status() == WL_CONNECTED)
          break;
      }
  }
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
 


  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
}

void loop(){
  //This is not going to be called
}
