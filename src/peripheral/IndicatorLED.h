/*
 * IndicatorLED.h
 * Created on: Mar 24, 2021
 * Author: Peter Dentch
 */

#ifndef SRC_PERIPHERAL_INDICATORLED_H_
#define SRC_PERIPHERAL_INDICATORLED_H_

#include "Arduino.h"

#include <Metro.h>

#include "PeripheralInterface.h"
#include "../../Constants.h"


/*
 * A wrapper object for an LED which turns on, off, and or blinks to indicate
 * system state functionality. Functions in this peripheral are for the convenience
 * of system Looper functionality
 */
class IndicatorLED : public PeripheralInterface {

private:

	uint8_t pinLED = -1;

	float period = 0.5;							// A hard-coded 2Hz blink TODO
	Metro blinkTimer = Metro(period * 1000);	// passed to timer in milliseconds


public:

	IndicatorLED(uint8_t pin);

	void set(uint8_t mode);

	void enable();
	void disable();
	void update();

};



#endif /* SRC_PERIPHERAL_INDICATORLED_H_ */
