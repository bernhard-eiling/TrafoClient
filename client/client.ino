#include <SPI.h>
#include <WiFlyHQ.h>
#include "LedController.h"

#define numLeds 34
#define numChannels 3
#define maxNumFrames 100

WiFly wifly;
LedController controller;
int frameArray[numLeds][numChannels];
//int numFrameChannels = maxNumFrames * numChannels;
String rawFrames[102];
int loadCounter = 0;

void setup()
{ 
  // SETUP Leds
  controller.setupDisplayLedIndices();
  controller.setupLedControl();

  // SETUP WiFLy
  Serial.begin(115200);
  wifly.setupForUDP<HardwareSerial>(
  &Serial1,   //the serial you want to use (this can also be a software serial)
  115200, // if you use a hardware serial, I would recommend the full 115200. This does not work with a software serial.
  true,	// should we try some other baudrates if the currently selected one fails?
  "TrafoControl",  //Your Wifi Name (SSID)
  //"SommerInAltTreptow",  //Your Wifi Name (SSID)
  "1234567890", //Your Wifi Password
  "WiFly",                 // Device name for identification in the network
  0,         // IP Adress of the Wifly. if 0 (without quotes), it will use dhcp to get an ip
  997,                    // WiFly receive port
  "255.255.255.255",       // Where to send outgoing Osc messages. "255.255.255.255" will send to all hosts in the subnet
  8757,                     // outgoing port
  true	// show debug information on Serial
  );
}


void loop()
{

  /////////////////////////
  // SERIAL READING
  String data;

  if (Serial1.available()) {
    data = Serial1.readStringUntil('t');
    //data = Serial1.readString();
    Serial.println(data);
    //String frame;

    if (data.startsWith("load")) {
      rawFrames[loadCounter] = data;  
      loadCounter++;
      //parseLoad(data);
    } else if (data.startsWith("sync")) {
      parseSync(data);
    } else if (data.startsWith("finish")) {
      for (int i = 0; i < loadCounter; i++) {
        Serial.println(rawFrames[i]);
        //parseLoad(rawFrames[i]);
        //Serial.write("\n------+----------+-------\n"); 
      }
      loadCounter = 0;
    }
    
/*
    for (int i = 0; i < numLeds; i++) {
      for (int k = 0; k < numChannels; k++) {
        Serial.print(frameArray[i][k]);
        Serial.print(", ");
      }
    }
*/
    Serial.write("\n-----------------------\n"); 
  }
  if (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte); 
    Serial1.write("xxxxxxxxxxxxxxxxxxxxxx\n"); 
  }

  ////////////////////////////
  // WiFly CONTROLS
  /*
  if (wifly.available() > 0) {
   Serial.write(wifly.read());
   Serial.println("-----------------------"); 
   }
   */

  ////////////////////
  // LED CONTROLS
  /*
  controller.white();
   controller.sendColorBuffer();
   delay(100);
   controller.black();
   controller.sendColorBuffer();
   delay(100);
   */
}

void parseSync(String frame) {

}

void parseChannel(String channel) {
  
}

void parseLoad(String frame) {
  int endFramePos;
  int endChannelPos;

  frame = frame.substring(4);
  Serial.println(frame);

  for (int ledIndex = 0; ledIndex < numLeds; ledIndex++) {
    endFramePos = frame.indexOf('c');
    if (endFramePos != -1) {
      String led = frame.substring(0, endFramePos);
      for (int channelIndex = 0; channelIndex < numChannels; channelIndex++) {
        endChannelPos = led.indexOf(',');
        if (endChannelPos != -1) {
          String value = led.substring(0, endChannelPos);
          frameArray[ledIndex][channelIndex] = value.toInt();
        }
        led = led.substring(endChannelPos + 1, led.length());
      }
      frame = frame.substring(endFramePos+1, frame.length());
    }
  }
}









