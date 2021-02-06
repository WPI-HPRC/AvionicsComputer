/*
 * NEOM9.cpp
 *
 *  Created on: Jan 5, 2021
 *  	Author: Michael Beskid
 */

#include "NEOM9.h"

void NEOM9::enable(){
	initGPS();
}

/*
 * Initializes GPS
 * No registers to set, checks if I2C device on GPS address
 * @return true if successful communication with device, false if error
 */
bool NEOM9::initGPS(){
	Wire.beginTransmission(NEOM9_ADDRESS);
	return !Wire.endTransmission(true);			//endTransmission returns 0 if success, error code if failure
}

void NEOM9::disable(){

}

void NEOM9::update(){
	checkUblox(&packetCfg, requestedClass, requestedID);
}

void NEOM9::checkUblox(ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID) {

	uint16_t bytesAvailable = 0;
	Wire.beginTransmission(NEOM9_ADDRESS);
	Wire.write(0xFD);                     			//0xFD (MSB) and 0xFE (LSB) are the registers that contain number of bytes available
	if (Wire.endTransmission(false) != 0) { 		//Send a restart command. Do not release bus.
		Serial.println("BIG SAD");
		return;                         		 	//Sensor did not ACK
	}

	Wire.requestFrom(NEOM9_ADDRESS, 2);

	uint8_t msb = Wire.read();
	uint8_t lsb = Wire.read();
	bytesAvailable = msb <<8| lsb;

	while (bytesAvailable)
	    {
	      Wire.beginTransmission(NEOM9_ADDRESS);
	      Wire.write(0xFF);                     	//0xFF is the register to read data from
	      if (Wire.endTransmission(false) != 0) 	//Send a restart command. Do not release bus.
	    	  Serial.println("BIG SAD");
	    	  return;                          		//Sensor did not ACK

	      //Limit to 32 bytes or whatever the buffer limit is for given platform
	      uint16_t bytesToRead = bytesAvailable;
	      if (bytesToRead > i2cTransactionSize)
	        bytesToRead = i2cTransactionSize;

	    TRY_AGAIN:

	      Wire.requestFrom(NEOM9_ADDRESS, bytesToRead);
	      if (Wire.available())
	      {
	        for (uint16_t x = 0; x < bytesToRead; x++)
	        {
	          uint8_t incoming = Wire.read(); //Grab the actual character

	          //Check to see if the first read is 0x7F. If it is, the module is not ready
	          //to respond. Stop, wait, and try again
	          if (x == 0)
	          {
	            if (incoming == 0x7F)
	            {
	              delay(5); //In logic analyzation, the module starting responding after 1.48m
	              goto TRY_AGAIN;
	            }
	          }
	          Serial.println(incoming);

	          process(incoming, &packetCfg, requestedClass, requestedID); //Process this valid character
	        }
	      }
	      else
	    	  Serial.println("BIG SAD");
	    	  return;                          			//Sensor did not respond
	      bytesAvailable -= bytesToRead;
	    }
}

