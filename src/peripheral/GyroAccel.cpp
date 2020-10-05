//
// Created by james on 11/16/2019.
//

#include <HardwareSerial.h>
#include "GyroAccel.h"

void GyroAccel::debug() {
    // TODO Write this
    Serial.println("GyroAccel::debug");
}

void GyroAccel::push(uint8_t* buf, uint8_t len) {
    // TODO Write this
}

void GyroAccel::pull(uint8_t* buf, uint8_t len) {
    // TODO Write this

	getGyroValues();

	noInterrupts(); //ATOMIC_BLOCK_START;
	memcpy(buf, values, sizeof(values));
	interrupts(); //ATOMIC_BLOCK_END;
}


void GyroAccel::gyroStart(){
  Wire.beginTransmission(0x68);  //start a transmission with the gyro (address 68 for me, not 69)
  Wire.write(0x6B);              //register for power management
  Wire.write(0);                 //disable sleep mode, 'wake up' the device
  Wire.endTransmission(true);    //'true' means send a stop message, false doesn't do that. Leaving it blank sets it to true by default

  Wire.beginTransmission(0x68);  //start a transmission with the gyro
  Wire.write(0x1B);              //gyro config register
  Wire.write(B00001000);         //set the FS_SEL bits to 1 (binary: 01) meaning set gyro range to +- 500 deg/s
  Wire.endTransmission(true);

  Wire.beginTransmission(0x68);  //start a transmission with the gyro
  Wire.write(0x1C);              //accelerometer config register
  Wire.write(B00011000);         //set the AFS_SEL bits to 2 (binary: 10) meaning set accelerometer range to +- 8g
  Wire.endTransmission(true);

}


void GyroAccel::getGyroValues(){
  Wire.beginTransmission(0x68);       //start a transmission with the gyro
  Wire.write(0x3B);                   //begin with the ACCEL_XOUT_H register, the rest of the data registers begin there
  Wire.endTransmission(false);        //nothing else to write, but leave the bus commumincation open (false) so the next command can be run
  Wire.requestFrom(0x68, 14, true);   //request 14 bytes of data starting from the indicated register. 6 for accelerometer, 2 for temp, 6 for gyro

//  acc_x = Wire.read() <<8| Wire.read();         //the data for each axis of the accelerometer or gyro is 2 bytes, one high byte and one low
//  acc_y = Wire.read() <<8| Wire.read();         //the two must be added together to get the raw sensor value
//  acc_z = Wire.read() <<8| Wire.read();
//  temperature = Wire.read() <<8| Wire.read();
//  gyro_x = Wire.read() <<8| Wire.read();
//  gyro_y = Wire.read() <<8| Wire.read();
//  gyro_z = Wire.read() <<8| Wire.read();

  	values[0] = Wire.read();
  	values[1] = Wire.read();
  	values[2] = Wire.read();
  	values[3] = Wire.read();
  	values[4] = Wire.read();
  	values[5] = Wire.read();
  	temperature = Wire.read();
  	values[6] = Wire.read();
  	 values[7] = Wire.read();
  	  	values[8] = Wire.read();
  	  	values[9] = Wire.read();
  	  	values[10] = Wire.read();
  	  	values[11] = Wire.read();


}


void GyroAccel::gyroCalibrate() {
  Wire.begin();
  //Serial.begin(9600);
  gyroStart();

  Serial.println("Gyro calibrating, don't move it");
  for(int cal_int = 0; cal_int < 2000; cal_int++){
    if(cal_int % 125 == 0)
      Serial.print(".");
    getGyroValues();
    gyro_x_cal += gyro_x;
    gyro_y_cal += gyro_y;
    gyro_z_cal += gyro_z;
    delay(3);
  }
  gyro_x_cal /= 2000;
  gyro_y_cal /= 2000;
  gyro_z_cal /= 2000;

  Serial.println("starting");

}


