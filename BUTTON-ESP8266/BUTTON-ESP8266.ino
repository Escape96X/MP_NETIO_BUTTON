#include <ESP8266WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);
const int button = 2;
int buttonState = 0;
bool cableState = false;
bool buttonReleased = false;

const char* MQTT_SERVER = "mqtts://connect.hexio.cloud";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "0aOAzhLy";
const char* MQTT_PASSWORD = "04XqHw2FMv";
const char* MQTT_ON_OFF_TOPIC = "PowerCable101E24a42c38d054/output/1/action";


void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

  
void wifiWPS(){
 // Nastavani WIFI a pripojeni WPS
  WiFi.mode(WIFI_STA);
  delay(1000);
  WiFi.begin("","");
  delay(4000);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting..");
    WiFi.beginWPSConfig();
    delay(3000);
      
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected!");
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.SSID());
    }
    else {
      Serial.println("Connection failed!");
    }
  }
}
bool MQTTConnect(){
  // Pokus o připojení do MQTT
  while (!client.connected()){
   Serial.println("Trying to connect MQTT");
   if(client.connect("ESP8266Client", MQTT_USERNAME, MQTT_PASSWORD)){
     Serial.println("Connected to MQTT");
   }
   else{
     Serial.print("Failed with state: ");
     Serial.println(client.state());
     delay(2000);
    }  
  }
  return true;    
}


void setup() {
  // Nastavení serveru a serialu
  wifiWPS();
  Serial.begin(115200);
  pinMode(button, OUTPUT);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  MQTTConnect();
  
}


void loop() {
  client.publish(MQTT_ON_OFF_TOPIC, "1");
  client.subscribe(MQTT_ON_OFF_TOPIC);
}
  
