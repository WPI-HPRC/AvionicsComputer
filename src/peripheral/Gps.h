//
// Created by james on 11/16/2019.
//

#ifndef USLI_20_GPS_H
#define USLI_20_GPS_H

#include "../PeripheralInterface.h"

class Gps : public PeripheralInterface {
public:
    Gps(){}
//    ~Gps(){}
    void debug();
    void push(uint8_t* buf, uint8_t len);
    void pull(uint8_t* buf, uint8_t len);
};


#endif //USLI_20_GPS_H
