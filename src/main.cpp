#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RELAY_PIN 3

const char *WIFI_SSID = "Kablonet Netmaster-22EB-A";
const char *WIFI_PASSWORD = "281bbfa2";
const char *MQTT_BROKER = "192.168.0.7";


WiFiClient esp;
PubSubClient client(esp);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char receivedChar = (char)payload[0];
  Serial.print(receivedChar);
  if (receivedChar == '1'){
    makeCoffee();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()){
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect("ESP8266 Client")) {
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
  connect2Internet("somenetwork","somepassword");
  pinMode(RELAY_PIN,OUTPUT);
  Serial.begin(9600);
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}


//Sample code for connecting to the Internet with ESP. Added int return for examine the result. 
int connect2Internet(String ssid,String password){
  WiFi.begin(ssid,password);
  delay(2000);
  if(WiFi.status() != WL_CONNECTED){
      Serial.print("Not Connected after 2 seconds");
      Serial.print("Trying again");
      delay(2000);
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Connection to Wifi has failed.");
        return 0;
      }
  }
  return 1;
}

//TODO Measure the coffee brew duration
//Function to control relays and keep the machine up for coffee preperation time.
void makeCoffee() { 
  openRelay();
  delay(60*1000);
  closeRelay();
}

// Function to open or close relays
void openRelay() {
  digitalWrite(RELAY_PIN,HIGH);
}
void closeRelay() {
  digitalWrite(RELAY_PIN,LOW);
}