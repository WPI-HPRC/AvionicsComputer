//
// Created by james on 11/9/2019.
//
#include "Arduino.h"

#ifndef USLI20_PERIPHERAL_H
#define USLI20_PERIPHERAL_H
class PeripheralInterface {
public:
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual void update() = 0;
};
#endif //SRC_PERIPHERAL_H
