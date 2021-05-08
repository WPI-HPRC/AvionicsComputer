/*
 * SelfRighting.h
 *
 *  Created on: May 8, 2021
 *      Author: abbyg
 */

#ifndef SRC_SUBSYSTEMS_SELFRIGHTING_H_
#define SRC_SUBSYSTEMS_SELFRIGHTING_H_

#include "SubsystemInterface.h"
#include "../../Constants.h"

#include "../peripheral/LoRaCustom.h"
#include "../peripheral/GyroAccel.h"
#include "../peripheral/MPU6050.h"
#include "../peripheral/PIDMotor.h"

class SelfRighting : public SubsystemInterface {
private:
//	DriveControlState driveControlState;

	uint8_t motor1PWM  = LEFT_MOTOR_PWM_PIN;
//	uint8_t rightMotorPWM = RIGHT_MOTOR_PWM_PIN;

	uint8_t motor1Dir1  = LEFT_MOTOR_DIR_PIN_1;
	uint8_t motor1Dir2  = LEFT_MOTOR_DIR_PIN_2;
//	uint8_t rightMotorDir1 = RIGHT_MOTOR_DIR_PIN_1;
//	uint8_t rightMotorDir2 = RIGHT_MOTOR_DIR_PIN_2;

	uint8_t motor1EncoderA = LEFT_ENCODER_A_PIN;
	uint8_t motor1EncoderB = LEFT_ENCODER_B_PIN;
//	uint8_t rightEncoderA = RIGHT_ENCODER_A_PIN;
//	uint8_t rightEncoderB = RIGHT_ENCODER_B_PIN;

	uint8_t motor1CurSense  = LEFT_MOTOR_CUR_PIN;
	uint8_t motor1Pot = PETAL_1_POT_PIN;
//	uint8_t rightCurSense = RIGHT_MOTOR_CUR_PIN;

	MPU6050 * imu;

	PIDMotor * motor1 = new PIDMotor(motor1PWM, motor1Dir1, motor1Dir2, motor1CurSense, motor1Pot);
//	PIDMotor * rightMotor = new PIDMotor(rightMotorPWM, rightMotorDir1, rightMotorDir2, rightCurSense, rightEncoderA, rightEncoderB);

public:

	class SRLoop : public Loop {
		SelfRighting * selfRighting;

	public:
		SRLoop(SelfRighting * instance) {
			selfRighting = instance;
		}

		void onStart(uint32_t timestamp) {
			Serial.println(F("START SELF-RIGHTING LOOP"));
		}

		void onLoop(uint32_t timestamp) {
			uint16_t potValue = analogRead(selfRighting->motor1Pot);
			Serial.println(potValue);
		}

		void onStop(uint32_t timestamp) {
			//
		}


	} * srLoop = new SRLoop(this);

	SelfRighting(MPU6050 * imuSensor);
//	virtual ~SelfRighting();

	void subsystemInit();

	//	double getHeading();
	void zeroSensors();
	void registerEnabledLoops(Looper * enabledLooper);
	void printOutput();
};

#endif /* SRC_SUBSYSTEMS_SELFRIGHTING_H_ */
