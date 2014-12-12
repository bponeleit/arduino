// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <JeeLib.h>
#include <XBee.h>

ISR(WDT_vect) { 
  Sleepy::watchdogEvent(); 
}

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define NODE_ID 1

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);
XBee xbee = XBee();

uint8_t payload[] = { 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, }; 

XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x40acb044);
ZBTxRequest zbTx = ZBTxRequest();//addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider

int statusLed = 13;
int errorLed = 13;

float h;
float t;

void flashLed(int pin, int times, int wait) {
  //Serial.println("flash led");
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}

void setup() {
  Serial.begin(9600); 

  dht.begin();
  xbee.setSerial(Serial);

  zbTx.setAddress64(addr64);
}

void loop() {  

  readDHT();
  readPhotocell();

  char nodebuf[7];
  itoa(NODE_ID, nodebuf, 10);

  char tbuf[7];
  dtostrf(t, 6, 2, tbuf);

  char hbuf[7];
  dtostrf(h, 6, 2, hbuf);

  char photobuf[7];
  itoa(photocellReading, photobuf, 10);

  char strbuf[28];
  strcpy(strbuf, nodebuf);
  strcat(strbuf, tbuf);
  strcat(strbuf, hbuf);
  strcat(strbuf, photobuf);

  uint8_t data[strlen(strbuf)];
  memcpy(data, strbuf, strlen(strbuf));
  

  zbTx.setPayload(data);
  zbTx.setPayloadLength(sizeof(data));
  xbee.send(zbTx);
  
  flashLed(statusLed, 1, 100);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(1000)) {
    // got a response!

    // should be a znet tx status            	
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 5, 200);
        Serial.print("success");
      } 
      else {
        // the remote XBee did not receive our packet. is it powered on?
        Serial.print("Delivery failed");
        flashLed(errorLed, 3, 200);
      }
    }
  } 
  else if (xbee.getResponse().isError()) {
    //Serial.print("Analog reading = ");
    Serial.print("Error reading packet.  Error code: ");  
    Serial.println(xbee.getResponse().getErrorCode());
  } 
  else {
    // local XBee did not provide a timely TX Status Response -- should not happen
    Serial.print("XBee no response");
    Serial.print(xbee.getResponse().getApiId());
    flashLed(errorLed, 2, 200);
  }
  
    //Every 30 Seconds  
  Sleepy::loseSomeTime(30000);

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


