/*
 * ControlSystem.h
 *
 *  Created on: May 1, 2021
 *      Author: Max Schrader
 */

#ifndef SRC_SUBSYSTEMS_CONTROLSYSTEM_H_
#define SRC_SUBSYSTEMS_CONTROLSYSTEM_H_

#include "SubsystemInterface.h"
#include "../../Constants.h"
#include "../peripheral/MPU6050.h"
#include "../peripheral/MPL3115A2.h"

enum BrakeState {
	Inactive,
	Active,
	ShuttingDown
};

class ControlSystem : public SubsystemInterface {

private:
	BrakeState brakeState;
	float P;
	float I;
	float D;
	MPU6050 * imu;
	MPL3115A2 * barometer;

public:
	ControlSystem(float Prop, float Int, float Deriv, MPU6050 * imuSensor, MPL3115A2 * barometerSensor);

	class ControlLoop : public Loop {
		ControlSystem * control_;

	public:
		ControlLoop(ControlSystem * instance) {
			control_ = instance;
		};

		void onStart(uint32_t timestamp) {
			Serial.println(F("START AIRBRAKE CONTROL LOOP"));
		}

		void onLoop(uint32_t timestamp){

			switch(control_->brakeState) {
			case Inactive:
				return;
			case Active:
				return;
			case ShuttingDown:
				return;
			default:
				Serial.print("Unexpected control state: ");
				Serial.println(control_->brakeState);
				return;
			}
		}

		void onStop(uint32_t timestamp) {}
	} * controlLoop = new ControlLoop(this); // instantiate the DriveTrain subsystem loop and pass it the instance

	void subsystemInit();

	void idle();
	void zeroSensors();
	void registerEnabledLoops(Looper * enabledLooper);
	void printOutput();
};

#endif /* SRC_SUBSYSTEMS_CONTROLSYSTEM_H_ */
