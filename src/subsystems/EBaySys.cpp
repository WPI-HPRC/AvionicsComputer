//
// Created by james on 11/16/2019.
//

#include "EBaySys.h"
EBaySys::EBaySys(SysState::state state) : TelemSysInterface(state) {
    loRaComms.debug();
    gps.debug();
    stratoLogger.debug();
    gyroAccel.debug();
    dataLogger.debug();

}

void EBaySys::loop() {
    // TODO Write this
    debugLoop();
}

void EBaySys::parseMessage() {
    // TODO Write this
}

void EBaySys::buildMessage() {
    // TODO Write this
}