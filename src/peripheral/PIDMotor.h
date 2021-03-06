/*
 * PIDMotor.h
 *
 *  Created on: Mar 4, 2021
 *      Author: Abby Hyde
 */

#ifndef SRC_PERIPHERAL_PIDMOTOR_H_
#define SRC_PERIPHERAL_PIDMOTOR_H_

#include "Arduino.h"
#include "Encoder.h"
#include "PeripheralInterface.h"


/*
 * Class for a DC motor actuator to be controlled using a PID loop
 * Developed for the Pololu TODO controller
 */
class PIDMotor : public PeripheralInterface {

private:

	bool usePot 		= false;	// everyday
	bool useQuadEncoder = false;	// using a quadrature encoder as the feedback sensor, set in constructor


	uint8_t motorPWMpin = -1;    // must be set in constructor
	uint8_t motorDirPin = -1;  	 // must be set in constructor

	uint8_t potPin 	 = -1;		 // should be set in constructor, otherwise unused
	uint8_t encoderA = -1;		 // should be set in constructor, otherwise unused
	uint8_t encoderB = -1;		 // should be set in constructor, otherwise unused

	uint8_t currentSensor = -1;  // should be set in constructor, otherwise unused


	Encoder * encoder;
	int64_t encoderPos = 0;

	//PWMServo ?


public:

    PIDMotor(uint8_t pwmPin, uint8_t directionPin, uint8_t curSensePin, uint8_t encoderPin);
    PIDMotor(uint8_t pwmPin, uint8_t directionPin, uint8_t curSensePin, uint8_t encoderPinA, uint8_t encoderPinB);

    void enable();
    void disable();
    void update();


    int64_t getPosition();
    float getCurrent();

    void overridePosition(int64_t val);
    void setOutput(float speed);


    void attachMotor(uint8_t pwmPin, uint8_t directionPin);
    void attachEncoder(uint8_t encoderPin);
    void attachEncoder(uint8_t encoderPinA, uint8_t encoderPinB);
    void attachCurrentSensor(uint8_t currentSensorPin);



};




#endif /* SRC_PERIPHERAL_PIDMOTOR_H_ */
