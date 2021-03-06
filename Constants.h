/*
 * Constants.h
 * Created on: Dec 19, 2020
 * Author: Peter Dentch
 *
 * Storing all pin wiring and physical constants of the avionics computer board
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_


/* MAIN SYSTEM USE? */

//#define USE_AIRBRAKES_SYSTEM				// current system is the airbrakes computer
#define USE_PAYLOAD_ROBOT_SYSTEM			// current system is the payload robot
//#define USE_GROUND_STATION_SYSTEM			// current system is ground station



/* ROCKET CONSTANTS */

#define AIRBRAKES_NUM_LOOPS 1

// Electrical //


// Physical //



/* ROBOT CONSTANTS */

#define PAYLOAD_ROBOT_NUM_LOOPS 2

// Electrical //

#define LEFT_MOTOR_PWM_PIN  10; 	// TODO
#define RIGHT_MOTOR_PWM_PIN -1;     // TODO

#define LEFT_MOTOR_DIR_PIN  11; 	// TODO
#define RIGHT_MOTOR_DIR_PIN -1;     // TODO

#define LEFT_ENCODER_A_PIN 	 5; 	// TODO
#define LEFT_ENCODER_B_PIN 	 6;  	// TODO
#define RIGHT_ENCODER_A_PIN -1;  	// TODO
#define RIGHT_ENCODER_B_PIN -1;  	// TODO

#define LEFT_MOTOR_CUR_PIN  -1;     // TODO
#define RIGHT_MOTOR_CUR_PIN -1;     // TODO


// Physical //


// Tuned //



/* GROUND STATION CONSTANTS */

#define GROUND_STATION_NUM_LOOPS 1

// Electrical //





/* LOOPER CONSTANTS */

// Set to zero for loop as fast as possible
#define DT_LOOPER 0//0.01							// seconds, 10 milliseconds or 100 Hz

// Total required loops list space allocated to be run by Looper, different depending on the system running
#ifdef USE_PAYLOAD_ROBOT_SYSTEM
#define TOTAL_NUM_LOOPS PAYLOAD_ROBOT_NUM_LOOPS		// using robot system
#endif

#ifdef USE_AIRBRAKES_SYSTEM
#define TOTAL_NUM_LOOPS AIRBRAKES_NUM_LOOPS			// using airbrakes system
#endif

#ifdef USE_USE_GROUND_STATION_SYSTEM
#define TOTAL_NUM_LOOPS GROUND_STATION_NUM_LOOPS	// using ground station system
#endif



/*
 * COMM BUS CONSTANTS
 */

#define I2C_BUS_FREQUENCY 400000		// 400kHz(MPU6050 max)

#define SPI_SCK_FREQUENCY 10000000		// 10MHz (LoRa max)



/* DEBUG CONSTANTS */

//TODO #define USE_DEBUG

#define DEBUG_BAUD_RATE 115200
//2000000




#endif /* CONSTANTS_H_ */
