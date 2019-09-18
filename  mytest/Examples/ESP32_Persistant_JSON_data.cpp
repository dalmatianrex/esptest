/*
  ESP32 demonstration code - Dr Paul Lunn
  data stored persistantly within a JSOn file in flash memory
*/

#include <Arduino.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <..\src\ESP32_defines.h>

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
StaticJsonDocument<500> json_data;

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

  DeserializationError error = deserializeJson(json_data, file);
  if (error)
  {
    Serial.println("Failed to read file");
  }

  //load data
  data.temp = json_data["temp"];
  data.num_boots = json_data["num_boots"];

  file.close();
}

void save_data()
{
  File file = SPIFFS.open(fileName, "w");
  
  json_data["temp"] = data.temp;
  json_data["num_boots"] = data.num_boots;

  if (serializeJson(json_data, file) == 0)
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
