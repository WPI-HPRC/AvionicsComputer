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
#include "../peripheral/PIDMotor.h"


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


	uint8_t leftMotorPWM  = LEFT_MOTOR_PWM_PIN;
	uint8_t rightMotorPWM = RIGHT_MOTOR_PWM_PIN;

	uint8_t leftMotorDir1  = LEFT_MOTOR_DIR_PIN_1;
	uint8_t leftMotorDir2  = LEFT_MOTOR_DIR_PIN_2;
	uint8_t rightMotorDir1 = RIGHT_MOTOR_DIR_PIN_1;
	uint8_t rightMotorDir2 = RIGHT_MOTOR_DIR_PIN_2;

	uint8_t leftEncoderA = LEFT_ENCODER_A_PIN;
	uint8_t leftEncoderB = LEFT_ENCODER_B_PIN;
	uint8_t rightEncoderA = RIGHT_ENCODER_A_PIN;
	uint8_t rightEncoderB = RIGHT_ENCODER_B_PIN

	uint8_t leftCurSense  = LEFT_MOTOR_CUR_PIN;
	uint8_t rightCurSense = RIGHT_MOTOR_CUR_PIN;



	MPU6050 * imu;

	PIDMotor * leftMotor = new PIDMotor(leftMotorPWM, leftMotorDir1, leftMotorDir2, leftCurSense, leftEncoderA, leftEncoderB);
	PIDMotor * rightMotor = new PIDMotor(rightMotorPWM, rightMotorDir1, rightMotorDir2, rightCurSense, rightEncoderA, rightEncoderB);
	//	PIDMotor * leftMotor;			 	// motor driving left side robot tread
	//	PIDMotor * rightMotor;				// motor driving right side robot tread




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
		}
		void onLoop(uint32_t timestamp){

			drive_->leftMotor->update();
			drive_->rightMotor->update();

			uint16_t potVal = analogRead(A0);

			float leftMotorSpeed = drive_->leftMotor->fMap(potVal, 0.0, 255.0, -1.0, 1.0);
			float rightMotorSpeed = drive_->rightMotor->fMap(potVal, 0.0, 255.0, -1.0, 1.0);

			drive_->leftMotor->setSpeed(leftMotorSpeed);
			drive_->rightMotor->setSpeed(rightMotorSpeed);

			int32_t leftMotorPosition = drive_->leftMotor->getPosition();
			int32_t rightMotorPosition = drive_->rightMotor->getPosition();

			Serial.print(leftMotorPosition); Serial.print(F(", "));
			Serial.println(rightMotorPosition);

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
	int16_t getHeading();


	void zeroSensors();
	void registerEnabledLoops(Looper * enabledLooper);
	void printOutput();



};


#endif /* SRC_SUBSYSTEMS_DRIVETRAIN_H_ */
