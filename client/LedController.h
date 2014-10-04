#pragma once
#include "Arduino.h"

#define numLeds 34
#define totalCurrentLimit 1000

struct Color{
  byte r;
  byte g;
  byte b;
};

Color colorBuffer[numLeds]; 

/////////////////////
// DECLARATION

class LedController {

public:
  LedController();

  void setupDisplayLedIndices();
  void setupLedControl();
  bool contains(int* array, int length,int value);

  void loadFrame(int animationArray[][34][3], int frameIndex);
  void red();
  void white();
  void black();
  void sendColorBuffer();
};

/////////////////////
// IMPLEMENTATION

LedController::LedController(){

};

int logoLeds[] = {
  0, 1, 2, 3};
int brightnessMultiplier = 0;
int displayLeds[numLeds];
/*
// check if array contains a value
 bool LedController::contains(int* array, int length,int value){
 for(int i=0;i<length;i++){
 if(array[i]==value) return true;
 }
 return false;
 };
 */
/*
void LedController::setupDisplayLedIndices(){
 // create a list that only contains the LEDs that shall be used for display
 int otherLedCount=0;
 for(int i = 0; i < numLeds; i++) {
 if(!contains(logoLeds, nLogoLeds, i)) {
 displayLeds[otherLedCount] = i;
 otherLedCount++;
 }
 }
 }
 */
void LedController::setupDisplayLedIndices(){
  // create a list that only contains the LEDs that shall be used for display
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
  // SPI.setClockDivider(SPI_CLOCK_DIV16); // 1 MHz
  SPI.setClockDivider(SPI_CLOCK_DIV8); // 2 MHz
  // SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz
  //calculate a brightness scaling factor that honors the current limit:
  float totalMaxCurrent = numLeds * 3 * 20; //each color draws 20mA max per Pixel
  float safeMultiplier = totalCurrentLimit / totalMaxCurrent;
  brightnessMultiplier = floor(safeMultiplier * 255);
  // we use bitshifting to modify the brightness values because it is faster than dividing
  // but this means we can only 'divide' by powers of two.
  //&brightnessBitShift= (int)ceil(log(1.0/safeMultiplier)/log(2.0)); // this calculates the bitshift that is always on the safe side.

}

void LedController::sendColorBuffer(){
  byte* rawData = (byte*) colorBuffer; // we treat the color array as raw bytes to speed up the next step.
  int dataLength = numLeds * 3;
  for (int i = 0; i < dataLength; i++) {
    unsigned int limitedColorPixelData = ((unsigned int) rawData[i] * brightnessMultiplier) >> 8;
    //byte limitedColorPixelData=rawData[i]>>brightnessBitShift; // reduce the brightness value to limit power consumption
    //load the data into the SPI DATA register (SPDR) and wait for the transmission to be finished (SPSR & _BV(SPIF)).
    for (SPDR = (byte)limitedColorPixelData; !(SPSR & _BV(SPIF)););
  }
}

void LedController::loadFrame(int animationArray[][34][3], int frameIndex) {
  for(int i = 0; i < numLeds; i++) {
    colorBuffer[displayLeds[i]].r = animationArray[frameIndex][i][0];
    colorBuffer[displayLeds[i]].g = animationArray[frameIndex][i][1];
    colorBuffer[displayLeds[i]].b = animationArray[frameIndex][i][2];
  }
  sendColorBuffer();
}

void LedController::red(){
  for(int i = 0; i < numLeds; i++) {
    colorBuffer[displayLeds[i]].r = 255;
    colorBuffer[displayLeds[i]].g = 0;
    colorBuffer[displayLeds[i]].b = 0;
  }
}

void LedController::white(){
  for(int i = 0; i < numLeds; i++) {
    colorBuffer[displayLeds[i]].r = 255;
    colorBuffer[displayLeds[i]].g = 255;
    colorBuffer[displayLeds[i]].b = 255;
  }
}

void LedController::black(){
  for(int i = 0; i < numLeds; i++) {
    colorBuffer[displayLeds[i]].r = 0;
    colorBuffer[displayLeds[i]].g = 0;
    colorBuffer[displayLeds[i]].b = 0;
  }
}


