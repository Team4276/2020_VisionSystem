#include "Bitcraze_PMW3901.h"

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow_1(10);
Bitcraze_PMW3901 flow_2(9);

void setup() {
  Serial.begin(9600);

  if (!flow_1.begin()) {
    Serial.println("Initialization of the flow 1 sensor failed");
    while(1) { }
  }
//  if (!flow_2.begin()) {
//    Serial.println("Initialization of the flow 2 sensor failed");
//    while(1) { }
//  }
}

int16_t deltaX1,deltaY1,deltaX2,deltaY2;

void loop() {
  // Get motion count since last call
  flow_1.readMotionCount(&deltaX1, &deltaY1);
  //flow_2.readMotionCount(&deltaX2, &deltaY2);

  Serial.print("X1: ");
  Serial.print(deltaX1);
  Serial.print(", Y1: ");
  Serial.print(deltaY1);
  Serial.print("   X2: ");
  Serial.print(deltaX2);
  Serial.print(", Y2: ");
  Serial.print(deltaY2);
  Serial.print("\n");

  delay(100);
}
