/*
 * PIDMotor.h
 *
 *  Created on: Mar 4, 2021
 *      Author: Abby Hyde
 */

#ifndef SRC_PERIPHERAL_PIDMOTOR_H_
#define SRC_PERIPHERAL_PIDMOTOR_H_

#include "Arduino.h"
#include "../../Constants.h"
#include "Encoder.h"
#include "PWMServo.h"
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
	uint8_t motorDirPin1 = -1;  	 // must be set in constructor
	uint8_t motorDirPin2 = -1;  	 // must be set in constructor

	uint8_t potPin 	 = -1;		 // should be set in constructor, otherwise unused
	uint8_t encoderA = -1;		 // should be set in constructor, otherwise unused
	uint8_t encoderB = -1;		 // should be set in constructor, otherwise unused

	uint8_t currentSensor = -1;  // should be set in constructor, otherwise unused

	uint16_t analogRes = ANALOGRES;


	Encoder * encoder;
	int64_t encoderPos = 0;

	PWMServo motor;


public:

    PIDMotor(uint8_t pwmPin, uint8_t directionPin1, uint8_t directionPin2, uint8_t curSensePin, uint8_t encoderPin);
    PIDMotor(uint8_t pwmPin, uint8_t directionPin1, uint8_t directionPin2, uint8_t curSensePin, uint8_t encoderPinA, uint8_t encoderPinB);

    void enable();
    void disable();
    void update();


    int64_t getPosition();
    float getCurrent();

    void overridePosition(int64_t val);

    uint16_t speedToAnalog(float speed);
    float fMap(float x, float inMin, float inMax, float outMin, float outMax);
    void setSpeed(float speed);


    void attachMotor(uint8_t pwmPin, uint8_t directionPin1, uint8_t directionPin2);
    void attachEncoder(uint8_t encoderPin);
    void attachEncoder(uint8_t encoderPinA, uint8_t encoderPinB);
    void attachCurrentSensor(uint8_t currentSensorPin);



};




#endif /* SRC_PERIPHERAL_PIDMOTOR_H_ */
