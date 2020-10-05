// Do not remove the include below
#include <HardwareSerial.h>
#include "LoRaCustom.h"


// Static variables which are manipulated in the interrupt, LoRaComms class is SINGLTON (only one instance) //
LoRaComms* LoRaComms::_deviceForInterrupt;

uint16_t LoRaComms::_interruptCount = 0;
int8_t LoRaComms::_lastSNR = 0;
int16_t LoRaComms::_lastRssi = 0;
uint8_t LoRaComms::_RxDataBufLen = 0;
bool LoRaComms::_RxBufValid = false;
RadioState::state LoRaComms::_mode;
uint8_t LoRaComms::_dataBuf[RH_RF95_MAX_MESSAGE_LEN];
uint16_t LoRaComms::_rxBad;
uint16_t LoRaComms::_rxGood;
uint16_t LoRaComms::_txGood;

/*
* Constructor for this class LoRaComms, should only be one instance from only one LoRa board per Arduino
*/
LoRaComms::LoRaComms(){}

void LoRaComms::debug() {
    // TODO Write this
    Serial.println("LoRaComms::debug");
}

void LoRaComms::push(uint8_t* buf, uint8_t len) {
    // TODO Write this
}

void LoRaComms::pull(uint8_t* buf, uint8_t len) {
    // TODO Write this
}

/*
 * Initialize the radio
 * @return if the initialization function executed successfully or not
 */
bool LoRaComms::init(){
	//TODO: should have an #ifdef DEBUG for the print statements, make a LOG function

	SPI.begin();	// apparently good practice to do before pin initialization/binding

	// setup for pins and interrupts
	pinMode(SLAVE_SELECT, OUTPUT);
	pinMode(INTERRUPT_PIN, INPUT);	// Arduino inits pins as inputs by default but doesn't hurt to declare

	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onG0Rise, RISING);	// bind the radio's G0 pin to an Arduino interrupt
	SPI.usingInterrupt(INTERRUPT_PIN);	// interrupt on radio, in this case for radio TxDone and RxDone interrupts
										// do SPI.using instead of only attach because we want to use
										// SPI within the interrupt routine, like clearing the radio flags

	setModeSleep();
	//spiWrite(0x01, (0x80 | 0x00));	//put radio into sleep mode so that it can be put into LoRa mode
	Serial.print("Radio is in mode: "); Serial.println(_mode);//Serial.println(spiRead(0x01));

	setModeIdle();
	//spiWrite(0x01, (0x80 | 0x01));	//put radio into standby mode
	Serial.print("Now in mode: "); Serial.println(_mode);//Serial.println(spiRead(0x01));

	spiWrite(RH_RF95_REG_0E_FIFO_TX_BASE_ADDR, 0);	// The transmission data starts at the beginning of the FIFO (0)
	spiWrite(RH_RF95_REG_0F_FIFO_RX_BASE_ADDR, 127);  // the received data starts in the middle of the FIFO (127)
													// Full 256 byte FIFO is halved for TX & RX, max message size is 128 bytes

	setFrequency(915.0); //Set the frequency to 915MHz
	Serial.println(spiRead(RH_RF95_REG_06_FRF_MSB));	//should read 11100100 = 228 = 0xE4 for 915

	setTxPower();
	Serial.print(spiRead(0x09)); Serial.print(", "); Serial.println(spiRead(0x4D));

	spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
	spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // apparently do it twice to make absolutely sure its done

	Serial.println(_mode);

    return true;
}

/*
 * When the radio triggers an interrupt on pin G0 such as for TxDone & RxDone, take appropriate action for handling it
 */
void LoRaComms::onG0Rise(){
	_deviceForInterrupt->handleInterrupt();
}

/*
 * Function which should be run when the radio triggers an interrupt
 */
