


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
    float pressure;
    float altitude;
    float maxAltitudeReading;
    float zeroAltitude;
    float readPressure();
    float readAltitude();
    bool checkLaunched();
    bool checkApogee();
    uint8_t read8(uint8_t address);

public:
    MPL3115A2();
    boolean initBaro();
    void setZeroAltitude();
    void enable();
    void update();
    void disable();
    float getPressure();
    float getAltitude();
    bool hasLaunched;
    bool reachedApogee;
};

#endif
