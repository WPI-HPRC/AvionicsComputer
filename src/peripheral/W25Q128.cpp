/*
 * W35Q128.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: Christian Max Schrader
 */

#include <W25Q128.h>

void W25Q128::enable()
{
	SerialFlash.begin(10);	// TODO: Replace pin with const FLASHCHIPSELECT
}

void W25Q128::update()
{
	return;
}

/**
 * TODO: Description
 * Note, this function will block code execution if the chip
 * is not ready to be accessed.
 */
void W25Q128::writeAvionicsPacket(char *buffer, struct avionicsPacket packet)
{
	while(!ready());
	data32 dat;
	strncpy(buffer, packet.id, 3);
	int index = 3;
	buffer[index++] = packet.version;
	dat.i = packet.timeStamp;
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.altitude;
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.linearAcc[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.linearAcc[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.linearAcc[2];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.rotationalAcc[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.rotationalAcc[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.rotationalAcc[2];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.magneticField[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.magneticField[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.magneticField[2];
	this->write32(buffer, index, dat);
	index += 4;


	dat.f = packet.relativePosition[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.relativePosition[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.relativePosition[2];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.velocity[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.velocity[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.velocity[2];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.attitude[0];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.attitude[1];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.attitude[2];
	this->write32(buffer, index, dat);
	index += 4;
	dat.f = packet.attitude[3];
	this->write32(buffer, index, dat);
	index += 4;

	dat.f = packet.servoAngle;
	this->write32(buffer, index, dat);
	index += 4;
}

/**
 * TODO: Description
 * Note, this function will block code execution if the chip
 * is not ready to be accessed.
 */
struct avionicsPacket W25Q128::readAvionicsPacket(char *buffer)
{
	union avionicsPacket packet;
	return packet;
}

/**
 * Writes 32 bits of data to a char buffer array.  Intended for
 * converting floats and int32_ts to binary.
 *
 * @param	buffer	Char array to copy to.  It must have space for
 * 					the data.
 * @param	index	Index to start copying data to.  Must 4 or
 * 					less than the size of the buffer array.
 * @param	data	32 bits of data to write.  Given as a union.
 */
void W25Q128::write32(char *buffer, int index, data32 data)
{
	for (int i = 0; i < 4; ++i)
	{
		buffer[index + i] = data.c[i];
	}
}

/**
 * Reads 32 bits of data from a char array and combines them into
 * a single 32 bit union.  It is up to the function calling code
 * to interpret the union as a float or int32_t.
 *
 * @param	buffer	Char array to read from.
 * @param	index	Index to start reading data from.  Must 4 or
 * 					less than the size of the buffer array.
 * @return	Data read as union.
 */
data32 W25Q128::read32(char *buffer, int index)
{
	union data32 data;
	for (int i = 0; i < 4; ++i)
	{
		data.c[i] = buffer[index + i];
	}
	return data;
}

/**
 * Determines whether or not the flash chip is ready for
 * a new command.
 */
bool W25Q128::ready()
{
	return SerialFlash.ready();
}

/**
 * Erases all memory on the chip.  Note that this takes around
 * 40 seconds.  While this is nonblocking, this will prevent
 * the chip from operating for the time.
 */
void W25Q128::eraseMemory()
{
	SerialFlash.eraseAll();
}

