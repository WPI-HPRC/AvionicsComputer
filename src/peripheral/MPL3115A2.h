


#ifndef MPL3115A2_h
#define MPL3115A2_h

#define MPL3115A2_ADDRESS (0x60) //Default I2C address of sensor
#define MPL3115A2_WHOAMI  (0x0C)//WhoAmI register of barometer
#define MPL3115A2_REGISTER_PRESSURE_MSB (0x01)//Pressure register MSB
#define MPL3115A2_CTRL_REG1 (0x26)

#include "Arduino.h"
#include "PeripheralInterface.h"

class Baro_mpl3115A2 : public PeripheralInterface {

private:
    float pressure;
    float alt;
    float readPressure();
    float readAltitude();
    uint8_t read8(uint8_t address);

public:
    Baro_mpl3115A2();
    boolean initBaro();
    void enable();
    void update();
    void disable();
    float getPressure();
    float getAltitude();
};

#endif