void LoRaComms::handleInterrupt(){
	_interruptCount++; // Keeping a running count of how many times this function is called (interrupts raised by radio after startup)

	uint8_t irq_flags = spiRead(RH_RF95_REG_12_IRQ_FLAGS);	//read the interrupt info register to see which interrupt was raised

	if((_mode == RadioState::RHModeRx) && (irq_flags & RH_RF95_RX_DONE)){	//RxDone bit set to a 1? (RxDone caused interrupt)

		uint8_t len = spiRead(RH_RF95_REG_13_RX_NB_BYTES);	//should be equal to the number of bytes received
		spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, spiRead(RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR));	// Set the FIFO read pointer to the beginning of the packet
		spiBurstRead(0x00, _dataBuf, len);		// read the data in FIFO, put it in the data buffer
		_RxDataBufLen = len;	// keep track of the amount of bytes received, for message validation checking and debugging
		spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags, they do this next in the library

		_lastSNR = (int8_t)spiRead(RH_RF95_REG_19_PKT_SNR_VALUE) / 4;	// Remember the last signal to noise ratio, LORA mode
		_lastRssi = spiRead(RH_RF95_REG_1A_PKT_RSSI_VALUE);	// Remember the RSSI of this packet, LORA mode

		if (_lastSNR < 0)	// Adjust the RSSI, datasheet gives details
			_lastRssi = _lastRssi + _lastSNR;
		else
			_lastRssi = (int)_lastRssi * 16 / 15;
		//if (_usingHFport)	[we only transmit at 915MHz like true ballers, the else is {lastRssi -= 164} for low freq]
		_lastRssi -= 157;

		validateRxDataBuf();	// Have received a message, check it
		if(_RxBufValid){
			setModeIdle(); 		// Got a good one, idling the radio apparently isn't needed for reading it but good if next move is to transmit
		}
	}

	else if((_mode == RadioState::RHModeTx) && (irq_flags & RH_RF95_TX_DONE)){	//TxDone bit set to a 1? (TxDone caused interrupt)

		_txGood++;		// up the number of valid messages transmitted during runtime since startup
		setModeIdle();
	}

	else{
		_rxBad++;	// iqr_flags says neither good send nor recv, library uses crc check for better use of this variable
	}

	spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
	spiWrite(RH_RF95_REG_12_IRQ_FLAGS, 0xff); // apparently do it twice to make absolutely sure its done
}

/*
 * 	Check if a new valid message is available (stored in the class to be copied using the getReceived function)
 * 	Sets the radio back to receive mode, possible a packet came in from the interrupt just as the getReceived function executed.
 * 	Acts as a getter for rxBufValid and safety for not attempting to read from the FIFO while the radio could be transmitting data in it
 *	@return false if the radio is transmitting, or the value of rxBufValid to confirm if a good message was successfully received
 */
bool LoRaComms::available(){
	if (_mode == RadioState::RHModeTx)
	return false;				// don't disturb an outgoing transmission
	setModeRx();
	return _RxBufValid; // Will be set by the interrupt handler when a good message is received
}

/*
 * Copy the received message from the _dataBuf array of this class to the array specified in the parameters
 * Received messages from the radio are copied to _dataBuf during the interrupt routine to be accessed by this function
 * @param buf pointer to the buffer array to which the latest message should be copied
 * @param len pointer specifying the size of the buf array parameter, how many bytes should be copied
 * @return if the function executed successfully or not
 */
bool LoRaComms::getReceived(uint8_t* buf, uint8_t* len){
	if (!available()){
	return false;
	}
	if (buf && len)	// if one of these is zero it doesn't exist
	{
	noInterrupts(); //ATOMIC_BLOCK_START;

	memcpy(buf, _dataBuf, *len);
	interrupts(); //ATOMIC_BLOCK_END;
	}
	clearRxDataBuf(); // This last message was accepted and copied, reset for next one
	return true;
}


void LoRaComms::validateRxDataBuf(){
	//TODO: Potential location for Jimmy's corruption checksum, but prob a better idea to make that more external, raw data is important

	if(_RxDataBufLen < 3){		// All messages sent should have at least 3 bytes, the first two being used for a corruption checksum
		return;		// don't set _RxBufValid, message was too short to be valid
	}

	_rxGood++;	// Total valid packets received during runtime
	_RxBufValid = true;
}

/*
 * Reset the received message buffer for the next message
 */
void LoRaComms::clearRxDataBuf(){
	noInterrupts(); //ATOMIC_BLOCK_START;
	_RxDataBufLen = 0;
	_RxBufValid = false;
	interrupts(); //ATOMIC_BLOCK_END;
}

/*
 * Copy a message from a buffer array to be sent to the radio for transmitting
 * @param data pointer to the buffer array from which the message to be sent is copied
 * @param len the size of the buf array parameter, how many bytes should be copied
 * @return if the function executed successfully or not
 */
