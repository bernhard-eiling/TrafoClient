#include <WiFlyHQ.h>
#include <ArdOSCForWiFlyHQ.h>

WiFly wifly;
OSCServer server(&wifly);

void setup()
{ 
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
  //wifly.printStatusInfo(); //print some debug information 
  server.addCallback("/test/function",&blinkAway);
}


void loop()
{
  server.availableCheck(2);
}

void blinkAway(OSCMessage *message) {
  String fadeValue = message -> getArgStringData(0);
  Serial.println(fadeValue);

  Serial.println("blink");
}

