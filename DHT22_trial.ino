// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

//#include <Adafruit_Sensor.h>
//#include <DHT.h>
//#include <DHT_U.h>

#include "DHTesp.h"

#define DHTPIN 4
// Digital pin connected to the DHT sensor 
//#define DHTPIN 2     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define LEDPIN 2
#define HEATERPIN 12

// Uncomment the type of sensor in use:
//efine DHTTYPE    DHT11     // DHT 11
//efine DHTTYPE    DHT21     // DHT 21 (AM2301)
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

// DHT_Unified dht(DHTPIN, DHTTYPE);

DHTesp dht;

uint32_t delayMS;
float ThresholdHigh = 21;
float ThresholdLow  = 20;
int   HeaterPad = LOW;

void setup() {
  Serial.begin(115200);

  // Initialize temperature sensor
  dht.setup(DHTPIN, DHTesp::DHT22);
  Serial.println("DHT initiated");

  delayMS=2000;

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LEDPIN, OUTPUT);

  // Initialize HeaterPad OFF
  pinMode(HEATERPIN,OUTPUT);
  digitalWrite(HEATERPIN,LOW);

  // Get temperature event and print its value.
  TempAndHumidity newValues = dht.getTempAndHumidity();
  ThresholdHigh = newValues.temperature + 2;
  ThresholdLow  = newValues.temperature + 1.9;

}

void loop() {

  // Delay between measurements.
  delay(delayMS);
  BlinkLedCode(LEDPIN,200,200);
  
  // Get temperature event and print its value.
  TempAndHumidity newValues = dht.getTempAndHumidity();

  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return;
  }

  if (newValues.temperature > ThresholdHigh)
  {
    // Turn OFF the heater pad
    HeaterPad = LOW;
  }
  
  if (newValues.temperature < ThresholdLow)
  {
    // Turn ON the heater pad
    HeaterPad = HIGH;
  }

  if (HeaterPad == LOW)
  {
    pinMode(HEATERPIN,OUTPUT);  
    digitalWrite(HEATERPIN,LOW);
  }
  else
  {
    //digitalWrite(HEATERPIN,HIGH);
    //delay(10);
    pinMode(HEATERPIN,INPUT);  
  }

  //LED
  digitalWrite(LEDPIN,HeaterPad);

  Serial.print(F("Temperature: "));
  Serial.print(newValues.temperature);
  Serial.print(F("°C     "));
  Serial.print((newValues.temperature*9)/5 + 32);
  Serial.print(F("°F"));

  Serial.print(F("     Humidity: "));
  Serial.print(newValues.humidity);
  Serial.print(F("%"));

  Serial.print(F("     Heater Pad: "));
  Serial.println(F(HeaterPad == HIGH)?"ON":"OFF");

}

void BlinkLedCode(int NumBlinks, int HighTimeMs, int LowTimeMs)
{
    for (int i = 0; i < NumBlinks; i++)
    {
        digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(HighTimeMs);
        digitalWrite(LEDPIN, LOW);   // turn the LED on (HIGH is the voltage level)
        delay(LowTimeMs);
    }
}
