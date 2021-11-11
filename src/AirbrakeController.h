/*
 * AirbrakeController.h
 *
 *  Created on: Feb 27, 2021
 *      Author: Christian M. Schrader
 */

#ifndef SRC_AIRBRAKECONTROLLER_H_
#define SRC_AIRBRAKECONTROLLER_H_

#include "Arduino.h"

#include "loops/Looper.h"
#include "loops/loop.h"

// Include Subsystems

enum FlightState {
	STANDBY,
	POWERED_FLIGHT,
	UNPOWERED_FLIGHT,
	DESCENT,
	LANDED,
	ABORT
};

class AirbrakeController {
private:
	Looper * runningLooper;

	FlightState flightState;

	// Sensors
	const int baroBufferSize = 100;
	float baroBuffer[100]; //[baroBufferSize];
	int baroBufIndex;
	const int accelBufferSize = 100;
	float accelBuffer[100]; //[accelBufferSize];
	int accelBufIndex;
	float avgBaroArray();
	float avgAccelArray();
	// Subsystems

public:
	AirbrakeController(Looper * looper);

	class AirbrakeLoop : public Loop {
		AirbrakeController * controller_;

	public:
		AirbrakeLoop(AirbrakeController * instance) {
			controller_ = instance;
		}

		void onStart(uint32_t timestamp) {
			controller_->beginStateMachine();
		}

		void onLoop(uint32_t timestamp) {
			controller_->updateStateMachine();
		}

		void onStop(uint32_t timestamp) {
			controller_->endStateMachine();
		}
	} * airbrakeLoop = new AirbrakeLoop(this);

	void systemInit();

	void zeroAllSensors();

	void beginStateMachine();
	void updateStateMachine();
	void endStateMachine();
};

#endif /* SRC_AIRBRAKECONTROLLER_H_ */
