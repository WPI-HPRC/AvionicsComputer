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

class PIDMotor : public PeripheralInterface {
private:
	long encoderPos = 0;
	Encoder encoder;

public:
    PIDMotor();

    void enable();
    void disable();
    void update();

};




#endif /* SRC_PERIPHERAL_PIDMOTOR_H_ */
