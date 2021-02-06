/*
 * NEOM9.h
 *
 *  Created on: Jan 5, 2021
 *  	Author: Michael Beskid
 */

#ifndef SRC_PERIPHERAL_NEOM9_H_
#define SRC_PERIPHERAL_NEOM9_H_

#include "Arduino.h"
#include "Wire.h"
#include "PeripheralInterface.h"

#define NEOM9_ADDRESS 0x42		//Defines the GPS address.
#define MAX_PAYLOAD_SIZE 256
#define getPVTmaxWait 1100
#define defaultMaxWait 1100

typedef enum
{
	SFE_UBLOX_PACKET_VALIDITY_NOT_VALID,
	SFE_UBLOX_PACKET_VALIDITY_VALID,
	SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED,
	SFE_UBLOX_PACKET_NOTACKNOWLEDGED // This indicates that we received a NACK
} sfe_ublox_packet_validity_e;

typedef enum
{
	SFE_UBLOX_PACKET_PACKETCFG,
	SFE_UBLOX_PACKET_PACKETACK,
	SFE_UBLOX_PACKET_PACKETBUF
} sfe_ublox_packet_buffer_e;

typedef enum
{
	SFE_UBLOX_STATUS_SUCCESS,
	SFE_UBLOX_STATUS_FAIL,
	SFE_UBLOX_STATUS_CRC_FAIL,
	SFE_UBLOX_STATUS_TIMEOUT,
	SFE_UBLOX_STATUS_COMMAND_NACK, // Indicates that the command was unrecognised, invalid or that the module is too busy to respond
	SFE_UBLOX_STATUS_OUT_OF_RANGE,
	SFE_UBLOX_STATUS_INVALID_ARG,
	SFE_UBLOX_STATUS_INVALID_OPERATION,
	SFE_UBLOX_STATUS_MEM_ERR,
	SFE_UBLOX_STATUS_HW_ERR,
	SFE_UBLOX_STATUS_DATA_SENT,		// This indicates that a 'set' was successful
	SFE_UBLOX_STATUS_DATA_RECEIVED, // This indicates that a 'get' (poll) was successful
	SFE_UBLOX_STATUS_I2C_COMM_FAILURE,
	SFE_UBLOX_STATUS_DATA_OVERWRITTEN // This is an error - the data was valid but has been or _is being_ overwritten by another packet
} sfe_ublox_status_e;

typedef struct
{
	uint8_t cls;
	uint8_t id;
	uint16_t len;		   //Length of the payload. Does not include cls, id, or checksum bytes
	uint16_t counter;	   //Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
	uint16_t startingSpot; //The counter value needed to go past before we begin recording into payload array
	uint8_t *payload;
	uint8_t checksumA; //Given to us from module. Checked against the rolling calculated A/B checksums.
	uint8_t checksumB;
	sfe_ublox_packet_validity_e valid;			 //Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
	sfe_ublox_packet_validity_e classAndIDmatch; // Goes from NOT_DEFINED to VALID or NOT_VALID when the Class and ID match the requestedClass and requestedID
} ubxPacket;

class NEOM9 : public PeripheralInterface {

private:

	const uint8_t UBX_CLASS_NAV = 0x01;
	const uint8_t UBX_NAV_PVT = 0x07;
	const uint8_t UBX_CLASS_ACK = 0x05;
	const uint8_t UBX_NAV_DOP = 0x04;		//Dilution of precision
	const uint8_t UBX_NAV_HPPOSLLH = 0x14;	//High Precision Geodetic Position Solution. Used for obtaining lat/long/alt in high precision
	const uint8_t UBX_HNR_ATT = 0x01;			  //HNR Attitude
	const uint8_t UBX_HNR_INS = 0x02;			  //HNR Vehicle Dynamics
	const uint8_t UBX_HNR_PVT = 0x00;			  //HNR PVT
	const uint8_t UBX_ACK_NACK = 0x00;
	const uint8_t UBX_ACK_ACK = 0x01;
	const uint8_t UBX_SYNCH_1 = 0xB5;
	const uint8_t UBX_SYNCH_2 = 0x62;

	int32_t latitude;		 //Degrees * 10^-7 (more accurate than floats)
	int32_t longitude;		 //Degrees * 10^-7 (more accurate than floats)
	int32_t altitude;		 //Number of mm above ellipsoid

	uint8_t i2cTransactionSize = 32;
	boolean ignoreThisPayload = false;
	sfe_ublox_packet_buffer_e activePacketBuffer = SFE_UBLOX_PACKET_PACKETBUF;
	uint8_t requestedClass = UBX_CLASS_NAV;
	uint8_t requestedID = UBX_NAV_PVT;
	uint16_t ubxFrameCounter;
	uint16_t rtcmFrameCounter = 0; //Tracks the type of incoming byte inside RTCM frame
	uint8_t rollingChecksumA; //Rolls forward as we receive incoming bytes. Checked against the last two A/B checksum bytes
	uint8_t rollingChecksumB; //Rolls forward as we receive incoming bytes. Checked against the last two A/B checksum bytes
	sfe_ublox_status_e sendCommand(ubxPacket *outgoingUBX, uint16_t maxWait = defaultMaxWait);

	//The packet buffers
	//These are pointed at from within the ubxPacket
	uint8_t payloadAck[2];				  // Holds the requested ACK/NACK
	uint8_t payloadCfg[MAX_PAYLOAD_SIZE]; // Holds the requested data packet
	uint8_t payloadBuf[2];				  // Temporary buffer used to screen incoming packets or dump unrequested packets

	//Init the packet structures and init them with pointers to the payloadAck, payloadCfg and payloadBuf arrays
		ubxPacket packetAck = {0, 0, 0, 0, 0, payloadAck, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};
		ubxPacket packetCfg = {0, 0, 0, 0, 0, payloadCfg, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};
		ubxPacket packetBuf = {0, 0, 0, 0, 0, payloadBuf, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};

	enum SentenceTypes
		{
			NONE = 0,
			NMEA,
			UBX,
			RTCM
		} currentSentence = NONE;

	struct
		{
			uint32_t all : 1;
			uint32_t longitude : 1;
			uint32_t latitude : 1;
			uint32_t altitude : 1;
		} moduleQueried;

	// Functions
	uint32_t extractLong(uint8_t spotToStart);
	int32_t extractSignedLong(uint8_t spotToStart);
	boolean getPVT(uint16_t maxWait = getPVTmaxWait);

public:
	bool initGPS();
	void enable();
	void disable();
	void update();
	void checkUblox(ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID);
	void process(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID);
	void processUBX(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID);
	void processUBXpacket(ubxPacket *msg);
	void addToChecksum(uint8_t incoming);

	int32_t getLatitude(uint16_t maxWait = getPVTmaxWait);			  //Returns the current latitude in degrees * 10^-7. Auto selects between HighPrecision and Regular depending on ability of module.
	int32_t getLongitude(uint16_t maxWait = getPVTmaxWait);			  //Returns the current longitude in degrees * 10-7. Auto selects between HighPrecision and Regular depending on ability of module.
	int32_t getAltitude(uint16_t maxWait = getPVTmaxWait);			  //Returns the current altitude in mm above ellipsoid

};

#endif /* SRC_PERIPHERAL_NEOM9_H_ */
