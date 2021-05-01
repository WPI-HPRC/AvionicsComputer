#include "Arduino.h"
#include "MPL3115A2.h"
#include "Wire.h"

MPL3115A2::MPL3115A2(){
    this->pressure = 0;
    this->altitude = 0;
    this-> maxAltitudeReading = 0;
    this-> zeroAltitude = 0;
    this->reachedApogee = false;
    this->hasLaunched = false;
}

/*
 * Function for initializing the barometer
 * @return true if barometer was found on I2C bus
 */
boolean MPL3115A2::initBaro() {
    uint8_t whoami;// = read8(MPL3115A2_WHOAMI);
    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_WHOAMI);//Accessing WHOAMI register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
    whoami = Wire.read();//read the byte from barometer whoami


    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(0x13);//Accessing event flag generator register
    uint8_t eventFlags = B00000111;//sets flag register to detect new pressure and alt data
    Wire.write(eventFlags);
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus


    return whoami == 0xC4;
}
void MPL3115A2::enable(){
	initBaro();

}
void MPL3115A2::disable(){

}

/*
 * Function for reading the current pressure
 * @return float containing pressure in Pascals
 */
float MPL3115A2::readPressure() {

	while (read8(MPL3115A2_CTRL_REG1) & 0x02)//read control register 1 (0x26) OST bit (0x02) to check if measurment is ready to be made, and delays if not
	    delay(10);

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_CTRL_REG1);//Accessing Control register 1 of barometer
    uint8_t ctrl_reg1 = B00000010;//sets control register to immediately read barometer
    Wire.write(ctrl_reg1);
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus



    uint8_t sta = 0;
     while (!(sta & 0x04)){//checks bit 2 of status register(0x00), bit 2 indicates new pressure data is avialable, delays otherwise
       sta = read8(0);
       //Serial.println(sta);
       delay(10);
     }


    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer
    uint32_t pressure = Wire.read();
    pressure <<= 8;				//format pressure from bytes read
    pressure |= Wire.read();
    pressure <<= 8;
    pressure |= Wire.read();
    pressure >>= 4;
    float reading = pressure;
    return reading / 4.0;
}

/*
 * Function for reading the current altitude
 * @return float containing altitude in meters
 */
float MPL3115A2::readAltitude() {


	while (read8(MPL3115A2_CTRL_REG1) & 0x02)//read control register 1 (0x26) OST bit (0x02) to check if measurment is ready to be made, and delays if not
	    delay(10);


    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_CTRL_REG1);//Accessing Control register 1 of barometer
    uint8_t ctrl_reg1 = B10000010;//sets control register to immediately read barometer in altitude mode
    Wire.write(ctrl_reg1);
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus


    uint8_t sta = 0;
      while (!(sta & 0x04)){//checks bit 2 of status register(0x00), bit 2 indicates new pressure data is avialable, delays otherwise
        sta = read8(0);
        //Serial.println(sta);
        delay(10);
      }


    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer
    uint32_t alt = Wire.read();
    alt <<= 8;				//format altitude data
    alt |= Wire.read();
    alt <<= 8;
    alt |= Wire.read();

    float reading = alt;
    return reading / 256.0;
}

/*
 * Function for setting the zero altitude
 * Takes average of 4 altitude readings and sets zeroAltitude to the result
 * @return void
 */
void MPL3115A2::setZeroAltitude(){
	float altAvg = this->readAltitude();
	for(int i = 0;i<3;i++){
		altAvg += this->readAltitude();
	}

	this->zeroAltitude = altAvg/4;
}

/*
 * Function for updating the altitude and pressure readings
 * also checks for launch and apogee
 * @return void
 */
void MPL3115A2::update() {
    this->pressure = readPressure();
    this->altitude = readAltitude();

    if(this->altitude > this->maxAltitudeReading){
       	this->maxAltitudeReading = this->altitude;//sets maximum altitude if current is greater than max
    }

    if(!this->reachedApogee){
    this->reachedApogee = checkApogee();//checks for apogee if it hasnt been reached yet
    }

    if(!this->hasLaunched){
        this->hasLaunched = checkLaunched();//checks for launch if it hasnt happened yet
    }

}

/*
 * Function for checking if apogee has been reached
 * @return true if current alt is significantly less than maximum alt
 */
bool MPL3115A2::checkApogee(){
	return (this->altitude < this->maxAltitudeReading - 5);
}

/*
 * Function for checking if rocket has launched
 * @return true if alt is greater than 5m
 */
bool MPL3115A2::checkLaunched(){
	return  getAltitude() > 5;
}

/*
 * function for publicly accessing the last pressure reading
 * @return float containing last pressure reading
 */
float MPL3115A2::getPressure() {
    return this->pressure;
}

/*
 * function for publicly accessing the last altitude reading relative to ground
 * @return float containing last altitude reading
 */
float MPL3115A2::getAltitude() {
    return this->altitude - this->zeroAltitude;
}




/*
 * function for reading 1 byte from a general register on the barometer using i2c
 * @param uint8_t address: the address of the register to read from
 * @return uint8_t containing the byte from the address
 */
uint8_t MPL3115A2::read8(uint8_t address) {
    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(address);//Accessing input register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
    return Wire.read();//return read byte
}


