/*
 * MPU9050.h
 *
 *  Created on: Mar 24, 2021
 *      Author: Giovanni Giacalone
 */

#ifndef SRC_PERIPHERAL_MPU9250_H_
#define SRC_PERIPHERAL_MPU9250_H_

#include "MPU6050.h"

#define MAG_AD 0xC 					//Defines Magnetometer Address.
#define HXL 0x03					//Defines the register for the Low byte of the X Axis Magnetometer Output.
#define CNTL1_AD 0x0A  				//Defines the control_1 Address.
#define USER_CTRL 0x6A         		//Defines the User Control Address.
#define INT_BYPASS_CONFIG 0x37   	//Defines the Bypass Enable Configuration Address.

class MPU9250 : public MPU6050 {
private:

	int16_t magneX = 0; 			// raw X-axis value
	int16_t magneY = 0; 			// raw Y-axis value
	int16_t magneZ = 0; 			// raw Z-axis value

	int8_t status_2 = 0; 			// STATUS_2 register, 4th bit indicates 16-bit output, 3rd bit indicates if overflow occurred

public:
	MPU9250();

	void enable();
	void updateSensorValues();
	void update();

	void complementaryFilter();

	float getFilteredYaw();
};



#endif /* SRC_PERIPHERAL_MPU9250_H_ */
