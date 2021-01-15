#include "Arduino.h"
#include "MPL3115A2.h"
#include "Wire.h"

Baro_mpl3115A2::Baro_mpl3115A2(){
    this->pressure = 0;
    this->alt = 0;
}
boolean Baro_mpl3115A2::initBaro() {
    uint8_t whoami;// = read8(MPL3115A2_WHOAMI);
    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(MPL3115A2_WHOAMI);//Accessing WHOAMI register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
    whoami = Wire.read();//read the byte from barometer whoami
    return whoami == 0xC4;
}
void Baro_mpl3115A2::enable(){

}
void Baro_mpl3115A2::disable(){

}

//returns pressure in Pa
float Baro_mpl3115A2::readPressure() {

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

float Baro_mpl3115A2::readAltitude() {

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

void Baro_mpl3115A2::update() {
    this->pressure = readPressure();
    this->alt = readAltitude();
}

float Baro_mpl3115A2::getPressure() {
    return this->pressure;
}
float Baro_mpl3115A2::getAltitude() {
    return this->alt;
}



uint8_t Baro_mpl3115A2::read8(uint8_t address) {
    Wire.beginTransmission(MPL3115A2_ADDRESS);//Beginning transmission with barometer
    Wire.write(address);//Accessing input register of barometer
    Wire.endTransmission(false);//Ending transmission, no longer writing to bus
    Wire.requestFrom(MPL3115A2_ADDRESS, 1, false);//Request one byte from barometer
    return Wire.read();//return read byte
}


