// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <JeeLib.h>

ISR(WDT_vect) { 
  Sleepy::watchdogEvent(); 
}

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

float h;
float t;

void setup() {
  Serial.begin(9600); 
  //Serial.println("DHTxx test!");

  dht.begin();
}

void loop() {
  readDHT();
  readPhotocell();
  Serial.print(t);
  Serial.print(h);
  Serial.print(photocellReading);

  //Every 3 Seconds  
  Sleepy::loseSomeTime(3000);
}

void readDHT() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  t = dht.readTemperature();
}

void readPhotocell() {
  photocellReading = analogRead(photocellPin);  
}

