/*
 * SystemManager.h
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 */

#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include "Arduino.h"

#include "Constants.h"
#include "src/loops/Looper.h"
#include "src/loops/Loop.h"
#include "src/PayloadRobot.h"

/*
 * Overall system states, main loop execution with everything necessary before and after
 */
enum SystemState {
	// Overall system states, main loop execution with everything necessary before and after
	Startup, WaitForConnect, Running//, Halting //, ESTOP
};


/*
 * Class SystemManager is the starting point of all AvionicsComputer code, it manages
 * the iterative functionality of the system/subsystems. Subsystems are defined by the system
 * on which this code is running (AirbrakeSystem, PayloadRobotSystem, GroundStationSystem)
 */
class SystemManager{

private:

	SystemState state;			// initial system state is Startup

	Looper * looper;			// looper keeps track of the loops the system will be running


public:

	SystemManager();
	//	~SystemManager(){}

	const uint32_t baud = DEBUG_BAUD_RATE;		// baud rate for serial prints from the Teensy


//TODO:
//#ifdef AIRBRAKE_SYSTEM
//#ifdef PAYLOAD_ROBOT_SYSTEM
//#ifdef GROUNDSTATION_SYSTEM

	PayloadRobot * robot = new PayloadRobot(looper);	// looper is needed for registering all subsystem loops

	/* Robot loop functionality */
	class RobotLoop : public Loop {
		PayloadRobot * robot_;

	public:
		RobotLoop(PayloadRobot * instance){
			robot_ = instance;
		};

		void onStart(uint32_t timestamp){
			robot_->beginStateMachine();
		}
		void onLoop(uint32_t timestamp){
			robot_->updateStateMachine();

		}
		void onStop(uint32_t timestamp){
			robot_->endStateMachine();
		}
	} * robotLoop = new RobotLoop(robot);				// instantiate the main system loop and pass it the system instance

//#endif

//	long runCount = 100000;	// debugging, stopping loops after this many mainLoop() iterations



	void mainSetup();

	void mainLoop();

};


#endif /* SYSTEMMANAGER_H_ */
