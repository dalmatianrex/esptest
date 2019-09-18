/*
  ESP32 demonstration code - Dr Paul Lunn
  code for communicating with MQTT server
*/
#include <Arduino.h> 
#include <WiFi.h>
#include <PubSubClient.h>

#define SERIAL_0_SPEED  115200

const char *ssid = "";                 // name of your WiFi network
const char *password = "";          // password of the WiFi network
const char *mqtt_server = "test.mosquitto.org"; /// MQTT Broker
int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client"))
    {

      Serial.println("connected");
      // client.subscribe("event");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(SERIAL_0_SPEED );
  // Start up the library

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Serial.println("Connected ");
  Serial.print("MQTT Server ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.println(String(mqtt_port));
  Serial.print("ESP32 IP ");
  Serial.println(WiFi.localIP());
  Serial.println("Modbus RTU Master Online");
}

void loop()
{
  char temperaturenow [15] = "hello         ";
  client.publish("lunn/temperaturel/sensor", temperaturenow ); /// send char

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  delay(10000);
}