bool LoRaComms::send(const uint8_t* data, uint8_t len){

	spiWrite(RH_RF95_REG_0D_FIFO_ADDR_PTR, 0);		// Position at the beginning of the FIFO

	spiBurstWrite(RH_RF95_REG_00_FIFO, data, len);		// push the data array to the FIFO for transmitting
	spiWrite(0x22, len);	//TODO: macro def 0x22

	//TODO: #ifdef DEBUG these prints are helpful
//		spiWrite(0x0d, 0x00);				//FIFO pointer to the 0 index so the following reads can confirm the message was pushed
//		Serial.println(spiRead(0x00));		//should be an ASCII letter for the test message
//		Serial.println(spiRead(0x00));
//		Serial.println(spiRead(0x00));
//		Serial.println(spiRead(0x00));

	setModeTx(); // set to transmit mode to send data, when finished interruptHandler will fire and radio mode will return to idle mode

	return true;
}

/*
 * Set the frequency of the radio
 * @param center is the frequency in MHz, acceptable values are 915 and 433
 * @return true if the function executed successfully
 */
bool LoRaComms::setFrequency(float center){

    uint32_t frf = (center * 1000000.0) / RH_RF95_FSTEP;
    spiWrite(RH_RF95_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    spiWrite(RH_RF95_REG_07_FRF_MID, (frf >> 8) & 0xff);
    spiWrite(RH_RF95_REG_08_FRF_LSB, frf & 0xff);

    return true;
}

/*
 * Set the radio to sleep mode
 * this allows LoRa mode to be set which is the mode that we always want to use
 * @return true if the function executed successfully
 */
bool LoRaComms::setModeSleep(){
    if (_mode != RadioState::RHModeSleep){
    	spiWrite(RH_RF95_REG_01_OP_MODE, (RH_RF95_LONG_RANGE_MODE | RH_RF95_MODE_SLEEP));	// binary OR with 0x80 to set radio to LoRa mode, always want this mode
    	_mode = RadioState::RHModeSleep;
    }
    return true;
}

/*
 * Set the radio to idle mode
 * this is commonly done after sleep mode, hence the setting to LoRa mode for reassurance, otherwise setting this is redundant
 */
void LoRaComms::setModeIdle(){
	if(_mode != RadioState::RHModeIdle){
		spiWrite(RH_RF95_REG_01_OP_MODE, (RH_RF95_LONG_RANGE_MODE | RH_RF95_MODE_STDBY));	// binary OR with 0x80 to set radio to LoRa mode, always want this mode
		_mode = RadioState::RHModeIdle;
	}
}

/*
 * Set the radio to continuous receive mode
 * as well as have the radio set the RxDone interrupt when a message is received for handling it
 */
void LoRaComms::setModeRx(){
	if (_mode != RadioState::RHModeRx){
		spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_RXCONTINUOUS);
		spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
		_mode = RadioState::RHModeRx;
	}
}

/*
 * Set the radio to transmit mode
 * as well as have the radio set the TxDone interrupt when a message is successfully sent
 * This should be done once after a message was written to the FIFO for transmitting
 */
