#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 2    // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11

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

WiFiClient espClient;
PubSubClient client(espClient);

float readTemperature() {
  float Temp = dht.readTemperature();
  return Temp;
}



void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("NETPIE2020 connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
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
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  temperature = readTemperature();
  light = 100;
  String place = "NECTEC";

  String data = "{\"data\": {\"temperature\":" + String(temperature) + ", \"light\":" + String(light) + ", \"place\": \"" +  String(place) + "\"}}";
  Serial.println(data);
  data.toCharArray(msg, (data.length() + 1));
  client.publish("@shadow/data/update", msg);
  delay(2000);
}
