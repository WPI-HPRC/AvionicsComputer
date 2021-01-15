/*
 * MPU6050.cpp
 *
 *  Created on: Dec 22, 2020
 *      Author: Giovanni Giacalone
 */

#include "MPU6050.h"

/*
 * Constructor for this class MPU6050, should only be one instance from only one MPU6050 board per Arduino
 */
MPU6050::MPU6050(){}

/*
 *
 */
void MPU6050::enable(){

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(MPU6050_PWR_MGMT_1);             	//register for power management
	Wire.write(SLEEP_MODE);                 		//disable sleep mode, 'wake up' the device
	Wire.endTransmission(true);    					//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(GYRO_CONFIG);              			//Configures the gyro to the register GYRO_CONFIG.
	Wire.write(FS_SEL);         					//Sets the gyro range to FS_SEL.
	Wire.endTransmission(true);						//Ends the transmition with the MPU6050.

	Wire.beginTransmission(MPU6050_ADDRESS);  		//Starts the transmission with the MPU6050.
	Wire.write(ACCEL_CONFIG);              			//Configures the Accelerometer to the register ACCEL_CONFIG.
	Wire.write(AFS_SEL);         					//Set the accelerometer range to AFS_SEL.
	Wire.endTransmission(true);						//Ends the transmition with the MPU6050.

}

void MPU6050::disable(){
	//IM NOT SURE WHAT THIS IS SUPPOSED TO DO.
}

/*
 *
 */
void MPU6050::updateGyroValues(){
	Wire.beginTransmission(MPU6050_ADDRESS);       	//Starts the transmission with the MPU6050.
	Wire.write(ACCEL_XOUT_H);                   	//Begins with the ACCEL_XOUT_H register, the rest of the data registers begin there.
	Wire.endTransmission(false);        			//Leaves the transmition with the MPU6050 open.
	Wire.requestFrom(MPU6050_ADDRESS, 14, true);   	//Requests 14 bytes of data starting from the indicated register. 6 for accelerometer, 2 for temp, 6 for gyro

	this->acc_x = Wire.read() <<8| Wire.read();     //the data for each axis of the accelerometer or gyro is 2 bytes, one high byte and one low
	this->acc_y = Wire.read() <<8| Wire.read();     //the two must be added together to get the raw sensor value
	this->acc_z = Wire.read() <<8| Wire.read();
	this->temperature = Wire.read() <<8| Wire.read();
	this->gyro_x = Wire.read() <<8| Wire.read();
	this->gyro_y = Wire.read() <<8| Wire.read();
	this->gyro_z = Wire.read() <<8| Wire.read();
}

/*
 *
 */
void MPU6050::update(){
	updateGyroValues();

	if(gyro_x_cal >= 0) gyro_x -= gyro_x_cal;	//Subtract the offset calibration value from the raw gyro_x value.
	if(gyro_x_cal <= 0) gyro_x -= gyro_x_cal;
	if(gyro_y_cal >= 0) gyro_y -= gyro_y_cal;	//Subtract the offset calibration value from the raw gyro_y value.
	if(gyro_y_cal <= 0) gyro_y -= gyro_y_cal;
	if(gyro_z_cal >= 0) gyro_z -= gyro_z_cal;	//Subtract the offset calibration value from the raw gyro_z value.
	if(gyro_z_cal <= 0) gyro_z -= gyro_z_cal;

	this->roll += gyro_x / gyroLSB * dt;	//Integrates the angular rate of X axis over dt to return absolute position of X axis.
	this->pitch += gyro_y / gyroLSB * dt;	//Integrates the angular rate of Y axis over dt to return absolute position of Y axis.
	this->yaw += gyro_z / gyroLSB * dt;		//Integrates the angular rate of Z axis over dt to return absolute position of Z axis.

	accXg = acc_x / accelLSB;	//Converts raw accelerometer values into g's by dividing by the least significant bit.
	accYg = acc_y / accelLSB;	//Converts raw accelerometer values into g's by dividing by the least significant bit.
	accZg = acc_z / accelLSB;	//Converts raw accelerometer values into g's by dividing by the least significant bit.

	complementaryFilter();
}

/*
 * Calibrates Gyro to remove biases from X, Y, and Z axes.
 */