void NEOM9::process(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID)
{
  if ((currentSentence == NONE) || (currentSentence == NMEA))
  {
    if (incoming == 0xB5) //UBX binary frames start with 0xB5, aka mu
    {
      //This is the start of a binary sentence. Reset flags.
      //We still don't know the response class
      ubxFrameCounter = 0;
      currentSentence = UBX;
      //Reset the packetBuf.counter even though we will need to reset it again when ubxFrameCounter == 2
      packetBuf.counter = 0;
      ignoreThisPayload = false; //We should not ignore this payload - yet
      //Store data in packetBuf until we know if we have a requested class and ID match
      activePacketBuffer = SFE_UBLOX_PACKET_PACKETBUF;
    }
    else if (incoming == '$')
    {
      currentSentence = NMEA;
    }
    else if (incoming == 0xD3) //RTCM frames start with 0xD3
    {
      rtcmFrameCounter = 0;
      currentSentence = RTCM;
    }
    else
    {
      //This character is unknown or we missed the previous start of a sentence
    }
  }

  //Depending on the sentence, pass the character to the individual processor
  if (currentSentence == UBX)
  {
    //Decide what type of response this is
    if ((ubxFrameCounter == 0) && (incoming != 0xB5))      //ISO 'mu'
      currentSentence = NONE;                              //Something went wrong. Reset.
    else if ((ubxFrameCounter == 1) && (incoming != 0x62)) //ASCII 'b'
      currentSentence = NONE;                              //Something went wrong. Reset.
    // Note to future self:
    // There may be some duplication / redundancy in the next few lines as processUBX will also
    // load information into packetBuf, but we'll do it here too for clarity
    else if (ubxFrameCounter == 2) //Class
    {
      // Record the class in packetBuf until we know what to do with it
      packetBuf.cls = incoming; // (Duplication)
      rollingChecksumA = 0;     //Reset our rolling checksums here (not when we receive the 0xB5)
      rollingChecksumB = 0;
      packetBuf.counter = 0;                                   //Reset the packetBuf.counter (again)
      packetBuf.valid = SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED; // Reset the packet validity (redundant?)
      packetBuf.startingSpot = incomingUBX->startingSpot;      //Copy the startingSpot
    }
    else if (ubxFrameCounter == 3) //ID
    {
      // Record the ID in packetBuf until we know what to do with it
      packetBuf.id = incoming; // (Duplication)
      //We can now identify the type of response
      //If the packet we are receiving is not an ACK then check for a class and ID match
      if (packetBuf.cls != UBX_CLASS_ACK)
      {
        //This is not an ACK so check for a class and ID match
        if ((packetBuf.cls == requestedClass) && (packetBuf.id == requestedID))
        {
          //This is not an ACK and we have a class and ID match
          //So start diverting data into incomingUBX (usually packetCfg)
          activePacketBuffer = SFE_UBLOX_PACKET_PACKETCFG;
          incomingUBX->cls = packetBuf.cls; //Copy the class and ID into incomingUBX (usually packetCfg)
          incomingUBX->id = packetBuf.id;
          incomingUBX->counter = packetBuf.counter; //Copy over the .counter too
        }
        //This is not an ACK and we do not have a complete class and ID match
        //So let's check for an HPPOSLLH message arriving when we were expecting PVT and vice versa
        else if ((packetBuf.cls == requestedClass) &&
          (((packetBuf.id == UBX_NAV_PVT) && (requestedID == UBX_NAV_HPPOSLLH || requestedID == UBX_NAV_DOP)) ||
           ((packetBuf.id == UBX_NAV_HPPOSLLH) && (requestedID == UBX_NAV_PVT || requestedID == UBX_NAV_DOP)) ||
           ((packetBuf.id == UBX_NAV_DOP) && (requestedID == UBX_NAV_PVT || requestedID == UBX_NAV_HPPOSLLH))))
        {
          //This is not the message we were expecting but we start diverting data into incomingUBX (usually packetCfg) and process it anyway
          activePacketBuffer = SFE_UBLOX_PACKET_PACKETCFG;
          incomingUBX->cls = packetBuf.cls; //Copy the class and ID into incomingUBX (usually packetCfg)
          incomingUBX->id = packetBuf.id;
          incomingUBX->counter = packetBuf.counter; //Copy over the .counter too
        }
        else if ((packetBuf.cls == requestedClass) &&
          (((packetBuf.id == UBX_HNR_ATT) && (requestedID == UBX_HNR_INS || requestedID == UBX_HNR_PVT)) ||
           ((packetBuf.id == UBX_HNR_INS) && (requestedID == UBX_HNR_ATT || requestedID == UBX_HNR_PVT)) ||
           ((packetBuf.id == UBX_HNR_PVT) && (requestedID == UBX_HNR_ATT || requestedID == UBX_HNR_INS))))
        {
          //This is not the message we were expecting but we start diverting data into incomingUBX (usually packetCfg) and process it anyway
          activePacketBuffer = SFE_UBLOX_PACKET_PACKETCFG;
          incomingUBX->cls = packetBuf.cls; //Copy the class and ID into incomingUBX (usually packetCfg)
          incomingUBX->id = packetBuf.id;
          incomingUBX->counter = packetBuf.counter; //Copy over the .counter too
        }
        else
        {
          //This is not an ACK and we do not have a class and ID match
          //so we should keep diverting data into packetBuf and ignore the payload
          ignoreThisPayload = true;
        }
      }
      else
      {
        // This is an ACK so it is too early to do anything with it
        // We need to wait until we have received the length and data bytes
        // So we should keep diverting data into packetBuf
      }
    }
    else if (ubxFrameCounter == 4) //Length LSB
    {
      //We should save the length in packetBuf even if activePacketBuffer == SFE_UBLOX_PACKET_PACKETCFG
      packetBuf.len = incoming; // (Duplication)
    }
    else if (ubxFrameCounter == 5) //Length MSB
    {
      //We should save the length in packetBuf even if activePacketBuffer == SFE_UBLOX_PACKET_PACKETCFG
      packetBuf.len |= incoming << 8; // (Duplication)
    }
    else if (ubxFrameCounter == 6) //This should be the first byte of the payload unless .len is zero
    {
      if (packetBuf.len == 0) // Check if length is zero (hopefully this is impossible!)
      {
        //If length is zero (!) this will be the first byte of the checksum so record it
        packetBuf.checksumA = incoming;
      }
      else
      {
        //The length is not zero so record this byte in the payload
        packetBuf.payload[0] = incoming;
      }
    }
    else if (ubxFrameCounter == 7) //This should be the second byte of the payload unless .len is zero or one
    {
      if (packetBuf.len == 0) // Check if length is zero (hopefully this is impossible!)
      {
        //If length is zero (!) this will be the second byte of the checksum so record it
        packetBuf.checksumB = incoming;
      }
      else if (packetBuf.len == 1) // Check if length is one
      {
        //The length is one so this is the first byte of the checksum
        packetBuf.checksumA = incoming;
      }
      else // Length is >= 2 so this must be a payload byte
      {
        packetBuf.payload[1] = incoming;
      }
      // Now that we have received two payload bytes, we can check for a matching ACK/NACK
      if ((activePacketBuffer == SFE_UBLOX_PACKET_PACKETBUF) // If we are not already processing a data packet
          && (packetBuf.cls == UBX_CLASS_ACK)                // and if this is an ACK/NACK
          && (packetBuf.payload[0] == requestedClass)        // and if the class matches
          && (packetBuf.payload[1] == requestedID))          // and if the ID matches
      {
        if (packetBuf.len == 2) // Check if .len is 2
        {
          // Then this is a matching ACK so copy it into packetAck
          activePacketBuffer = SFE_UBLOX_PACKET_PACKETACK;
          packetAck.cls = packetBuf.cls;
          packetAck.id = packetBuf.id;
          packetAck.len = packetBuf.len;
          packetAck.counter = packetBuf.counter;
          packetAck.payload[0] = packetBuf.payload[0];
          packetAck.payload[1] = packetBuf.payload[1];
        }
        else // Length is not 2 (hopefully this is impossible!)
        {
        }
      }
    }

    //Divert incoming into the correct buffer
    if (activePacketBuffer == SFE_UBLOX_PACKET_PACKETACK)
      processUBX(incoming, &packetAck, requestedClass, requestedID);
    else if (activePacketBuffer == SFE_UBLOX_PACKET_PACKETCFG)
      processUBX(incoming, incomingUBX, requestedClass, requestedID);
    else // if (activePacketBuffer == SFE_UBLOX_PACKET_PACKETBUF)
      processUBX(incoming, &packetBuf, requestedClass, requestedID);

    //Finally, increment the frame counter
    ubxFrameCounter++;
  }
  else if (currentSentence == NMEA)
  {
//    processNMEA(incoming); //Process each NMEA character
  }
  else if (currentSentence == RTCM)
  {
//    processRTCMframe(incoming); //Deal with RTCM bytes
  }
}

