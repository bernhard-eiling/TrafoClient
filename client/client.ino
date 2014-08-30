#include <SoftwareSerial.h>
//SoftwareSerial wifiSerial(8,9);

const char mySSID[] = "TrafoControl";
const char myPassword[] = "123";

void terminal();

WiFly wifly;

void setup()
{
    char buf[32];

    Serial.begin(115200);
    Serial.println("Starting");
    Serial.print("Free memory: ");
    Serial.println(wifly.getFreeMemory(),DEC);

    //wifiSerial.begin(9600);

    //if (!wifly.begin(&wifiSerial, &Serial1)) {
    if (!wifly.begin(&Serial1)) {
        Serial.println("Failed to start wifly");
	terminal();
    }

    if (wifly.getFlushTimeout() != 10) {
        Serial.println("Restoring flush timeout to 10msecs");
        wifly.setFlushTimeout(10);
	wifly.save();
	wifly.reboot();
    }
    
    join();
    
    pingGateway();

    wifly.getGateway(buf, sizeof(buf));
    //wifly.setIpProtocol(WIFLY_PROTOCOL_TCP);
    //wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
    wifly.setIpProtocol(WIFLY_PROTOCOL_TCP_CLIENT);

    Serial.print("MAC: ");
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print("IP: ");
    Serial.println(wifly.getIP(buf, sizeof(buf)));
    Serial.print("Netmask: ");
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    Serial.print("Gateway: ");
    Serial.println(wifly.getGateway(buf, sizeof(buf)));
    
    wifly.setDeviceID("Trafo-Client");
    Serial.print("DeviceID: ");
    Serial.println(wifly.getDeviceID(buf, sizeof(buf)));
}

void loop()
{


    if (wifly.isConnected()) {
      Serial.println("Connected");
    } else {
      //join();
    }

    if (Serial.available()) {
        /* if the user hits 't', switch to the terminal for debugging */
        if (Serial.read() == 't') {
	    terminal();
	}
    }

}

void join() {
      
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
	//wifly.setPassphrase(myPassword);
	//wifly.enableDHCP();
        wifly.setIP("192.168.1.");
        wifly.save();
        // IP wont be set...

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	}
}

void pingGateway() {
    /* Ping the gateway */
    char buf[32];
    wifly.getGateway(buf, sizeof(buf));

    Serial.print("ping ");
    Serial.print(buf);
    Serial.print(" ... ");
    if (wifly.ping(buf)) {
	Serial.println("ok");
    } else {
	Serial.println("failed");
    }
}

void terminal()
{
    Serial.println("Terminal ready");
    Serial.println(wifly.available());
    while (1) {
	if (wifly.available() > 0) {
            Serial.println("Waiting for incoming");
	    Serial.write(wifly.read());
	}

	if (Serial.available()) {
	    wifly.write(Serial.read());
	}
    }
}
