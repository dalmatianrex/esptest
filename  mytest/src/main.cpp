#include <Arduino.h>
#include <..\src\ESP32_defines.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

// An example of an esp32 program that opens a file called hello.txt using SPIFFS
// to upload the file create a folder called data at the same level as \src
// in the data folder add anew file called hello.txt and save some text in it
// using platformio Run “Upload File System image” task

struct Data_file
{
  float temp;
  int num_boots;
} data;

const char *fileName = "/data.json";

void load_data()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open(fileName);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  StaticJsonDocument<500> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error)
  {
    Serial.println("Failed to read file");
  }

  //load data
  data.temp = doc["temp"];
  data.num_boots = doc["num_boots"];

  file.close();
}
void save_data()
{
  File file = SPIFFS.open(fileName, "w");
  StaticJsonDocument<500> doc;

  doc["temp"] = data.temp;
  doc["num_boots"] = data.num_boots;

  if (serializeJson(doc, file) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }
  file.close();
}

void setup()
{
  Serial.begin(SERIAL_0_SPEED);
  load_data();
  data.num_boots++;
  Serial.printf("Num boots: %d \n", data.num_boots);

  save_data();
}

void loop()
{
}