void NEOM9::processUBX(uint8_t incoming, ubxPacket *incomingUBX, uint8_t requestedClass, uint8_t requestedID)
{
   size_t max_payload_size = (activePacketBuffer == SFE_UBLOX_PACKET_PACKETCFG) ? MAX_PAYLOAD_SIZE : 2;
   bool overrun = false;

  //Add all incoming bytes to the rolling checksum
  //Stop at len+4 as this is the checksum bytes to that should not be added to the rolling checksum
  if (incomingUBX->counter < incomingUBX->len + 4)
    addToChecksum(incoming);

  if (incomingUBX->counter == 0)
  {
    incomingUBX->cls = incoming;
  }
  else if (incomingUBX->counter == 1)
  {
    incomingUBX->id = incoming;
  }
  else if (incomingUBX->counter == 2) //Len LSB
  {
    incomingUBX->len = incoming;
  }
  else if (incomingUBX->counter == 3) //Len MSB
  {
    incomingUBX->len |= incoming << 8;
  }
  else if (incomingUBX->counter == incomingUBX->len + 4) //ChecksumA
  {
    incomingUBX->checksumA = incoming;
  }
  else if (incomingUBX->counter == incomingUBX->len + 5) //ChecksumB
  {
    incomingUBX->checksumB = incoming;

    currentSentence = NONE; //We're done! Reset the sentence to being looking for a new start char

    //Validate this sentence
    if ((incomingUBX->checksumA == rollingChecksumA) && (incomingUBX->checksumB == rollingChecksumB))
    {
      incomingUBX->valid = SFE_UBLOX_PACKET_VALIDITY_VALID; // Flag the packet as valid

      // Let's check if the class and ID match the requestedClass and requestedID
      // Remember - this could be a data packet or an ACK packet
      if ((incomingUBX->cls == requestedClass) && (incomingUBX->id == requestedID))
      {
        incomingUBX->classAndIDmatch = SFE_UBLOX_PACKET_VALIDITY_VALID; // If we have a match, set the classAndIDmatch flag to valid
      }

      // If this is an ACK then let's check if the class and ID match the requestedClass and requestedID
      else if ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_ACK) && (incomingUBX->payload[0] == requestedClass) && (incomingUBX->payload[1] == requestedID))
      {
        incomingUBX->classAndIDmatch = SFE_UBLOX_PACKET_VALIDITY_VALID; // If we have a match, set the classAndIDmatch flag to valid
      }

      // If this is a NACK then let's check if the class and ID match the requestedClass and requestedID
      else if ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->id == UBX_ACK_NACK) && (incomingUBX->payload[0] == requestedClass) && (incomingUBX->payload[1] == requestedID))
      {
        incomingUBX->classAndIDmatch = SFE_UBLOX_PACKET_NOTACKNOWLEDGED; // If we have a match, set the classAndIDmatch flag to NOTACKNOWLEDGED
      }

      //We've got a valid packet, now do something with it but only if ignoreThisPayload is false
      if (ignoreThisPayload == false)
      {
        processUBXpacket(incomingUBX);
      }
    }
    else // Checksum failure
    {
      incomingUBX->valid = SFE_UBLOX_PACKET_VALIDITY_NOT_VALID;

      // Let's check if the class and ID match the requestedClass and requestedID.
      // This is potentially risky as we are saying that we saw the requested Class and ID
      // but that the packet checksum failed. Potentially it could be the class or ID bytes
      // that caused the checksum error!
      if ((incomingUBX->cls == requestedClass) && (incomingUBX->id == requestedID))
      {
        incomingUBX->classAndIDmatch = SFE_UBLOX_PACKET_VALIDITY_NOT_VALID; // If we have a match, set the classAndIDmatch flag to not valid
      }
      // If this is an ACK then let's check if the class and ID match the requestedClass and requestedID
      else if ((incomingUBX->cls == UBX_CLASS_ACK) && (incomingUBX->payload[0] == requestedClass) && (incomingUBX->payload[1] == requestedID))
      {
        incomingUBX->classAndIDmatch = SFE_UBLOX_PACKET_VALIDITY_NOT_VALID; // If we have a match, set the classAndIDmatch flag to not valid
      }

    }
  }
  else //Load this byte into the payload array
  {
    //If a UBX_NAV_PVT packet comes in asynchronously, we need to fudge the startingSpot
    uint16_t startingSpot = incomingUBX->startingSpot;
    if (incomingUBX->cls == UBX_CLASS_NAV && incomingUBX->id == UBX_NAV_PVT)
      startingSpot = 0;
    // Check if this is payload data which should be ignored
    if (ignoreThisPayload == false)
    {
      //Begin recording if counter goes past startingSpot
      if ((incomingUBX->counter - 4) >= startingSpot)
      {
        //Check to see if we have room for this byte
        if (((incomingUBX->counter - 4) - startingSpot) < max_payload_size) //If counter = 208, starting spot = 200, we're good to record.
        {
          incomingUBX->payload[incomingUBX->counter - 4 - startingSpot] = incoming; //Store this byte into payload array
        }
        else
        {
          overrun = true;
        }
      }
    }
  }

  //Increment the counter
  incomingUBX->counter++;

  if (overrun || (incomingUBX->counter == MAX_PAYLOAD_SIZE))
  {
    //Something has gone very wrong
    currentSentence = NONE; //Reset the sentence to being looking for a new start char
  }
}

