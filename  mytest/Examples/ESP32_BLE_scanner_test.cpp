#include <Arduino.h>
#include <..\src\ESP32_defines.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define SCAN_TIME_SECONDS 10

void setup()
{

  Serial.begin(SERIAL_0_SPEED);
  Serial.println("ESP32 BLE Scanner Test");

  BLEDevice::init("");
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(0x50);
  pBLEScan->setWindow(0x30);

  Serial.printf("Start BLE scan for %d seconds \n", SCAN_TIME_SECONDS);

  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME_SECONDS);

  int numBLEDevicesFound = foundDevices.getCount();
  Serial.printf("Found %d BLE devices...\n", numBLEDevicesFound);

  for (int i = 0; i < numBLEDevicesFound; i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    String address = device.getAddress().toString().c_str();

    int rssi = device.getRSSI();
    String name = device.getName().c_str();
    int appearance = device.getAppearance();

    String manufacturerData;
    if (device.haveManufacturerData())
    {
      manufacturerData = device.getManufacturerData().c_str();
    }
    else
    {
      manufacturerData = "No Manu Data";
    }

    String serviceUUID = "NO UUID";
    if (device.haveServiceUUID())
    {
      serviceUUID = device.getServiceUUID().toString().c_str();
    }

    int tx = 0;
    if (device.haveTXPower())
    {
      tx = device.getTXPower();
    }

    Serial.println("*****************************************");
    Serial.printf("Name: %s \n", name.c_str());
    Serial.printf("Address: %s \n", address.c_str());
    Serial.printf("RSSI: %d \n", rssi);
    Serial.printf("TX Power: %d \n", tx);
    Serial.printf("Appearance: %d \n", appearance);
    Serial.printf("Manu Data: %s \n", manufacturerData.c_str());
    Serial.printf("UUID: %s \n", serviceUUID.c_str());
  }

  Serial.println("Scan done!");
}

void loop() {}

