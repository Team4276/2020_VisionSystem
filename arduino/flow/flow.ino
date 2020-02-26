#include "Bitcraze_PMW3901.h"
#include <Wire.h>
#include <WireData.h>


// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow_1(10);
Bitcraze_PMW3901 flow_2(9);


#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE);               
  Wire.onRequest(requestEvent); // register event
  //if (!flow_1.begin()) {
  //  Serial.println("Initialization of the 1 flow sensor failed");
  //  while (1) { }
  //}
  if (!flow_2.begin()) {
    Serial.println("Initialization of the 2 flow sensor failed");
    while (1) { }
  }
}



int16_t deltaX1, deltaY1, deltaX2, deltaY2;
const uint16_t syncPattern1 = 0xFEED;
const uint16_t syncPattern2 = 0xFACE;

void loop() {
  // Nothing to do - all the work is done in the Wire data handlers (onReceive, onRequest).
  delay(100);
  flow_1.readMotionCount(&deltaX1, &deltaY1);
  flow_2.readMotionCount(&deltaX2, &deltaY2);

  String msg = "X1: ";
  msg += deltaX1;
  msg += " Y1: ";
  msg += deltaY1;
  msg += "     X2: ";
  msg += deltaX2;
  msg += " Y2: ";
  msg += deltaY2;
  Serial.println(msg);
  
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent()
{
  // Get motion count since last call
  flow_1.readMotionCount(&deltaX1, &deltaY1);
  flow_2.readMotionCount(&deltaX2, &deltaY2);

  wireWriteData(syncPattern1);
  wireWriteData(syncPattern2);
  wireWriteData(deltaX1);
  wireWriteData(deltaY1);
  wireWriteData(deltaX2);
  wireWriteData(deltaY2);  // 12 bytes total
}