//Once a packet has been received and validated, identify this packet's class/id and update internal flags
//Note: if the user requests a PVT or a HPPOSLLH message using a custom packet, the data extraction will
//      not work as expected because extractLong etc are hard wired to packetCfg payloadCfg. Ideally
//      extractLong etc should be updated so they receive a pointer to the packet buffer.
void NEOM9::processUBXpacket(ubxPacket *msg)
{
  switch (msg->cls)
  {
  case 0x01:
    if (msg->id == UBX_NAV_PVT && msg->len == 92)
    {
      //Parse various byte fields into global vars
      constexpr int startingSpot = 0; //fixed value used in processUBX

      longitude = extractSignedLong(24 - startingSpot);
      latitude = extractSignedLong(28 - startingSpot);
      altitude = extractSignedLong(32 - startingSpot);

      moduleQueried.longitude = true;
      moduleQueried.latitude = true;
      moduleQueried.altitude = true;
    }

  }
}

void NEOM9::addToChecksum(uint8_t incoming)
{
  rollingChecksumA += incoming;
  rollingChecksumB += rollingChecksumA;
}

//Get the current latitude in degrees
//Returns a long representing the number of degrees *10^-7
int32_t NEOM9::getLatitude(uint16_t maxWait)
{
  if (moduleQueried.latitude == false)
    getPVT(maxWait);
  moduleQueried.latitude = false; //Since we are about to give this to user, mark this data as stale
  moduleQueried.all = false;

  return (latitude);
}

