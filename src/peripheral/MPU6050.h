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
#define AFS_SEL B00011000			//Defines the AFS_SEL accelerometer range to be +- 16g.
#define ACCEL_XOUT_H 0x3B

class MPU6050 : public PeripheralInterface {
private:
	const float dt = DT_LOOPER;
	const float gyroLSB = 65.5;			//Defines Least-significant Bit per deg/s for 500 deg/s sensitivity.
	const float accelLSB = 2048; 		//Defines Least-significant Bit per g for +-8g sensitivity.
	const float radToDeg = 180 / 3.14159;
	const float alpha = 0.98;
	const float maxGravity = 2;

	const float accXBias = ACCEL_X_BIAS;	//Average Accelerometer Bias for X axis
	const float accYBias = ACCEL_Y_BIAS;	//Average Accelerometer Bias for Y axis
	const float accZBias = ACCEL_Z_BIAS;	//Average Accelerometer Bias for Z axis

	uint16_t calibrationIndex = 0;  			//This is an index variable used to iterate the recalibrateGyro function.
	const uint16_t calibrationSamples = 2000;	//This is the total number of calibration samples to be taken per calibration routine.

	int32_t gyro_x_reCal = 0;
	int32_t gyro_y_reCal = 0;
	int32_t gyro_z_reCal = 0;

	float roll = 0;
	float pitch = 0;
	float yaw = 0;

	int32_t gyro_x_cal = 0;
	int32_t gyro_y_cal = 0;
	int32_t gyro_z_cal = 0;

	float totalAccelVector = 0;
	int16_t acc_x = 0;
	int16_t acc_y = 0;
	int16_t acc_z = 0;
	float accXg = 0;
	float accYg = 0;
	float accZg = 0;
	float anglePitchAccel = 0 ;
	float angleRollAccel = 0;

	float filteredPitch = 0;
	float filteredRoll = 0;

	int16_t temperature = 0;
	int16_t gyro_x = 0;
	int16_t gyro_y = 0;
	int16_t gyro_z = 0;

public:
	MPU6050();

	void enable();
	void disable();
	void updateSensorValues();
	void update();
	void gyroCalibrateOnce();
	void recalibrateGyro();
	void complementaryFilter();

	float getRoll();
	float getPitch();
	float getYaw();

	float getAccXg();
	float getAccYg();
	float getAccZg();
	int16_t getRawTemperature();
	int16_t getRawGyroX();
	int16_t getRawGyroY();
	int16_t getRawGyroZ();

	float getTotalAccelVector();

	float getFilteredPitch();
	float getFilteredRoll();

};

#endif /* SRC_PERIPHERAL_MPU6050_H_ */
