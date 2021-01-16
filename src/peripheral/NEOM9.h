/*
 * NEOM9.h
 *
 *  Created on: Jan 5, 2021
 *  	Author: Michael Beskid
 */

#ifndef SRC_PERIPHERAL_NEOM9_H_
#define SRC_PERIPHERAL_NEOM9_H_

#include "Arduino.h"
#include "Wire.h"
#include "PeripheralInterface.h"

#define NEOM9_ADDRESS 0x42		//Defines the GPS address.

class NEOM9 : public PeripheralInterface {

private:

	const uint8_t UBX_CLASS_NAV = 0x01;
	const uint8_t UBX_NAV_PVT = 0x07;

public:
	bool initGPS();
	void enable();
	void disable();
	void update();

};
















#endif /* SRC_PERIPHERAL_NEOM9_H_ */
