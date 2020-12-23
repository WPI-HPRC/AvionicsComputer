#include "Arduino.h"

#include "src/peripheral/MPU6050.h"

const uint32_t BAUD = 115200;

MPU6050 myMPU;

void setup() {

	Serial.begin(BAUD);								// issues sometime arise if this isn't done in setup
	while(!Serial);									// loop until serial port is opened, stalls program

	myMPU.enable();									//Runs the gyroStart method that

	Serial.println(F("Setup ran"));					//Prints the statement "Setup ran" to indicate that the program is working correctly.
}

void loop() {

	myMPU.update();
	Serial.println(myMPU.getAcc_z());
	delay(1000);

}