void MPU6050::gyroCalibrateOnce() {

  Serial.println(F("Gyro calibrating, don't move it!!!"));

  for(int cal_int = 0; cal_int < calibrationSamples; cal_int++){
	  if(cal_int % 125 == 0)
		  Serial.print(".");
	  updateGyroValues();
	  gyro_x_cal += gyro_x;
	  gyro_y_cal += gyro_y;
	  gyro_z_cal += gyro_z;
	  delay(3);
  }

  gyro_x_cal /= calibrationSamples;
  gyro_y_cal /= calibrationSamples;
  gyro_z_cal /= calibrationSamples;

  Serial.println(gyro_x_cal); 				//Check to see if calibration was successfull. Value should be very close to 0.
  Serial.println(gyro_y_cal); 				//Check to see if calibration was successfull. Value should be very close to 0.
  Serial.println(gyro_z_cal); 				//Check to see if calibration was successfull. Value should be very close to 0.

  delay(2000); 	//DELETE WHEN DONE!!! Delay to allow for previous prints to be read. DELETE WHEN DONE!!!

  Serial.println(F("Done Calibrating!"));	//Alerts that the Gyro has completed it's calibration routine.
}

/*
 * This is an iterative function that adds an additional sample to a new calibration sample buffer but doesnt update the
 * gyro cal values until samples have been taken.
 */
void MPU6050::recalibrateGyro() {

	updateGyroValues();

	gyro_x_reCal += gyro_x;
	gyro_y_reCal += gyro_y;
	gyro_z_reCal += gyro_z;

	if(calibrationIndex >= calibrationSamples){

		gyro_x_cal = gyro_x_reCal / calibrationSamples;
		gyro_y_cal = gyro_y_reCal / calibrationSamples;
		gyro_z_cal = gyro_z_reCal / calibrationSamples;

		gyro_x_reCal = 0;
		gyro_y_reCal = 0;
		gyro_z_reCal = 0;

		Serial.println(gyro_x_cal);
		Serial.println(gyro_y_cal);
		Serial.println(gyro_z_cal);

		calibrationIndex = 0;
	} else {
		calibrationIndex++;
	}
}

/*
 * An iterative function that filters the roll and pitch for every time the data updates
 */
void MPU6050::complementaryFilter() {
	filteredRoll += gyro_x / gyroLSB * dt;	//Integrates the angular rate of X axis over dt to return absolute position of X axis.
	filteredPitch += gyro_y / gyroLSB * dt;	//Integrates the angular rate of Y axis over dt to return absolute position of Y axis.


	totalAccelVector = sqrt((accXg*accXg)+(accYg*accYg)+(accZg*accZg));  	//Calculates the total accelerometer vector.

	anglePitchAccel = asin((float)accYg/totalAccelVector) * degToRad;       //Calculates the pitch angle.
	angleRollAccel = asin((float)accXg/totalAccelVector) * -degToRad;       //Calculates the roll angle.

	if (totalAccelVector < 2) {
		filteredPitch = filteredPitch * 0.98 + anglePitchAccel * 0.02;
		filteredRoll = filteredRoll * 0.98 + angleRollAccel * 0.02;
	}

	//Serial.println("Filtered values: ");
	//Serial.println(filteredPitch);
	//Serial.println(filteredRoll);


}

/*
 * Returns the current Roll which is the absolute position of the X axis.
 */
float MPU6050::getRoll(){
	return this->roll;
}

/*
 * Returns the current Pitch which is the absolute position of the Y axis.
 */
float MPU6050::getPitch(){
	return this->pitch;
}

/*
 * Returns the current Yaw which is the absolute position of the Z axis.
 */
float MPU6050::getYaw(){
	return this->yaw;
}

/*
 * Returns the current Acceleration value in the x direction in Gs.
 */
float MPU6050::getAccXg(){
	return this->accXg;
}

/*
 * Returns the current Acceleration value in the y direction in Gs.
 */
float MPU6050::getAccYg(){
	return this->accYg;
}

/*
 * Returns the current Acceleration value in the z direction in Gs.
 */
float MPU6050::getAccZg(){
	return this->accZg;
}

/*
 * Returns the current temperature value in deg C.
 */
int16_t MPU6050::getRawTemperature(){
	return this->temperature;
}

/*
 * Returns the current Gyro orientation in the x direction in deg/s.
 */
int16_t MPU6050::getRawGyroX(){
	return this->gyro_x;
}

/*
 * Returns the current Gyro orientation in the y direction in deg/s.
 */
int16_t MPU6050::getRawGyroY(){
	return this->gyro_y;
}

/*
 * Returns the current Gyro orientation in the z direction in deg/s.
 */
int16_t MPU6050::getRawGyroZ(){
	return this->gyro_z;
}

/*
 * Returns the total Acceleration Vector Raw value.
 */
float MPU6050::getTotalAccelVector(){
	return this->totalAccelVector;
}

/*
 * Returns the pitch calculated using the complementary filter
 */
float MPU6050::getFilteredPitch() {
	return this->filteredPitch;
}

/*
 * Returns the roll calculated using the complementary filter
 */
float MPU6050::getFilteredRoll() {
	return this->filteredRoll;
}

