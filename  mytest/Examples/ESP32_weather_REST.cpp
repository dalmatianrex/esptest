/*
  ESP32 demonstration code - Dr Paul Lunn

*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <..\src\ESP32_defines.h>
#include <WiFi.h>
#include <HTTPClient.h>

String WIFI_NAME = "";        // Your Wifi name
String WIFI_PASSWORD = ""; // Your Wifi password
WiFiServer server(80);

String API_BASE = "http://api.openweathermap.org/data/2.5/weather?";
String API_LAT_LONG = "lat=35&lon=139";
String API_KEY = "&APPID=";

String URL = API_BASE + API_LAT_LONG + API_KEY;

DynamicJsonDocument doc(3000);

void setup()
{
  Serial.begin(SERIAL_0_SPEED);

  // Let's connect to wifi network
  Serial.printf("Connecting to %s \n", WIFI_NAME.c_str());

  //Connecting to wifi network
  WiFi.begin(WIFI_NAME.c_str(), WIFI_PASSWORD.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    //Waiting for the responce of wifi network
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connection Successful!");

  HTTPClient http;
  http.begin(URL);
  int httpCode = http.GET(); //Make the request

  if (httpCode > 0)
  {
    //Check for the returning code
    String json = http.getString();

    DeserializationError err = deserializeJson(doc, json.c_str());

    if (err)
    {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
    }

    Serial.println(json.c_str());
  }
  else
  {
    Serial.println("Error on HTTP request");
  }

  http.end(); //Free the resources
}

void loop() {}
