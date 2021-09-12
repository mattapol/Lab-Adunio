
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

char* ssid = "AP";
const char* password = "1234567890";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "4c1e7409-ba71-4c85-bb22-e33c0ebe1db6";
const char* mqtt_username = "KRVNioqgbGRZVEwHqH3z5VdPxVQHT2Us";
const char* mqtt_password = "qK(3PpW1U~czp35l4tZ!bDPWN5qaPMG2";

WiFiClient espClient;
PubSubClient client(espClient);

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
  Serial.begin(115200);
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

  client.publish("@msg/test", "Hello Sunnn");
  Serial.println("Hello NETPIE2020");
  delay(2000);
}
