#include <SPI.h>
#include <WiFlyHQ.h>
#include "LedController.h"
#include <stdlib.h>

#define numLeds 34
#define numChannels 3
#define maxNumFrames 100
#define numFrames 2

WiFly wifly;
LedController controller;

int loadCounter = 0;
int animationArray[numFrames][numLeds][numChannels];

void setup()
{ 
  // SETUP Leds
  controller.setupDisplayLedIndices();
  controller.setupLedControl();

  // SETUP WiFLy
  Serial1.begin(115200);
  wifly.setupForUDP<HardwareSerial>(
  &Serial1,
  115200, // baudrate
  true, // scan for baudrates
  "TrafoControl", // SSID
  "1234567890", // password
  "TrafoClient", // Device name
  0, // using 0 enables DHCP
  997, // port
  "255.255.255.255", // send to all hosts in the subnet
  8757,  // outgoing port (not needed)
  true	// show debug information on Serial
  );
}


void loop()
{

  /////////////////////////
  // SERIAL READING
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('t');
    
    Serial.print("data: ");
    Serial.println(data);

    if (data.startsWith("l")) {
      parseLoadHexa(data, loadCounter);
      loadCounter++;
    } else if (data.startsWith("s")) {
      data = data.substring(1);
      controller.loadFrame(animationArray, data.toInt());
    } else if (data.startsWith("f")) {
      /*
      for (int i = 0; i < numFrames; i++) {
        for (int k = 0; k < numLeds; k++) {
          for (int j = 0; j < numChannels; j++) {
            Serial.print(animationArray[i][k][j]);
            Serial.println(", ");
          }
        }
      }
      */
      loadCounter = 0;
    }
  }
}

/*
void parseLoad(String frame, int frameIndex) {
  int endFramePos;
  int endChannelPos;

  frame = frame.substring(1);
  //Serial.println(frame);

  for (int ledIndex = 0; ledIndex < numLeds; ledIndex++) {
    endFramePos = frame.indexOf('c');
    if (endFramePos != -1) {
      String led = frame.substring(0, endFramePos);
      for (int channelIndex = 0; channelIndex < numChannels; channelIndex++) {
        endChannelPos = led.indexOf(',');
        if (endChannelPos != -1) {
          animationArray[frameIndex][ledIndex][channelIndex] = led.substring(0, endChannelPos).toInt();
        }
        led = led.substring(endChannelPos + 1, led.length());
      }
      frame = frame.substring(endFramePos + 1, frame.length());
    }
  }
}
*/
void parseLoadHexa(String frame, int frameIndex) {
  frame = frame.substring(1);
  for (int ledIndex = 0; ledIndex < numLeds; ledIndex++) {
    for (int channelIndex = 0; channelIndex < numChannels; channelIndex++) {
      int subIndex = ledIndex * 6 + channelIndex * 2;
      String stringValue = frame.substring(subIndex, subIndex + 2);
      char charString[2];
      stringValue.toCharArray(charString, 4);
      int val = strtoul(charString, NULL, 16);
      animationArray[frameIndex][ledIndex][channelIndex] = val;
    }
  }
}






