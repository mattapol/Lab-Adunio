#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 0    // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define LM73_ADDR 0x4D
#define LED 12

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "AP";
const char* password = "1234567890";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "4c1e7409-ba71-4c85-bb22-e33c0ebe1db6";
const char* mqtt_username = "KRVNioqgbGRZVEwHqH3z5VdPxVQHT2Us";
const char* mqtt_password = "qK(3PpW1U~czp35l4tZ!bDPWN5qaPMG2";

int temperature = 0;
int light = 0;
int analog_value = 0;
char msg[100];
long lastMsg = 0;

WiFiClient espClient;
PubSubClient client(espClient);

float readTemperature() {
  float Temp = dht.readTemperature();
  return Temp;
}

float readlight() {
  analog_value = analogRead(36);
  if (analog_value > 1000) {
    analog_value = 1000;
  }
  int light = (1000 - analog_value) * 100 / 1000;
  return light;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("NETPIE2020 connected");
      client.subscribe("@msg/led");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);

  if(String(topic) == "@msg/led") {
    if (message == "on"){
      digitalWrite(LED,0);
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"on\"}}");
      Serial.println("LED ON");
    }
    else if (message == "off") {
      digitalWrite(LED,1);
      client.publish("@shadow/data/update", "{\"data\" : {\"led\" : \"off\"}}");
      Serial.println("LED OFF");
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED,OUTPUT);
  digitalWrite(LED,1);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  temperature = readTemperature();
  light = readlight();
  String place = "NECTEC";
  
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    String data = "{\"data\": {\"temperature\":" + String(temperature) + ", \"light\":" + String(light) + ", \"place\": \"" +  String(place) + "\"}}";
    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);
  }
  delay(1); 
}
