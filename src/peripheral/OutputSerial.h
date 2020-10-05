//
// Created by james on 11/16/2019.
//

#ifndef USLI_20_OUTPUTSERIAL_H
#define USLI_20_OUTPUTSERIAL_H

#include "../PeripheralInterface.h"

class OutputSerial : public PeripheralInterface {
public:
    OutputSerial(){}
    ~OutputSerial(){}
    void debug();
    void push(uint8_t* buf, uint8_t len);
    void pull(uint8_t* buf, uint8_t len);
};


#endif //USLI_20_OUTPUTSERIAL_H
