/*
 * NEOM9.cpp
 *
 *  Created on: Jan 5, 2021
 *  	Author: Michael Beskid
 */

#include "NEOM9.h"

void NEOM9::enable(){
	initGPS();
}

/*
 * Initializes GPS
 * No registers to set, checks if I2C device on GPS address
 * @return true if succesful commuication with device, false if error
 */
bool NEOM9::initGPS(){
	Wire.beginTransmission(NEOM9_ADDRESS);
	return !Wire.endTransmission(true);			//endTransmission returns 0 if success, error code if failure
}

void NEOM9::disable(){

}

void NEOM9::update(){

	uint16_t bytesAvailable = 0;
	Wire.beginTransmission(NEOM9_ADDRESS);
	Wire.write(0xFD);                     			//0xFD (MSB) and 0xFE (LSB) are the registers that contain number of bytes available
	if (Wire.endTransmission(false) != 0) { 		//Send a restart command. Do not release bus.
		Serial.println("BIG SAD");
		return;                         		 	//Sensor did not ACK
	}

	Wire.requestFrom(NEOM9_ADDRESS, 2);

	uint8_t msb = Wire.read();
	uint8_t lsb = Wire.read();
	bytesAvailable = msb <<8| lsb;

	Serial.print(msb);
	Serial.print(", ");
	Serial.println(lsb);
	Serial.println(bytesAvailable);
}
