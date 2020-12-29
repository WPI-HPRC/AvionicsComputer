/*
 * PeripheralInterface.h
 * Created on: Dec 22, 2020
 * Author: Max Schrader
 *
 * Interface structure for a peripheral
 *
 */

#ifndef SRC_PERIPHERAL_PERIPHERALINTERFACE_H_
#define SRC_PERIPHERAL_PERIPHERALINTERFACE_H_

#include "Arduino.h"


class PeripheralInterface {
public:
    PeripheralInterface(){}
//    virtual ~PeripheralInterface(){}

    virtual void push(uint8_t* buf, uint8_t len) = 0;
    virtual void pull(uint8_t* buf, uint8_t len) = 0;

//    virtual void enable() = 0;
//    virtual void disable() = 0;
//    virtual void update() = 0;

};


#endif /* SRC_PERIPHERAL_PERIPHERALINTERFACE_H_ */
