//
// Created by james on 11/16/2019.
//

#include "GroundStationSys.h"

GroundStationSys::GroundStationSys(SysState::state state) : TelemSysInterface(state) {
    outputSerial->debug();
    loRaComms->debug();
}

void GroundStationSys::loop() {
    // TODO Write this
    debugLoop();
}

void GroundStationSys::parseMessage() {
    // TODO Write this
}

void GroundStationSys::buildMessage() {
    // TODO Write this
}