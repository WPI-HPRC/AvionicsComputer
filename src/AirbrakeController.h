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

/*
 * Airbrake controller has TODO primary states of autonomous operation throughout its mission which begins when the system is powered on
 *
 * ON_PAD has the rocket constantly calibrating the IMU and transmitting data
 *
 * POWERED_FLIGHT has the rocket ascending under the power of the motor. Rocket is transmitting and logging telemetry
 *
 * UNPOWERED_FLIGHT has the rocket coasting to apogee. The rocket is calculating apogee based on sensor data and actuating
 * 	airbrakes to reach target. Rocket is transmitting and logging telemetry
 *
 * DESCENT has the rocket in descending after reaching apogee and deploying parachutes. Rocket is logging and transmitting telemetry
 *
 * LANDED has the rocket landed on the ground to be recovered. Rocket is transmitting GPS data
 */
enum FlightState {
	ON_PAD,						//Rocket is on the pad waiting to launch
	POWERED_FLIGHT,				//Rocket is in powered flight, motor is burning
	UNPOWERED_FLIGHT,			//Rocket is in unpowered flight, ascending coasting to apogee
	DESCENT,					//Rocket has reached apogee and is descending
	LANDED,						//Rocket has landed
	ABORT						//Abort state
};

/*
 * The airbrake controller system object, instantiates all airbrake controller subsystems and registers their loops
 *
 */
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
