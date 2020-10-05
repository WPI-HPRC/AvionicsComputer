//
// Created by james on 11/16/2019.
//

#ifndef USLI_20_EBAYSYS_H
#define USLI_20_EBAYSYS_H

#include "../peripheral/LoRaCustom.h"
#include "../peripheral/Gps.h"
#include "../peripheral/StratoLogger.h"
#include "../peripheral/GyroAccel.h"
#include "../peripheral/DataLogger.h"
#include "../TelemSysInterface.h"

class EBaySys : public TelemSysInterface {
public:
    EBaySys(SysState::state state);
//    ~EBaySys(){}
    void loop();
    void parseMessage();
    void buildMessage();
    unsigned char cid = 1;

private:
    LoRaComms loRaComms;
    Gps gps;
    StratoLogger stratoLogger;
    GyroAccel gyroAccel;
    DataLogger dataLogger;


};


#endif //USLI_20_EBAYSYS_H
