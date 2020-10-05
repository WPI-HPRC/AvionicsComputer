//
// Created by james on 11/9/2019.
//
#include "Arduino.h"

#ifndef USLI20_PERIPHERAL_H
#define USLI20_PERIPHERAL_H
class PeripheralInterface {
public:
    PeripheralInterface(){}
//    virtual ~PeripheralInterface(){}
    virtual void debug() = 0;
    virtual void push(uint8_t* buf, uint8_t len) = 0;
    virtual void pull(uint8_t* buf, uint8_t len) = 0;
};
#endif //USLI20_PERIPHERAL_H
