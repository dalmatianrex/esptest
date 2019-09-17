/*
  ESP32 demonstration code - Dr Paul Lunn

*/

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <..\src\ESP32_defines.h>

unsigned int counter = 0;
 
void setup() 
{
  Serial.begin(SERIAL_0_SPEED);
  Serial.println("LoRa Receiver Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);

  if (!LoRa.begin(LORA_BAND)) 
  {
    Serial.println("LoRa failed!");
    while (1);
  }
  LoRa.receive();
  LoRa.setSyncWord(LORA_SYNC_WORD);
  Serial.println("LoRa Init ok");
}

void loop() 
{
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet: ");

    // read packet
    while (LoRa.available()) 
    {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
    }
  }
  
}