/*
  ESP32 demonstration code - Dr Paul Lunn

*/

#include <Arduino.h>
#include <..\src\ESP32_defines.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// https://www.uuidgenerator.net/

#define SERVICE_UUID "998bb714-eff9-4ec1-a33b-e675ea7c2d66"
#define CHARACTERISTIC_UUID_RX "3536c4bb-6fd5-49d1-8154-ce71d462c294"
#define CHARACTERISTIC_UUID_TX "f03ef870-4eb4-417d-9767-1459627b1063"

BLECharacteristic *pCharacteristicRX;
BLECharacteristic *pCharacteristicTX;
bool connected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    connected = true;
    Serial.printf("Connected to BLE Server \n");
  }

  void onDisconnect(BLEServer *pServer)
  {
    connected = false;
    Serial.printf("Disconnected from BLE Server \n");
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string receivedData = pCharacteristic->getValue();

    if (receivedData.length() > 0)
    {
      Serial.print("Received Value: ");

      for (int i = 0; i < receivedData.length(); i++)
      {
        Serial.print(receivedData[i]);
      }
      Serial.printf("\n");
    }
  }
};

void setup()
{
  //set up serial link to console
  Serial.begin(SERIAL_0_SPEED);

  // Create a BLE Device
  BLEDevice::init("ESP32 BLE Transmitter Test");

  // Create a Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE TX Characteristic
  pCharacteristicTX = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  pCharacteristicTX->addDescriptor(new BLE2902());

  // Create a BLE RX Characteristic
  pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

  pCharacteristicRX->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();
  pServer->getAdvertising()->start();

  Serial.println("Waiting for a BLE client connection");
}

void loop()
{
  if (connected)
  {
    int batteryLevel = analogRead(BATTERY_INPUT); 

    char txString[8];                      
    dtostrf(batteryLevel, 1, 2, txString); 

    pCharacteristicTX->setValue(txString); 
    pCharacteristicTX->notify(); 
    Serial.printf("Sending Battery level: %s \n", txString);
  }
  delay(1000);
}
