/**
 * AvionicsComputer.ino
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 *
 * Main .ino file required by Sloeber/Eclipse IDE
 * Code developed to be run on Teensy 3.2
 *
 */

#include "Arduino.h"

#include "Constants.h"
#include "src/peripheral/LoRaCustom.h"
#include "src/peripheral/GyroAccel.h"

const uint32_t BAUD = 115200;


LoRaComms myRadio;
GyroAccel mpu6050;

uint8_t message[12];


// The setup routine runs once when you press reset:
void setup() {

	Serial.begin(BAUD);		// issues sometime arise if this isn't done in setup

	while(!Serial);			// loop until serial port is opened, stalls program


//	Have #ifdef AIRBRAKE_SYSTEM
//		 #ifdef PAYLOAD_ROBOT_SYSTEM
//		 #ifdef GROUNDSTATION_SYSTEM



	Serial.println(F("Setup ran"));
}



// The loop routine runs over and over again forever:
void loop() {



	if(myRadio.getMode() == RadioState::RHModeIdle){
				myRadio.send(message, 12);

				Serial.print(message[0]); Serial.print(F(", "));
				Serial.println(message[1]);
	}

	delay(10);

	mpu6050.pull(message, 12);
	Serial.print("mode is: "); Serial.println(myRadio.getMode());

}
