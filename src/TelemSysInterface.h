//
// Created by james on 11/9/2019.
//

#ifndef USLI20_TELEMSYS_H
#define USLI20_TELEMSYS_H

#include <HardwareSerial.h>

namespace SysState {
    enum state {
        SPAM,        // send altitude and acceleration data
        LOWSPAM,    // send altitude, acceleration, and gps data
        GPSONLY,     // send gps data
        GPSLISTEN,   // send gps then listen for command
        HEARTBEAT,   // send empty message
        OFF,         // don't send
    };
}


class TelemSysInterface {
public:
    TelemSysInterface(SysState::state sysState) {state = sysState;}
//    virtual ~TelemSysInterface(){}
    virtual void loop() = 0;
    virtual void parseMessage() = 0;
    virtual void buildMessage() = 0;
    unsigned char cid;  // communication id
    SysState::state state;
    void debugLoop() {
        switch (state) {
            case SysState::SPAM: {
                Serial.println("DEBUG SysState::SPAM");
                break;
            }
            case SysState::LOWSPAM: {
                Serial.println("DEBUG SysState::LOWSPAM");
                break;
            }
            case SysState::GPSONLY: {
                Serial.println("DEBUG SysState::GPSONLY");
                break;
            }
            case SysState::GPSLISTEN: {
                Serial.println("DEBUG SysState::GPSLISTEN");
                break;
            }
            case SysState::HEARTBEAT: {
                Serial.println("DEBUG SysState::HEARTBEAT");
                break;
            }
            case SysState::OFF: {
                Serial.println("DEBUG SysState::OFF");
                break;
            }
            default: {
                // error
                Serial.println("DEBUG SysState Error: Invalid state.");
            }
        }
    }
};
#endif //USLI20_TELEMSYS_H
