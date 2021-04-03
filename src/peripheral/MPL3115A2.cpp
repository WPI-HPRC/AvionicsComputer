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
    return whoami == 0xC4;
}
void MPL3115A2::enable(){
	initBaro();

}
void MPL3115A2::disable(){

}

/*
 * Function for reading the current pressure
 * @return pressure in Pascals
 */
float MPL3115A2::readPressure() {

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_CTRL_REG1);//Accessing Control register 1 of barometer
    uint8_t ctrl_reg1 = B00000010;//sets control register to immediately read barometer
    Wire.write(ctrl_reg1);
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer
    uint32_t pressure = Wire.read();
    pressure <<= 8;
    pressure |= Wire.read();
    pressure <<= 8;
    pressure |= Wire.read();
    pressure >>= 4;
    float reading = pressure;
    return reading / 4.0;
}

/*
 * Function for reading the current altitude
 * @return altitude in meters
 */
float MPL3115A2::readAltitude() {

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_CTRL_REG1);//Accessing Control register 1 of barometer
    uint8_t ctrl_reg1 = B10000010;//sets control register to immediately read barometer in altitude mode
    Wire.write(ctrl_reg1);
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus

    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB);//Accessing Pressure register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 3, false);//Request three bytes from barometer
    uint32_t alt = Wire.read();
    alt <<= 8;
    alt |= Wire.read();
    alt <<= 8;
    alt |= Wire.read();

    float reading = alt;
    return reading / 65536.0;
}

/*
 * Function for setting the zero altitude
 * @return void
 */
void MPL3115A2::setZeroAltitude(){
	this->zeroAltitude = this->readAltitude();
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


float MPL3115A2::getPressure() {
    return this->pressure;
}
float MPL3115A2::getAltitude() {
    return this->altitude - this->zeroAltitude;
}





/*
uint8_t Baro_mpl3115A2::read8(uint8_t address) {
    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(address);//Accessing input register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
    return Wire.read();//return read byte
}
*/

