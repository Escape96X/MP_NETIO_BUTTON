#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
String HTTP_REQUEST = "{\"Outputs\": [{\"ID\": 1,\"Action\": 4}]}";
String HTTP_CONNECTION = "http://192.168.0.196/netio.json";
const char* ssid = "Jirickovi_secured";
const char* password = "19192020";

HTTPClient http;
const int buzzer = 13;
//const int LED = 12;
const int wakePin = 5;
IPAddress local_IP(192, 168, 0, 166);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);



void buzzerTimer(int duration){ 
  // Bzucak na oznameni stavu
  digitalWrite(buzzer, HIGH);
  delay(duration);
  digitalWrite(buzzer, LOW);
  delay(100);
}

  
void WiFib(){
  Serial.println('\n');
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(++i); Serial.print(' ');
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected");
  }else{
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(++i); Serial.print(' ');
  }
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
}}

void httpPost()
{
  // odesilani requestu
  if (http.begin(HTTP_CONNECTION))
  {
    http.addHeader("Content-Type", "text/plain");
    Serial.println(HTTP_REQUEST);           // nastaveni headeru na klasicky test
    int httpCode = http.POST(HTTP_REQUEST); // odeslani json requestu
    String payload = http.getString();      // zjisteni zpetne vazby

    if (payload.indexOf("errors") > 0)
    {
      for (int i = 0; i < 2; i++)
      {
        buzzerTimer(200); // json byl odeslan ve spatnem formatu, 2 kratke zabzuceni
        //LEDTimer(1000);
      }
    }
    Serial.println(payload);
    http.end();
    //LEDTimer(200);
  }
  else
  {
    for (int i = 0; i < 2; i++)
    {
      buzzerTimer(200); // ESP se nepripojilo k zasuvce
    }
  }
}

void setup() {
  pinMode(wakePin, OUTPUT);
  digitalWrite(wakePin, LOW);
  Serial.begin(115200);
  delay(10);
  WiFib();
  httpPost();
  digitalWrite(wakePin, HIGH);
}

void loop() { }
