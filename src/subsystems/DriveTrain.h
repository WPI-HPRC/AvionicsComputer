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


//	GyroAccel * imu = new GyroAccel();		// sensor
//	uint8_t imuData[12];
//	LoRaComms * radio = new LoRaComms();


public:

	DriveTrain();
	//	~DriveTrain(){}

	/* Drivetrain loop functionality */
	class DriveLoop : public Loop {
		DriveTrain * driveTrain_;

	public:
		DriveLoop(DriveTrain * instance){
			driveTrain_ = instance;
		};

		void onStart(uint32_t timestamp){
			//
		}
		void onLoop(uint32_t timestamp){
			//


		}
		void onStop(uint32_t timestamp){
			//
		}
	} * driveLoop = new DriveLoop(this);	// instantiate the DriveTrain subystem loop and pass it the instance




};


#endif /* SRC_SUBSYSTEMS_DRIVETRAIN_H_ */
