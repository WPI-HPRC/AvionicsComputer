


#ifndef MPL3115A2_h
#define MPL3115A2_h

#define MPL3115A2_ADDRESS (0x60) //Default I2C address of sensor
#define MPL3115A2_WHOAMI  (0x0C)//WhoAmI register of barometer
#define MPL3115A2_REGISTER_PRESSURE_MSB (0x01)//Pressure register MSB
#define MPL3115A2_CTRL_REG1 (0x26)

#include "Arduino.h"
#include "PeripheralInterface.h"

/*
 * Class for reading altitude and pressure from the MPL3115A2 barometer
 * Uses i2c interface
 *
 */
class MPL3115A2 : public PeripheralInterface {

private:
    float pressure;			//float storing the last pressure reading taken by the sensor
    float altitude;			//float storing the last altitude reading taken by the sensor
    float maxAltitudeReading;//float storing the maximum altitude reading ever taken by the sensor
    float zeroAltitude;     //float storing the baseline altitude reading at ground level, used to calculate altitude from ground
    float readPressure();	//function for taking a pressure reading
    float readAltitude();  //function for taking an altitude reading
    bool checkLaunched();  //function for checking if current altitude is above ground altitude
    bool checkApogee();    //function for checking if current altitude is below max altitude
    uint8_t read8(uint8_t address); //function for reading from a register in the sensor over i2c

public:
    MPL3115A2();
    boolean initBaro();   //function for initializing the barometer and checking if i2c communication is working
    void setZeroAltitude();//function for setting the current altitude as ground level
    void enable();		//function for enabling and intitalizing barometer
    void update();		//update function for updating pressure and altitude readings
    void disable();
    float getPressure();//function for getting last pressure reading
    float getAltitude();//function for getting last altitude reading, relative to ground level altitude
    bool hasLaunched;	//bool for checking if current altitude is above ground altitude
    bool reachedApogee;//bool for checking if current altitude is below max altitude
};

#endif
