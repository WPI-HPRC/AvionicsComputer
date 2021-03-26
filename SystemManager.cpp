/*
 * SystemManager.cpp
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#include "SystemManager.h"

/*
* Constructor for this object SystemManager, should only be one instance running
*/
SystemManager::SystemManager(){

	state = Startup;
	looper = new Looper();

};

/*
 * Setup function for the system manager
 *
 * When main system is instantiated, subsystems are fields which get defined but initialized in this function
 * Main system has SPI/I2C/Serial objects which are defined and passed to their subsystems
 * Subsystems have init functions, pin bindings/config of peripherals happen there, not in any class constructor
 */
void SystemManager::mainSetup(){

	if(state != Startup){			// making sure this isn't run more than once
		return;
	}


	Wire.begin();									// initialize I2C bus
	Wire.setClock(i2c_freq);						// set its frequency

//	SPI.begin();									// initialize SPI bus
	// Test with setting higher freq SPI

#ifdef USE_DEBUG_SERIAL
	Serial.begin(baud);								// initialize USB port serial, gotta debug somehow
#endif												// apparently not needed in Teensy

#ifdef USE_PAYLOAD_ROBOT_SYSTEM
	robot->systemInit();				// initializing system object, registering all its subsystem loops
#endif


	looper->startLoops();				// all subsystems with their loops and peripherals successful init, start

}

/*
 * Main loop function of the system manager which is constantly iterated by the Arduino/Teensy loop()
 * This runs the highest level state machine of the computer, handles looper functionality,
 * calling to run the system loops when their setup is successfully and it is safe to do so
 */
void SystemManager::mainLoop(){


	switch(state){

	case Startup:

		mainSetup();
		state = WaitForConnect;

		break;

	case WaitForConnect:

		// implement this for tele-op robot? (WiFi, XBEE, LoRa?)

#ifdef USE_DEBUG_SERIAL

		if(!Serial){					// loop this state until serial port is opened, stalls program

			break;
		}
		Serial.println(F("Serial debug connected"));
#endif


		state = Running;

		break;

	case Running:
		//Serial.println(F("Running... ")); //Serial.println(millis());
		// Remember: everything in here runs as fast as possible!


		looper->runLoops();



		//if(runCount == 0)looper->stopLoops();
		//runCount--;

		//TODO
		//check for Halting case or something that should stop loop running, external button
		//make that state be set by a button on a pin, sets state = Halting in here
		//In Halting:
		//robot->systemSafeHalt
		break;

	default:
		break;
	}


}