//Get the current longitude in degrees
//Returns a long representing the number of degrees *10^-7
int32_t NEOM9::getLongitude(uint16_t maxWait)
{
  if (moduleQueried.longitude == false)
    getPVT(maxWait);
  moduleQueried.longitude = false; //Since we are about to give this to user, mark this data as stale
  moduleQueried.all = false;

  return (longitude);
}

//Get the current altitude in mm according to ellipsoid model
int32_t NEOM9::getAltitude(uint16_t maxWait)
{
  if (moduleQueried.altitude == false)
    getPVT(maxWait);
  moduleQueried.altitude = false; //Since we are about to give this to user, mark this data as stale
  moduleQueried.all = false;

  return (altitude);
}

//Given a spot in the payload array, extract four bytes and build a long
uint32_t NEOM9::extractLong(uint8_t spotToStart)
{
  uint32_t val = 0;
  val |= (uint32_t)payloadCfg[spotToStart + 0] << 8 * 0;
  val |= (uint32_t)payloadCfg[spotToStart + 1] << 8 * 1;
  val |= (uint32_t)payloadCfg[spotToStart + 2] << 8 * 2;
  val |= (uint32_t)payloadCfg[spotToStart + 3] << 8 * 3;
  return (val);
}

//Just so there is no ambiguity about whether a uint32_t will cast to a int32_t correctly...
int32_t NEOM9::extractSignedLong(uint8_t spotToStart)
{
  union // Use a union to convert from uint32_t to int32_t
  {
      uint32_t unsignedLong;
      int32_t signedLong;
  } unsignedSigned;

  unsignedSigned.unsignedLong = extractLong(spotToStart);
  return (unsignedSigned.signedLong);
}

