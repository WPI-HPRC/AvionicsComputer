/*
 * Looper.h
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_LOOPS_LOOPER_H_
#define SRC_LOOPS_LOOPER_H_

#include "Arduino.h"

#include <Metro.h>

#include "../../Constants.h"
#include "Loop.h"


/*
 * Class for handling all loop functionality for the system, Loop objects (systems/subsystems) are
 * registered to this handler to be executed together at a set iteration time step.
 * Every system with a loop runs onStart when began, onStop when halted, and onLoop periodically
 * when this object is called by the main system
 */
class Looper {

private:

	const float period = DT_LOOPER;				// period for all fast loops of system
	Metro loopTimer = Metro(period * 1000);		// passed to timer in milliseconds

	bool running_;								// is the Looper enabled and running?

	uint32_t timestamp_;						// last millis() timestamp of system loop execution
	uint32_t dt_;								// keeping track of the loop execution period

	uint8_t loopIndex;							// index in loops_ for next registered loop
	// make this a vector?
	Loop * loops_[TOTAL_NUM_LOOPS];				// list of all registered loops objects running, their pointers


public:

	Looper();
//	~Looper(){}

    void registerLoop(Loop * loop);

    void startLoops();
	void stopLoops();
	void runLoops();

	void printOutput();

};



#endif /* SRC_LOOPS_LOOPER_H_ */
