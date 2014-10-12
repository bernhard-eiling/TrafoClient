#pragma once
#include "Arduino.h"

#define numLeds 34
#define numChannels 3
#define totalCurrentLimit 1000

struct Color{
  byte r;
  byte g;
  byte b;
};

Color colorBuffer[numLeds]; 

class LedController {

public:
  LedController();
  void setupDisplayLedIndices();
  void setupLedControl();
  void loadFrame(int animationArray[][34][3], int frameIndex);
  void sendColorBuffer();
};

LedController::LedController(){

};

int brightnessMultiplier = 0;
int displayLeds[numLeds];

void LedController::setupDisplayLedIndices(){
  
  int otherLedCount=0;
  for(int i = 0; i < numLeds; i++) {
    displayLeds[otherLedCount] = i;
    otherLedCount++;
  }
  
}

// initialized SPI to talk to the LEDs
void LedController::setupLedControl() {
  //we use SPI to send data to the LEDs:
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV8); // 2 MHz
  float totalMaxCurrent = numLeds * 3 * 20; //each color draws 20mA max per Pixel
  float safeMultiplier = totalCurrentLimit / totalMaxCurrent;
  brightnessMultiplier = floor(safeMultiplier * 255);

}

void LedController::sendColorBuffer(){
  byte* rawData = (byte*) colorBuffer; // we treat the color array as raw bytes to speed up the next step.
  int dataLength = numLeds * 3;
  for (int i = 0; i < dataLength; i++) {
    unsigned int limitedColorPixelData = ((unsigned int) rawData[i] * brightnessMultiplier) >> 8;
    for (SPDR = (byte)limitedColorPixelData; !(SPSR & _BV(SPIF)););
  }
}

void LedController::loadFrame(int animationArray[][numLeds][numChannels], int frameIndex) {
  for(int i = 0; i < numLeds; i++) {
    colorBuffer[displayLeds[i]].r = animationArray[frameIndex][i][0];
    colorBuffer[displayLeds[i]].g = animationArray[frameIndex][i][1];
    colorBuffer[displayLeds[i]].b = animationArray[frameIndex][i][2];
  }
  sendColorBuffer();
}

