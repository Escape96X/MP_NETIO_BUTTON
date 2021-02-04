#include <WiFi.h>
#include <HTTPClient.h>

IPAddress local_IP(192, 168, 0, 166);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

String HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";
String HTTP_CONNECTION = "http://192.168.0.196/netio.json";

HTTPClient http;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */


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
  Serial.println('\n');
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  if(WiFi.status() == WL_CONNECTED){
   Serial.println('connected');
  }else{
  WiFi.begin("Jirickovi_secured", "19192020");

  Serial.println("Connecting to ");
//  Serial.print(ssid); Serial.println(" ...");
 for(int i = 0; i < 5; i++){
    delay(500);
    Serial.println(WiFi.status());
    if(WiFi.status() == WL_CONNECT_FAILED){
      esp_deep_sleep_start();}
    if(WiFi.status() == WL_CONNECTED)
      break;
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
}}


void setup(){
  Serial.begin(115200);
    
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");
  WiFib();
  httpPost();
  Serial.println("Going to sleep now boot count");
  esp_deep_sleep_start();
}

void loop(){
  //This is not going to be called
}
