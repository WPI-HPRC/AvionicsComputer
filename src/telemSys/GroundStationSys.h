//
// Created by james on 11/16/2019.
//

#ifndef USLI_20_GROUNDSTATIONSYS_H
#define USLI_20_GROUNDSTATIONSYS_H

#include "../peripheral/OutputSerial.h"
#include "../peripheral/LoRaCustom.h"
#include "../TelemSysInterface.h"

class GroundStationSys : public TelemSysInterface {
public:
    GroundStationSys(SysState::state state);
//    ~GroundStationSys(){}
    void loop();
    void parseMessage();
    void buildMessage();
    unsigned char cid = 0;

private:
    OutputSerial* outputSerial;
    LoRaComms* loRaComms;
};


#endif //USLI_20_GROUNDSTATIONSYS_H
