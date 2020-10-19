#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RELAY_PIN 3

const char *WIFI_SSID = "ssid";
const char *WIFI_PASSWORD = "passwd";
const char *MQTT_BROKER = "192.168.0.7";


WiFiClient esp;
PubSubClient client(esp);

void openRelay() {
  digitalWrite(RELAY_PIN,HIGH);
}
void closeRelay() {
  digitalWrite(RELAY_PIN,LOW);
}


//TODO Measure the coffee brew duration
//Function to control relays and keep the machine up for coffee preperation time.
void makeCoffee() { 
  openRelay();
  delay(60*1000);
  closeRelay();
}

//Sample code for connecting to the Internet with ESP. Added int return for examine the result. 
int connect2Internet(String ssid,String password){
  WiFi.begin(ssid,password);
  delay(2000);
  if(WiFi.status() != WL_CONNECTED){
      Serial.print("Not Connected after 2 seconds\n");
      Serial.print("Trying again\n");
      delay(2000);
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Connection to Wifi has failed.\n");
        return 0;
      }
  }
  return 1;
}



void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  char receivedChar = (char)payload[0];
  Serial.print(receivedChar);
  Serial.print("\n");
  if (receivedChar == '1'){
    Serial.print("\n...Making Coffee...");
    makeCoffee();
    Serial.print("\nCoffee is ready\n");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()){
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("coffemachine")) {
    Serial.println("connected");
    // ... and subscribe to topic
    client.subscribe("coffeeStatus");
  } 
  else {
    Serial.print("failed, rc=");
    Serial.print(client.state());
    Serial.println(" try again in 5 seconds");
    delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(3000);
  if (connect2Internet(WIFI_SSID,WIFI_PASSWORD) == 1){
    Serial.print("Connected succesfully\n");
    IPAddress ip;
    ip = WiFi.localIP();
    Serial.println(ip);
  }
  else{
    Serial.print("Not Connected\n");
  }
  pinMode(RELAY_PIN,OUTPUT);
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

