#include <Arduino.h>
#include <ESP8266WiFi.h>


#define RELAY_PIN 3



void setup() {
  connect2Internet("somenetwork","somepassword");
  pinMode(RELAY_PIN,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
}

int connect2Internet(String ssid,String password){
  WiFiClient client;
  WiFi.begin(ssid,password);
  delay(2000);
  if(WiFi.status() != WL_CONNECTED){
      Serial.print("Not Connected after 2 seconds");
      Serial.print("Trying again");
      delay(2000);
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Not Connected");
        return 0;
      }
  }
  return 1;
}

void makeCoffee() { 
  openRelay();
  delay(60*1000);
  closeRelay();
}

// Function to Open Coffee Machine
void  openRelay() {
  digitalWrite(RELAY_PIN,HIGH);
}

void closeRelay() {
  digitalWrite(RELAY_PIN,LOW);
}