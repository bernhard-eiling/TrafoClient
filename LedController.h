#pragma once
#include "Arduino.h"
#include "SPI.h"

struct Color{
  byte r;
  byte g;
  byte b;
};

class LedController {

public:

	LedController::LedController();
/*
	void LedController::testFunction() {
		Serial.println("led test");
	}

	void LedController::setupDisplayLedIndices(){
	  // create a list that only contains the LEDs that shall be used for display
		int otherLedCount=0;
	  	for(int i=0;i<nTotalLeds;i++){
	    	if(!contains(logoLeds,nLogoLeds,i)){
	      		displayLeds[otherLedCount]=i;
	      		otherLedCount++;
	    	}
	  	}
	}

		// initialized SPI to talk to the LEDs
	void LedController::setupLedControl(){
	  //we use SPI to send data to the LEDs:
	  SPI.begin();
	  SPI.setBitOrder(MSBFIRST);
	  SPI.setDataMode(SPI_MODE0);
	  // SPI.setClockDivider(SPI_CLOCK_DIV16); // 1 MHz
	  SPI.setClockDivider(SPI_CLOCK_DIV8); // 2 MHz
	  // SPI.setClockDivider(SPI_CLOCK_DIV4); // 4 MHz
	  //calculate a brightness scaling factor that honors the current limit:
	  float totalMaxCurrent=nTotalLeds*3*20; //each color draws 20mA max per Pixel
	  float safeMultiplier=totalCurrentLimit/totalMaxCurrent;
	  brightnessMultiplier=floor(safeMultiplier*255);
	  // we use bitshifting to modify the brightness values because it is faster than dividing
	  // but this means we can only 'divide' by powers of two.
	  //&brightnessBitShift= (int)ceil(log(1.0/safeMultiplier)/log(2.0)); // this calculates the bitshift that is always on the safe side.
	  TRACE(F("brightness multiplier:"));
	  TRACELN(brightnessMultiplier); 
	};
	*/
}