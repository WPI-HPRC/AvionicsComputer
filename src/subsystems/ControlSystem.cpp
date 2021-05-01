/*
 * ControlSystem.cpp
 *
 *  Created on: May 1, 2021
 *      Author: Max Schrader
 */

#include "ControlSystem.h"

/**
 * Constructor for the control system, should only be instantiated once.
 * @param Prop:	Proportional Constant
 * @param Int:	Integral Constant
 * @param Deriv:Derivative Constant
 */
ControlSystem::ControlSystem(float Prop, float Int, float Deriv, MPU6050 * imuSensor, MPL3115A2 * barometerSensor) {
	P = Prop;
	I = Int;
	D = Deriv;
	imu = imuSensor;
	barometer = barometerSensor;
	brakeState = Inactive;
};

/**
 * Initialize the components of the subsystem.
 */
void ControlSystem::subsystemInit() {
	// Enable Sensors
}

/**
 * Perform idle actions
 */
void ControlSystem::idle() {

}

/**
 * Zero subsystem sensors
 */
void ControlSystem::zeroSensors() {

}

/**
 * Register the control loop
 */
void ControlSystem::registerEnabledLoops(Looper * enabledLooper) {
	enabledLooper->registerLoop(controlLoop);
}

/**
 * Print output of control subsystem.
 */
void ControlSystem::printOutput() {

}


