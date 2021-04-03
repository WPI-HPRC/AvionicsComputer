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
#include "peripheral/MPL3115A2.h"
#include "peripheral/MPU6050.h"

#define BARO_BUFFER_SIZE 100
#define ACCEL_BUFFER_SIZE 100

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
	float baroBuffer[BARO_BUFFER_SIZE];
	int baroBufIndex = 0;
	float accelBuffer[ACCEL_BUFFER_SIZE];
	int accelBufIndex = 0;
	float avgBaroArray();
	float avgAccelArray();

	MPL3115A2* barometer = new MPL3115A2();
	MPU6050* accelerometer = new MPU6050();
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