void LoRaComms::setModeTx(){
	 if (_mode != RadioState::RHModeTx){
		spiWrite(RH_RF95_REG_01_OP_MODE, RH_RF95_MODE_TX);
		spiWrite(RH_RF95_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
		_mode = RadioState::RHModeTx;
	  }
}

/*
 * Set the transmission power to maximum (20dbm/100mW), no need to save power, rockets fly high and far
 */
void LoRaComms::setTxPower(){
	//TODO: be a good boi and #define these...
	spiWrite(0x09, 0x8C);	//10001100 = 0x8C = 140
	spiWrite(0x4D, 0x07);   //Set to max tx power, 20dbm
}


//*********************** Getter functions *****************************//

/*
 * Getter function
 * @return the SNR value of the last received packet
 */
int LoRaComms::getLastSNR(){
	return _lastSNR;
}

/*
 * Getter function
 * @return the RSSI value of the last received packet
 */
int16_t LoRaComms::getLastRSSI(){
	return _lastRssi;
}

/*
 * Getter function
 * @return the number of valid messages received during runtime since startup
 */
uint16_t LoRaComms::getRxGood(){
	return _rxGood;
}

/*
 * Getter function
 * @return the number of valid messages transmitted during runtime since startup
 */
uint16_t LoRaComms::getTxGood(){
	return _txGood;
}

/*
 * Getter function
 * @return the number of invalid messages received during runtime since startup
 */
uint16_t LoRaComms::getRxBad(){
	return _rxBad;
}


RadioState::state LoRaComms::getMode(){
	return _mode;
}



// maybe I'll use these eventually..........
bool LoRaComms::waitPacketSent(){
    while (_mode == RadioState::RHModeTx)
	yield(); //YIELD; // Wait for any previous transmit to finish
    return true;
}

bool LoRaComms::waitPacketSent(uint16_t timeout){
    unsigned long starttime = millis();
    while ((millis() - starttime) < timeout){
        if (_mode != RadioState::RHModeTx) // Any previous transmit finished?
           return true;
	yield(); //YIELD;
    }
    return false;
}



//******************* SPI communication functions for the RF95 LoRa radio *****************************//

/*
 * Write to the RF95 LoRa radio using its SPI parameters
 * @param reg the register to which a byte should be written
 * @param val the byte to write to the register
 * @return an SPI transfer command has a return value, often unused in this code
 */
uint8_t LoRaComms::spiWrite(uint8_t reg, uint8_t val){
    uint8_t status = 0;
    noInterrupts(); //ATOMIC_BLOCK_START;
    SPI.beginTransaction(SPISettings(RFM95X_SPI_FREQ_DEFAULT, MSBFIRST, SPI_MODE0));
    digitalWrite(SLAVE_SELECT, LOW);		//TODO: make fast!	[Say: equivalent to digitalwrite(number, low)]!
    status = SPI.transfer(reg | RH_SPI_WRITE_MASK); // Send the address with the write mask on
    SPI.transfer(val); // New value follows
    digitalWrite(SLAVE_SELECT, HIGH);	//TODO: make fast!
    SPI.endTransaction();
    interrupts(); //ATOMIC_BLOCK_END;
    return status;
}

/*
 * Write many bytes in succession to the RF95 LoRa radio using its SPI parameters
 * @param reg the register to which the bytes should be written
 * @param src pointer to the start of the byte array to write to the registers
 * @param len length of the array of bytes to be written, make sure its correct or reads array out of bounds!
 * @return an SPI transfer command has a return value, often unused in this code
 */
uint8_t LoRaComms::spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len){
    uint8_t status = 0;
    noInterrupts(); //ATOMIC_BLOCK_START;
    SPI.beginTransaction(SPISettings(RFM95X_SPI_FREQ_DEFAULT, MSBFIRST, SPI_MODE0));
    digitalWrite(SLAVE_SELECT, LOW);		//TODO: make fast!
    status = SPI.transfer(reg | RH_SPI_WRITE_MASK); // Send the start address with the write mask on
    while (len--)
	SPI.transfer(*src++);
    digitalWrite(SLAVE_SELECT, HIGH);		//TODO: make fast!
    SPI.endTransaction();
    interrupts(); //ATOMIC_BLOCK_END;
    return status;
}

/*
 * Read from the RF95 LoRa radio using its SPI parameters
 * @param reg the register from which a byte should be read
 * @return the byte read from the register
 */
uint8_t LoRaComms::spiRead(uint8_t reg){

	uint8_t val;
	noInterrupts(); //ATOMIC_BLOCK_START;
	digitalWrite(SLAVE_SELECT, LOW);		//TODO: make fast!

	SPI.beginTransaction(SPISettings(RFM95X_SPI_FREQ_DEFAULT, MSBFIRST, SPI_MODE0));

	SPI.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the address with the write mask off
	val = SPI.transfer(0); // The written value is ignored, reg value is read

	SPI.endTransaction();

	digitalWrite(SLAVE_SELECT, HIGH);		//TODO: make fast!
	interrupts(); //ATOMIC_BLOCK_END;
	return val;

}

/*
 * Read many bytes in succession from the the RF95 LoRa radio using its SPI parameters
 * @param reg the register from which bytes should be read
 * @param src pointer to the start of the byte array to which the bytes from the registers should be stored
 * @param len number of bytes to be read, make sure its correct or writes to array out of bounds!
 * @return the return value of the SPI transfer command for reading from reg, in this case the bytes of interest are written to dest
 */
uint8_t LoRaComms::spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len){
    uint8_t status = 0;
    noInterrupts(); //ATOMIC_BLOCK_START;
	SPI.beginTransaction(SPISettings(RFM95X_SPI_FREQ_DEFAULT, MSBFIRST, SPI_MODE0));
    digitalWrite(SLAVE_SELECT, LOW);		//TODO: make fast!
    status = SPI.transfer(reg & ~RH_SPI_WRITE_MASK); // Send the start address with the write mask off
    while (len--)
	*dest++ = SPI.transfer(0);
    digitalWrite(SLAVE_SELECT, HIGH);		//TODO: make fast!
    SPI.endTransaction();
    interrupts(); //ATOMIC_BLOCK_END;
    return status;
}
