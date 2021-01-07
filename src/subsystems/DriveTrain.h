/*
 * DriveTrain.h
 * Created on: Dec 28, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_SUBSYSTEMS_DRIVETRAIN_H_
#define SRC_SUBSYSTEMS_DRIVETRAIN_H_

#include "SubsystemInterface.h"
#include "../../Constants.h"

#include "../peripheral/LoRaCustom.h"
#include "../peripheral/GyroAccel.h"
#include "../peripheral/MPU6050.h"


/*
 *
 */
enum DriveControlState {
	Idle,
	OpenLoop,
	DriveStraight,
	TurnToHeading,
	PathFollowing

};

/*
 *
 */
class DriveTrain : public SubsystemInterface {

private:

	DriveControlState driveControlState;


	uint8_t imuData[12];
//	GyroAccel * imu;
	MPU6050 * imu;

//	LoRaComms * radio;


public:

	DriveTrain(MPU6050 * imuSensor);
	//	~DriveTrain(){}

	/* Drivetrain loop functionality */
	class DriveLoop : public Loop {
		DriveTrain * drive_;

	public:
		DriveLoop(DriveTrain * instance){
			drive_ = instance;
		};

		void onStart(uint32_t timestamp){
			//
			Serial.println(F("START DRIVETRAIN LOOP"));
			drive_->imu->gyroCalibrateOnce();
		}
		void onLoop(uint32_t timestamp){
			//
			drive_->imu->recalibrateGyro();
			//drive_->imu->update();

			//Serial.println(timestamp);
			//driveTrain_->printOutput();

		}
		void onStop(uint32_t timestamp){
			//
		}
	} * driveLoop = new DriveLoop(this);	// instantiate the DriveTrain subsystem loop and pass it the instance



	void subsystemInit();

//	double getHeading();
	double getHeading();


	void zeroSensors();
	void registerEnabledLoops(Looper * enabledLooper);
	void printOutput();



};


#endif /* SRC_SUBSYSTEMS_DRIVETRAIN_H_ */