/*
 * PIDMotor.cpp
 *
 *  Created on: Mar 4, 2021
 *      Author: Abby Hyde
 */

#include "PIDMotor.h"


/*
 * Constructor for PIDMotor using a potentiometer as a rotary encoder for feedback
 * @param pwmPin is the pin delivering a PWM signal for the motor controller
 * @param directionPin is the direction toggling for the motor controller
 * @param currentSensorPin is the analog input pin for the motor controller current feedback
 * @param encoderPin is the analog input pin for the potentiometer feedback
 */
PIDMotor::PIDMotor(uint8_t pwmPin, uint8_t directionPin, uint8_t curSensePin, uint8_t encoderPin) {

	motorPWMpin = pwmPin;
	motorDirPin = directionPin;
	currentSensor = curSensePin;

	potPin = encoderPin;
	usePot = true; 			// motor be blazin
}


/*
 * Constructor for PIDMotor using a quadrature rotary encoder for feedback
 * @param pwmPin is the pin delivering a PWM signal for the motor controller
 * @param directionPin is the direction toggling for the motor controller
 * @param currentSensorPin is the analog input pin for the motor controller current feedback
 * @param encoderPinA is the encoder signal pin A
 * @param encoderPinB is the encoder signal pin B
 */
PIDMotor::PIDMotor(uint8_t pwmPin, uint8_t directionPin, uint8_t curSensePin, uint8_t encoderPinA, uint8_t encoderPinB) {

	motorPWMpin = pwmPin;
	motorDirPin = directionPin;
	currentSensor = curSensePin;

	encoderA = encoderPinA;
	encoderB = encoderPinB;
	useQuadEncoder = true;

}


void PIDMotor::enable() {

	attachMotor(motorPWMpin, motorDirPin);

	if(currentSensor != 255){						// pin num is a uint so -1 = 255
		attachCurrentSensor(currentSensor);
	}

	if(useQuadEncoder) {		// motor was instantiated to use a quadrature encoder for feedback?
		attachEncoder(encoderA, encoderB);
	}

	else if(usePot) {			// motor was instantiated to use a potentiometer for feedback?
		attachEncoder(potPin);
	}

	else { // if we end up here, constructor must not have been run properly...

	}


}

void PIDMotor::disable() {

}

void PIDMotor::update() {


	uint32_t newPos;
	newPos = encoder->read();

	if (newPos != encoderPos) {

		Serial.print("Position = ");
		Serial.print(newPos);
		Serial.println();

		encoderPos = newPos;
	}

}


/*
 * Bind the proper motor signal pin, uses pinMode() and
 * The analog linear potentiometer pin is set
 * This function should not be run in a constructor, only during or after setup()
 * @param encoderPin is the analog input pin for the potentiometer feedback
 */
void PIDMotor::attachEncoder(uint8_t encoderPin) {

	//potPin
	// analog pins are input by default, doesn't hurt to pinMode tho
	//TODO analogReadResolution
	//TODO check to make sure the pin is a proper analog input pin, print error if not?

}


/*
 * Bind the proper motor signal pin, uses pinMode() and
 * The (quadrature) encoder pins are set on instantiation of an encoder object.
 * This function should not be run in a constructor, only during or after setup()
 * @param encoderPinA is the encoder signal pin A
 * @param encoderPinB is the encoder signal pin B
 */
void PIDMotor::attachEncoder(uint8_t encoderPinA, uint8_t encoderPinB) {

	// binding the quad encoder signal pins, Encoder class uses pinMode() in its constructor,
	// making it necessary to instantiate it in the systemInit which runs after setup()
	encoder = new Encoder(encoderPinA, encoderPinB);

}


/**
 * Bind the proper motor signal and direction pins, uses pinMode()
 * This function should not be run in a constructor, only during or after setup()
 * @param pwmPin is the pin delivering a PWM signal for the motor controller
 * @param directionPin is the direction toggling for the motor controller
 */
void PIDMotor::attachMotor(uint8_t pwmPin, uint8_t directionPin) {

	//use PWMServo library by PaulS ?
	pinMode(pwmPin, OUTPUT);
	pinMode(directionPin, OUTPUT);
}


/**
 * Bind the proper motor signal pin for direct current measurement, uses pinMode()
 * @param currentSensorPin is the analog input pin for the motor controller current feedback
 */
void PIDMotor::attachCurrentSensor(uint8_t currentSensorPin) {

	//currentSensor
	// analog pins are input by default, doesn't hurt to pinMode tho
	//TODO analogReadResolution
	//TODO check to make sure the pin is a proper analog input pin, print error if not?

}

