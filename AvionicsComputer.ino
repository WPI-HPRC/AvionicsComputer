#include "Arduino.h"
#include "MPU6050.h"

const uint32_t BAUD = 115200;

void setup() {

	Serial.begin(BAUD);								// issues sometime arise if this isn't done in setup
	while(!Serial);									// loop until serial port is opened, stalls program

	MPU6050->enable();									//Runs the gyroStart method that

	Serial.println(F("Setup ran"));					//Prints the statement "Setup ran" to indicate that the program is working correctly.
}

void loop() {

	getGyroValues();
	Serial.println(acc_z);
	delay(5);

}
