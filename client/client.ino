#include <SPI.h>
#include <stdlib.h>
#include <WiFlyHQ.h>
#include "LedController.h"

#define numLeds 34
#define numChannels 3
#define numFrames 2

WiFly wifly;
LedController controller;

int loadCounter = 0;
int animationArray[numFrames][numLeds][numChannels];

void setup()
{ 
  Serial1.begin(115200);
  Serial.begin(115200);
  // SETUP Leds
  controller.setupDisplayLedIndices();
  controller.setupLedControl();
  

  if (!wifly.begin(&Serial1, &Serial)) {
    Serial.println("Failed to start wifly");
  }   

  if (!wifly.isAssociated()) {
    joinNetwork();
  } 
  else {
    Serial.println("Already joined network");
  }
  //Serial.println("loadred");
  //controller.loadRed();
  
}


void loop()
{
  //delay(1000); 
  //Serial.print("data: ");
  /*
  if (!wifly.isAssociated()) {
    Serial.println("Reconnect");
    joinNetwork();
  }
  
  if (Serial1.available()) {
    Serial.println("available");
    Serial.print(Serial1.read());
  }
  */
  

  /////////////////////////
  // SERIAL READING
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('t');

    Serial.print("data: ");
    Serial.println(data);
    
  // FEHLER
    if (data.startsWith("l")) {
      parseLoad(data, loadCounter);
      loadCounter++;
    } 
    // OK
    else if (data.startsWith("s")) {
      data = data.substring(1);
      controller.loadFrame(animationArray, data.toInt());
    } 
    else if (data.startsWith("f")) {
      loadCounter = 0;
    }
    
  }
}

void parseLoad(String frame, int frameIndex) {
  frame = frame.substring(1);
    Serial.println(frameIndex);
  for (int ledIndex = 0; ledIndex < numLeds; ledIndex++) {
          Serial.print("led: ");
    Serial.println(ledIndex);
    for (int channelIndex = 0; channelIndex < numChannels; channelIndex++) {
      Serial.print("channel: ");
      Serial.println(channelIndex);
      int subIndex = ledIndex * 6 + channelIndex * 2;
      String stringValue = frame.substring(subIndex, subIndex + 2);
      char charString[2];
      stringValue.toCharArray(charString, 4);
      int val = strtoul(charString, NULL, 16);
      animationArray[frameIndex][ledIndex][channelIndex] = val;
      Serial.print("val: ");
      Serial.println(animationArray[frameIndex][ledIndex][channelIndex]);
    }
  }
}

void joinNetwork() {
  Serial.println("Joining network");
  wifly.setSSID("TrafoControl");
  wifly.setPassphrase("1234567890");
  wifly.enableDHCP();
  wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
  wifly.setDeviceID("TrafoControl");
  if (wifly.join()) {
    Serial.println("Joined wifi network");
  } 
  else {
    Serial.println("Failed to join wifi network");
  }
}







