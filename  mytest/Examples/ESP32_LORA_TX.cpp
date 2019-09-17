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
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);

  if (!LoRa.begin(LORA_BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSyncWord(LORA_SYNC_WORD);
  Serial.println("init ok");
  delay(1500);
}

void loop() 
{
  Serial.printf("Sending LoRa message %d \n", counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("Hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  delay(5000);
}