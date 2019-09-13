#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include <unity.h>
#include <..\src\ESP32_defines.h>
#include <TinyGPS++.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_on_board_led(void)
{
    TEST_ASSERT_EQUAL(14, ON_BOARD_LED);
}

void test_led_state_high(void)
{
    digitalWrite(ON_BOARD_LED, HIGH);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(ON_BOARD_LED));
}

void test_led_state_low(void)
{
    digitalWrite(ON_BOARD_LED, LOW);
    TEST_ASSERT_EQUAL(LOW, digitalRead(ON_BOARD_LED));
}

void test_i2c_bus(void)
{
    std::vector<int> i2c_results;
    Wire.begin();
    for (byte i = 0; i < 50; i++)
    {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0)
        {
            i2c_results.push_back(i);
        }
    }
    int num_i2c_found = i2c_results.size();
    TEST_ASSERT_EQUAL(0, num_i2c_found);
}

void test_battery(void)
{
    int analogValue = analogRead(BATTERY_INPUT);
    float voltage = analogValue * (MAX_BATTERY_VOLTAGE / ANALOG_MAX_VALUE) * 2.0;
    TEST_ASSERT_TRUE(voltage > 0.0);
    TEST_ASSERT_TRUE(voltage < (MAX_BATTERY_VOLTAGE * 2.0));
}

void test_gps(void)
{
    TinyGPSPlus gps;
    double latitude = 0.0;
    double longitude = 0.0;
    int numSatellites = 0;
    Serial1.begin(SERIAL_1_SPEED, SERIAL_8N1, GPS_TX, GPS_RX);

    while (Serial1.available())
    {
        gps.encode(Serial1.read());
    }
    while(numSatellites == 0)
    {
        numSatellites = gps.satellites.value();
        delay(1000);
    }
    
    if (numSatellites > 0)
    {
        latitude = gps.location.lat();
        longitude = gps.location.lng();

        Serial.print("Satellites: ");
        Serial.println(numSatellites);
        Serial.print("Latitude  : ");
        Serial.println(latitude, 8);
        Serial.print("Longitude : ");
        Serial.println(longitude, 8);
        Serial.println("**********************");
    }
    
    
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    delay(2000);
    pinMode(ON_BOARD_LED, OUTPUT);

    UNITY_BEGIN(); // IMPORTANT LINE!

    RUN_TEST(test_on_board_led);
    RUN_TEST(test_led_state_high);
    RUN_TEST(test_led_state_low);
    RUN_TEST(test_i2c_bus);
    RUN_TEST(test_battery);
    RUN_TEST(test_gps);
    UNITY_END(); // stop unit testing
}

void loop()
{
}