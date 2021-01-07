/*
 * MPU6050.h
 *
 *  Created on: Dec 22, 2020
 *      Author: Giovanni Giacalone
 */

#ifndef SRC_PERIPHERAL_MPU6050_H_
#define SRC_PERIPHERAL_MPU6050_H_

#include "Arduino.h"
#include "Wire.h"
#include "PeripheralInterface.h"
#include "../../Constants.h"

#define MPU6050_ADDRESS 0x68		//Defines the gyro address.
#define MPU6050_PWR_MGMT_1 0x6B		//Defines the register for power management.
#define GYRO_CONFIG 0x1B			//Defines the register for the gyro configuration.
#define SLEEP_MODE 0				//Defines the Sleep Mode to be 0 or disabled to "Wake Up" the gyro.
#define ACCEL_CONFIG 0x1C			//Defines the register for the Accelerometer configuration.
#define FS_SEL B00001000			//Defines the FS_SEL gyro range to be +- 500 deg/s.
#define AFS_SEL B00011000			//Defines the AFS_SEL accelerometer range to be +- 8g.
#define ACCEL_XOUT_H 0x3B

class MPU6050 : public PeripheralInterface {
private:
	const float dt = DT_LOOPER;
	const float LSB = 65.5;			//Defines Least-significant Bit per deg/s for 500 deg/s sensitivity.

	uint16_t calibrationIndex = 0;  			//This is an index variable used to iterate the recalibrateGyro function.
	const uint16_t calibrationSamples = 500;	//This is the total number of calibration samples to be taken per calibration routine.
	int32_t gyro_x_reCal, gyro_y_reCal, gyro_z_reCal = 0;

	float roll, pitch, yaw = 0;
	int32_t gyro_x_cal, gyro_y_cal, gyro_z_cal = 0;

	int16_t acc_x, acc_y, acc_z = 0;
	int16_t temperature = 0;
	int16_t gyro_x, gyro_y, gyro_z = 0;

public:
	MPU6050();

	void enable();
	void disable();
	void updateGyroValues();
	void update();
	void gyroCalibrateOnce();
	void recalibrateGyro();

	float getRoll();
	float getPitch();
	float getYaw();

	int16_t getRawAcc_x();
	int16_t getRawAcc_y();
	int16_t getRawAcc_z();
	int16_t getRawTemperature();
	int16_t getRawGyro_x();
	int16_t getRawGyro_y();
	int16_t getRawGyro_z();

};

#endif /* SRC_PERIPHERAL_MPU6050_H_ */