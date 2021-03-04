/*
 * PIDMotor.cpp
 *
 *  Created on: Mar 4, 2021
 *      Author: Abby Hyde
 */

#include "PIDMotor.h"

PIDMotor::PIDMotor() {

}

void PIDMotor::enable() {


}

void PIDMotor::disable() {


}

void PIDMotor::update() {
	long newPos;
	newPos = testEncoder->read();
	if (newPos != encoderPos) {
	Serial.print("Position = ");
	Serial.print(newPos);
	Serial.println();
	encoderPos = newPos;
	}

}



