//
// Created by james on 11/16/2019.
//

#include "RetentionSys.h"

RetentionSys::RetentionSys(SysState::state state) : TelemSysInterface(state) {
    gps->debug();
    loRaComms->debug();
}

void RetentionSys::loop() {
    // TODO Write this
    debugLoop();
}

void RetentionSys::parseMessage() {
    // TODO Write this
}

void RetentionSys::buildMessage() {
    // TODO Write this
}