//Get the latest Position/Velocity/Time solution and fill all global variables
boolean NEOM9::getPVT(uint16_t maxWait)
{
    //The GPS is not automatically reporting navigation position so we have to poll explicitly
    packetCfg.cls = UBX_CLASS_NAV;
    packetCfg.id = UBX_NAV_PVT;
    packetCfg.len = 0;
    //packetCfg.startingSpot = 20; //Begin listening at spot 20 so we can record up to 20+MAX_PAYLOAD_SIZE = 84 bytes Note:now hard-coded in processUBX

    //The data is parsed as part of processing the response
    sfe_ublox_status_e retVal = sendCommand(&packetCfg, maxWait);

    if (retVal == SFE_UBLOX_STATUS_DATA_RECEIVED)
      return (true);

    if ((retVal == SFE_UBLOX_STATUS_DATA_OVERWRITTEN) && (packetCfg.cls == UBX_CLASS_NAV))
    {
      return (true);
    }

    return (false);
}

//Returns false if sensor fails to respond to I2C traffic
sfe_ublox_status_e NEOM9::sendCommand(ubxPacket *outgoingUBX, uint16_t maxWait)
{
  //Point at 0xFF data register
  Wire.beginTransmission(NEOM9_ADDRESS); //There is no register to write to, we just begin writing data bytes
  Wire.write(0xFF);
  if (Wire.endTransmission(false) != 0)         //Don't release bus
    return (SFE_UBLOX_STATUS_I2C_COMM_FAILURE); //Sensor did not ACK

  //Write header bytes
  Wire.beginTransmission(NEOM9_ADDRESS); //There is no register to write to, we just begin writing data bytes
  Wire.write(UBX_SYNCH_1);                         //mu - oh ublox, you're funny. I will call you micro-blox from now on.
  Wire.write(UBX_SYNCH_2);                         //b
  Wire.write(outgoingUBX->cls);
  Wire.write(outgoingUBX->id);
  Wire.write(outgoingUBX->len & 0xFF);     //LSB
  Wire.write(outgoingUBX->len >> 8);       //MSB
  if (Wire.endTransmission(false) != 0)    //Do not release bus
    return (SFE_UBLOX_STATUS_I2C_COMM_FAILURE); //Sensor did not ACK

  //Write payload. Limit the sends into 32 byte chunks
  //This code based on ublox: https://forum.u-blox.com/index.php/20528/how-to-use-i2c-to-get-the-nmea-frames
  uint16_t bytesToSend = outgoingUBX->len;

  //"The number of data bytes must be at least 2 to properly distinguish
  //from the write access to set the address counter in random read accesses."
  uint16_t startSpot = 0;
  while (bytesToSend > 1)
  {
    uint8_t len = bytesToSend;
    if (len > i2cTransactionSize)
      len = i2cTransactionSize;

    Wire.beginTransmission(NEOM9_ADDRESS);
    //_i2cPort->write(outgoingUBX->payload, len); //Write a portion of the payload to the bus

    for (uint16_t x = 0; x < len; x++)
    	Wire.write(outgoingUBX->payload[startSpot + x]); //Write a portion of the payload to the bus

    if (Wire.endTransmission(false) != 0)    //Don't release bus
      return (SFE_UBLOX_STATUS_I2C_COMM_FAILURE); //Sensor did not ACK

    //*outgoingUBX->payload += len; //Move the pointer forward
    startSpot += len; //Move the pointer forward
    bytesToSend -= len;
  }

  //Write checksum
  Wire.beginTransmission(NEOM9_ADDRESS);
  if (bytesToSend == 1)
	Wire.write(outgoingUBX->payload, 1);
  Wire.write(outgoingUBX->checksumA);
  Wire.write(outgoingUBX->checksumB);

  //All done transmitting bytes. Release bus.
  if (Wire.endTransmission() != 0)
    return (SFE_UBLOX_STATUS_I2C_COMM_FAILURE); //Sensor did not ACK
  return (SFE_UBLOX_STATUS_SUCCESS);
}

