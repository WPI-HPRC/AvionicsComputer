// Header file for the custom LoRa comms library
// based off of RadioHead's library but made to be less general for size and reliability,
// original code for this project was working using this library

#ifndef _LoRaCustom_H_
#define _LoRaCustom_H_

#include "Arduino.h"
#include "PeripheralInterface.h"
//#include <stdio.h>	// for memcpy function
#include <SPI.h> 	// Use SPI for communicating with the LoRa radios

// Macro definitions

#define SLAVE_SELECT 4
#define INTERRUPT_PIN 3		// That's right, these never change! It's a PCB

#define RH_RF95_MAX_MESSAGE_LEN 128		// 256 byte FIFO is half TX data half RX data (128-128)
										// ! Using messages greater than this will cause FIFO overwriting !

#define RH_SPI_WRITE_MASK 0x80
#define RFM95X_SPI_FREQ_DEFAULT 8E6
#define RH_RF95_FSTEP  					 (32000000.0 / 524288)

#define RH_RF95_REG_00_FIFO                               0x00
#define RH_RF95_REG_06_FRF_MSB                            0x06
#define RH_RF95_REG_07_FRF_MID                            0x07
#define RH_RF95_REG_08_FRF_LSB                            0x08
#define RH_RF95_REG_10_FIFO_RX_CURRENT_ADDR				  0x10
#define RH_RF95_REG_12_IRQ_FLAGS						  0x12
#define RH_RF95_REG_13_RX_NB_BYTES						  0x13
#define RH_RF95_REG_0D_FIFO_ADDR_PTR					  0x0d
#define RH_RF95_REG_0E_FIFO_TX_BASE_ADDR                  0x0e
#define RH_RF95_REG_0F_FIFO_RX_BASE_ADDR                  0x0f
#define RH_RF95_REG_40_DIO_MAPPING1                       0x40
#define RH_RF95_REG_19_PKT_SNR_VALUE 					  0x19
#define RH_RF95_REG_1A_PKT_RSSI_VALUE 					  0x1a

#define RH_RF95_REG_01_OP_MODE							  0x01
#define RH_RF95_MODE_SLEEP			    				  0x00
#define RH_RF95_MODE_STDBY								  0x01
#define RH_RF95_MODE_TX                               	  0x03
#define RH_RF95_MODE_RXCONTINUOUS                     	  0x05
#define RH_RF95_LONG_RANGE_MODE							  0x80

#define RH_RF95_RX_TIMEOUT                            	  0x80
#define RH_RF95_RX_DONE                               	  0x40
#define RH_RF95_PAYLOAD_CRC_ERROR                     	  0x20
#define RH_RF95_VALID_HEADER                          	  0x10
#define RH_RF95_TX_DONE                                   0x08
#define RH_RF95_CAD_DONE                              	  0x04
#define RH_RF95_FHSS_CHANGE_CHANNEL                   	  0x02
#define RH_RF95_CAD_DETECTED                         	  0x01


// enumeration for keeping track of the current radio state
namespace RadioState {
	enum state {
		RHModeInitialising = 0, ///< Transport is initializing. Initial default value until init() is called..
		RHModeSleep = 1,            ///< Transport hardware is in low power sleep mode (if supported)
		RHModeIdle = 2,             ///< Transport is idle/standby.
		RHModeTx = 3,               ///< Transport is in the process of transmitting a message.
		RHModeRx = 4,               ///< Transport is in the process of receiving a message.
		RHModeCad = 5               ///< Transport is in the process of detecting channel activity (if supported)
	};
}

/*
 * Class for using RFM95 LoRa radio modules, made specifically for the rocket tracker/telemetry system
 * thus has little flexibility and is made mostly for good code organization
 */
class LoRaComms : public PeripheralInterface {

public:

	LoRaComms();
//    ~LoRaComms(){}
    void debug();
    void push(uint8_t* buf, uint8_t len);
    void pull(uint8_t* buf, uint8_t len);

	bool init();
	void handleInterrupt();

	bool available();
	bool getReceived(uint8_t* buf, uint8_t* len);
	void validateRxDataBuf();
	void clearRxDataBuf();

	bool send(const uint8_t* data, uint8_t len);

	bool setFrequency(float center);
	void setModeIdle();
	bool setModeSleep();
	void setModeRx();
	void setModeTx();
	void setTxPower();

	int getLastSNR();
	int16_t getLastRSSI();
	uint16_t getRxGood();
	uint16_t getTxGood();
	uint16_t getRxBad();
	uint8_t getTest();	// for testing, a variable to be set

	bool waitPacketSent();
	bool waitPacketSent(uint16_t timeout);

	RadioState::state getMode();

	static uint16_t _interruptCount; //TODO: should be private but I'm being a lazy boi for testing purposes...


private:

	static LoRaComms * _deviceForInterrupt;

//	static uint16_t _interruptCount;

	static void onG0Rise();

	static uint8_t _dataBuf[RH_RF95_MAX_MESSAGE_LEN];	// Buffer in which received packet data should be stored when the RxDone interrupt is triggered
    static int8_t _lastSNR;	// Last measured SNR, dB
    static int16_t _lastRssi;	// The value of the last received RSSI value, in some transport specific units

    static uint16_t _rxBad;	// Count of the number of bad messages (eg bad checksum etc) received
    static uint16_t _rxGood;	// Count of the number of successfully transmitted messaged
    static uint16_t _txGood;	// Count of the number of sent messages (correct checksum etc) sent

    static uint8_t _RxDataBufLen;		// size of the received data
    static bool _RxBufValid;	/// True when there is a valid message in the buffer

    /// The current transport operating mode
    static RadioState::state     _mode;


    // SPI
	uint8_t spiWrite(uint8_t reg, uint8_t val);
	uint8_t spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len);
	uint8_t spiRead(uint8_t reg);
	uint8_t spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);
};



#endif /* _LoRaCustom_H_ */
