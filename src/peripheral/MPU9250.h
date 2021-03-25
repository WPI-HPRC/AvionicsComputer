/*
 * MPU9050.h
 *
 *  Created on: Mar 24, 2021
 *      Author: Giovanni Giacalone
 */

#ifndef SRC_PERIPHERAL_MPU9250_H_
#define SRC_PERIPHERAL_MPU9250_H_

#include "MPU6050.h"

#define MAGNETOMETER_CONFIG

class MPU9250 : public MPU6050 {
private:

public:
	MPU9250();

	void enable();
	void updateSensorValues();
	void update();

	void complementaryFilter();

	float getFilteredYaw();
};



#endif /* SRC_PERIPHERAL_MPU9250_H_ */
