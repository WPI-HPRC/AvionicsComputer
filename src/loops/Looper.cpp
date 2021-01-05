/*
 * Looper.cpp
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#include "Looper.h"



/*
 * Constructor for Looper object, should be only one instance
 */
Looper::Looper(){

	running_ = false;			// system is not running on instantiation, only on call to startLoops()

	timestamp_ = 0;
	dt_ = 1;					// 1 to prevent divide by zero for Hz calc
	loopIndex = 0;

}


/*
 * Function to add a Loop object to the Looper's list of pointers so they can be executed accordingly
 * @param loop is the loop to add to the list
 */
void Looper::registerLoop(Loop * loop){

	loops_[loopIndex] = loop;		// enabled Looper needs loops successfully registered to be run
	loopIndex++;

}


/*
 * Function to start all registered loops, executes all onStart()s
 */
void Looper::startLoops(){

	if(!running_){

		if(loopIndex != TOTAL_NUM_LOOPS){								// don't start unless all loops registered
			Serial.println(F("!!! Not all Loops registered !!!"));
			Serial.print(F("Loops registered: ")); Serial.println(loopIndex);
			return;
		}

		uint32_t now = micros();	// micros() overflows in about an hour

		for(auto &loop : loops_){
			loop->onStart(now);		// where the magic begins, all loops started
		}

		timestamp_ = micros();
		dt_ = timestamp_ - now;
//		dt_ = now - timestamp_;
//		timestamp_ = now;

		Serial.println(F("Loops started"));
		Serial.print(F("Took ")); Serial.print((float)dt_ / 1000, 8); Serial.println(F(" ms to start loops"));
		//printOutput();

	}
	running_ = true;	// if all onStart()s run successfully system is now running

}


/*
 * Function to stop all registered loops, executes all onStop()s
 */
void Looper::stopLoops(){

	if(running_){

		Serial.println(F("Loops stopped"));
		Serial.print(F("Looper timestamp: ")); Serial.println(timestamp_);
		printOutput();
		running_ = false;

		for(auto &loop : loops_){
			timestamp_ = micros();
			loop->onStop(timestamp_);		// all magic ends safely, should be called from Halting/ESTOP, etc.
		}

	}

}

/*
 * Function to be iterated over and over, runs all loops of the system by executing onLoop()s
 */
void Looper::runLoops(){

	if(running_){

		if(loopTimer.check() == 1){			// check if the timer has passed it's interval

			uint32_t now = micros();		// micros() overflows in about an hour

			for(auto &loop : loops_){
				loop->onLoop(now);			// where the magic happens, all loops run
			}

			dt_ = now - timestamp_;
			timestamp_ = now;

		}

	}
	else {
//		Serial.println(F("! Loops not run !"));
	}
}

/*
 * Debug printing function for the Looper object, outputs looper execution properties (frequency, delta t, time stamp)
 */
void Looper::printOutput(){

//	Serial.print(F("Looper timestamp: ")); Serial.println(timestamp_);
//	Serial.print(F("Looper dt: ")); Serial.println(dt_);

	Serial.print(F("Took ")); Serial.print((float)dt_ / 1000, 3); Serial.println(F(" ms to run"));
	//Serial.print(F("Looper frequency: "));
	Serial.print( 1 / ((float)dt_ / 1000000) ); Serial.println(F(" Hz"));

}
