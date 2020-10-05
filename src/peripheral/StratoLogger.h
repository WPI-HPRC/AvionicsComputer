
#ifndef USLI_20_STRATOLOGGER_H
#define USLI_20_STRATOLOGGER_H

#include "../PeripheralInterface.h"

#include <HardwareSerial.h>		// For debug mode printing
#include <SoftwareSerial.h>		// For one-way comms with Strato


#define STRATOLOGGER_DATA_SIZE 2	// altitude reading is uint16_t (two bytes)
#define STRATOLOGGER_BAUD_RATE 9600

#define stratoTxPin 7	// This pin goes to StratoLogger
#define stratoRxPin 10	// This pin goes unused, we don't send to the Strato

#define ASCII_CR 13		// ASCII char CR
#define ASCII_LF 10		// ASCII char LF
#define ASCII_dash 45 	// ASCII char -
#define ASCII_0 48		// ASCII char 0
#define ASCII_1 49		// ASCII char 1
#define ASCII_2 50		// ASCII char 2
#define ASCII_3 51		// ASCII char 3
#define ASCII_4 52		// ASCII char 4
#define ASCII_5 53		// ASCII char 5
#define ASCII_6 54		// ASCII char 6
#define ASCII_7 55		// ASCII char 7
#define ASCII_8 56		// ASCII char 8
#define ASCII_9 57		// ASCII char 9


class StratoLogger : public PeripheralInterface {

private:
	SoftwareSerial stratoLogger = SoftwareSerial(stratoTxPin, stratoRxPin);

	bool validSerialMsg;
	uint16_t readingsCount;
	uint8_t serialBuffAvail;

	uint8_t currentAltReading[STRATOLOGGER_DATA_SIZE];
	uint8_t lastSerialBuff[8] = {0, 0, 0, 0, 0, 0, 0, 0};	// 8 byte messages received from StratoLogger

	bool bad;

public:
    StratoLogger();
//    ~StratoLogger(){}

    void debug();
    void push(uint8_t* buf, uint8_t len);
    void pull(uint8_t* buf, uint8_t len);

    void pull(uint8_t* buf);
    uint8_t parseStratoLoggerChar(uint8_t serialRead);
    bool readForAltitude();
    uint8_t asciiToDeci(uint8_t asciiNum);
    uint16_t getReadingsCount();
    int getSerialBuffAvail();
    int serAvailVal();
    void resetSerialBuff();
    bool getIsValidSerialMsg();

    void copylastSerialBuff(uint8_t* buf);	// function for debugging, left in

};
#endif //USLI_20_STRATOLOGGER_H
