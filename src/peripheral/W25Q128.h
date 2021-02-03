/*
 * W25Q128.h
 *
 *  Created on: Feb 3, 2021
 *      Author: Christian Max Schrader
 */

#ifndef SRC_PERIPHERAL_W25Q128_H_
#define SRC_PERIPHERAL_W25Q128_H_

#include "Arduino.h"
#include "SerialFlash.h"
#include "SPI.h"
#include "PeripheralInterface.h"
#include "math.h"

typedef union data32 {
	float f;
	int32_t i;
	char c[4];
};

typedef struct avionicsPacket {
	char id[4] = "AVI";
	char version = 0x01;
	int32_t timeStamp = -1;

	// Sensor Readings
	float altitude = NAN;
	float linearAcc[3] = {NAN, NAN, NAN};
	float rotationalAcc[3] = {NAN, NAN, NAN};
	float magneticField[3] = {NAN, NAN, NAN};

	// Estimated State
	float relativePosition[3] = {NAN, NAN, NAN};	// Position relative to launch pad.
	float velocity[3] = {NAN, NAN, NAN};
	float attitude[4] = {NAN, NAN, NAN};			// Attitude quaternion.

	// Other Telemetry
	float servoAngle = NAN;
};

class W25Q128 : public PeripheralInterface {
public:
	W25Q128();
	void enable();
	void disable();
	void update();

	void writePacket(char *buffer);
	struct avionicsPacket readPacket(char *buffer);

	bool ready();
	void eraseMemory();

private:
	void write32(char *buffer, int index, data32 data);
	data32 read32(char *buffer, int index);
};


#endif /* SRC_PERIPHERAL_W25Q128_H_ */
