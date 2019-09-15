#include <Arduino.h>
#include <..\src\ESP32_defines.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;

double latitude = 0.0;
double longitude = 0.0;
int numSatellites = 0;

void setup()
{
  Serial.begin(SERIAL_0_SPEED);
  Serial1.begin(SERIAL_1_SPEED, SERIAL_8N1, GPS_TX, GPS_RX);
}

void loop()
{

  while (Serial1.available())
  {
    gps.encode(Serial1.read());
  }

  numSatellites = gps.satellites.value();

  if (numSatellites > 0)
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();

    Serial.printf("Satellites: %d \n", numSatellites);
    Serial.printf("Latitude  : %f \n", latitude);
    Serial.printf("Longitude : %f \n", longitude);
    Serial.println("**********************");
  }
  
  delay(1000); 
}
