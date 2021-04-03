/*
 * IndicatorLED.cpp
 * Created on: Mar 24, 2021
 * Author: Peter Dentch
 */

#include "IndicatorLED.h"


/*
 * Constructor for the indicator LED peripheral, takes in the digital output pin used
 * @param pwmPin is the digital pin used
 */
IndicatorLED::IndicatorLED(uint8_t pin) {

	pinLED = pin;

}


/*
 * Function to toggle the output of the LED
 * @param mode should be the macros HIGH and LOW
 */
void IndicatorLED::set(uint8_t mode) {

	digitalWriteFast(pinLED, mode);

}


/*
 * Init function for the IndicatorLED
 */
void IndicatorLED::enable() {

	pinMode(pinLED, OUTPUT);

	set(HIGH);

}

/*
 * Update function for the IndicatorLED
 * makes use of the Metro library which simply reads millis()/micros()
 */
void IndicatorLED::update() {

	if(blinkTimer.check() == 1){		// check if the timer has passed it's interval

		digitalWriteFast(pinLED, !digitalReadFast(pinLED));	// toggle the pin

	}
}


void IndicatorLED::disable() {
	//no-op
}


