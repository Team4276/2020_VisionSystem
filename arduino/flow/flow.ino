#include "Bitcraze_PMW3901.h"
#include <Wire.h>
#include <WireData.h>


int csPinFlow_1 = 10;
int csPinFlow_2 = 9;
Bitcraze_PMW3901 flow_1(csPinFlow_1);
Bitcraze_PMW3901 flow_2(csPinFlow_2);

#define SDA_PIN 4
#define SCL_PIN 5
const int16_t I2C_MASTER = 0x42;
const int16_t I2C_SLAVE = 0x08;


int16_t deltaX1, deltaY1, deltaX2, deltaY2;
int8_t isFlowOK_1 = 0;  // false
int8_t isFlowOK_2 = 0;
int retryCount_1 = 0;
int retryCount_2 = 0;

const uint16_t syncPattern1 = 0xFEED;
const uint16_t syncPattern2 = 0xFACE;


void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE);               
  Wire.onRequest(requestEvent); // register event
  digitalWrite(csPinFlow_2, LOW); 
  digitalWrite(csPinFlow_2, HIGH);  // Make sure flow_2 is NOT seleced
  if (flow_1.begin()) {
    isFlowOK_1 = 1;
  }
  else
  {
    Serial.println("Initialization of the 1 flow sensor failed");
  }
  digitalWrite(csPinFlow_1, LOW); 
  digitalWrite(csPinFlow_1, HIGH);  // Make sure flow_1 is NOT seleced
  if (flow_2.begin()) {
    isFlowOK_2 = 1;
  }
  else
  {
    Serial.println("Initialization of the 2 flow sensor failed");
  }
}

void readFlowSensors()
{
  String msg;
  if(isFlowOK_1 != 0)
  {
    digitalWrite(csPinFlow_2, LOW); 
    digitalWrite(csPinFlow_2, HIGH);  // Make sure flow_2 is NOT seleced
    flow_1.readMotionCount(&deltaX1, &deltaY1);
    msg += "X1: ";
    msg += deltaX1;
    msg += " Y1: ";
    msg += deltaY1;    
  }
  else
  {
    msg += "*flow_1 not valid*";
    if(retryCount_1 < 10)
    {
      retryCount_1++;
    }
    else
    {
      retryCount_1 = 0;
      if (flow_1.begin()) {
        isFlowOK_1 = 1;
      }
      else
      {
        isFlowOK_1 = 0;
        Serial.println("flow_1 init failed");
      }
    }
  }
  msg += "   ";
  if(isFlowOK_2 != 0)
  {
    digitalWrite(csPinFlow_1, LOW); 
    digitalWrite(csPinFlow_1, HIGH);  // Make sure flow_1 is NOT seleced
    flow_2.readMotionCount(&deltaX2, &deltaY2);
    msg += "X2: ";
    msg += deltaX2;
    msg += " Y2: ";
    msg += deltaY2;    
  }
  else
  {
    msg += "*flow_2 not valid*";
    if(retryCount_2 < 10)
    {
      retryCount_2++;
    }
    else
    {
      retryCount_2 = 0;
      if (flow_2.begin()) {
        isFlowOK_2 = 1;
      }
      else
      {
        isFlowOK_2 = 0;
        Serial.println("flow_2 init failed");
      }
    }
  }
  Serial.println(msg);
}

void loop() {
  // Nothing to do - all the work is done in the Wire data handlers (onReceive, onRequest).
  delay(100);
  //readFlowSensors();
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void requestEvent()
{
  // Get motion count since last call
  readFlowSensors();

  wireWriteData(syncPattern1);
  wireWriteData(syncPattern2);
  wireWriteData(isFlowOK_1);
  wireWriteData(isFlowOK_2);
  wireWriteData(deltaX1);
  wireWriteData(deltaY1);
  wireWriteData(deltaX2);
  wireWriteData(deltaY2);  // 14 bytes total
}
