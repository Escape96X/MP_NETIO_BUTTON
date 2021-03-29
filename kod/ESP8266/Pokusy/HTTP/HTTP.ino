#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

IPAddress local_IP(192, 168, 0, 166);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

String HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";
String HTTP_CONNECTION = "http://192.168.0.196/netio.json";

HTTPClient http;
//#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */


void httpPost()
{
  if (http.begin(HTTP_CONNECTION))
  {
    int httpCode = http.POST(HTTP_REQUEST); // odeslani json requestu
    String payload = http.getString();      // zjisteni zpetne vazby
    //Serial.println(payload);
    http.end();
  }
}

void WiFib(){
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin("Jirickovi_secured", "19192020");
      for(int i = 0; i < 5; i++){
        delay(100);
        Serial.print(".");
        if(WiFi.status() == WL_CONNECTED)
          break;
      }
  }
  Serial.println("Connected to the WiFi network"); 
}


void setup(){
  Serial.begin(115200);
  Serial.println("START");
  WiFib();
  Serial.println("SEND BEGIN");
  httpPost();
  Serial.println("END");
  //esp_deep_sleep_start();
  ESP.deepSleep(0);
}

void loop(){
  //This is not going to be called
}
