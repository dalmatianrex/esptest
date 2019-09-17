/*
  ESP32 demonstration code - Dr Paul Lunn

*/

#include <Arduino.h>
#include <..\src\ESP32_defines.h>
#include <WiFi.h>

String WIFI_NAME = ""; // Your Wifi name
String WIFI_PASSWORD = "";    // Your Wifi password
WiFiServer server(80);   

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
}

void loop(){}
