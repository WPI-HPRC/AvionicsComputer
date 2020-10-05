//
// Created by james on 11/16/2019.
//

#ifndef USLI_20_RETENTIONSYS_H
#define USLI_20_RETENTIONSYS_H

#include "../peripheral/Gps.h"
#include "../peripheral/LoRaCustom.h"
#include "../TelemSysInterface.h"

class RetentionSys : public TelemSysInterface {
public:
    RetentionSys(SysState::state state);
//    ~RetentionSys(){}
    void loop();
    void parseMessage();
    void buildMessage();
    unsigned char cid = 2;

private:
    LoRaComms* loRaComms;
    Gps* gps;
};


#endif //USLI_20_RETENTIONSYS_H
