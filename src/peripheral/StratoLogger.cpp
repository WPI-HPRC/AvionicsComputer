
#include "StratoLogger.h"

/*
* Constructor for this class StratoLogger, should only be one instance from only one StratoLogger board per Arduino
*/
StratoLogger::StratoLogger():
	validSerialMsg(false),				// Initialize the StratoLogger class variables
	readingsCount(0),
	serialBuffAvail(0),
	bad(false)
{

	stratoLogger.begin(STRATOLOGGER_BAUD_RATE);	// Start the SoftwareSerial object for getting StratoLogger data
}

/**
 * Read the StratoLogger values placed into the serial buffer and parse them for storing the
 * altitude reading into the curentAltRreading buffer
 * @return true if a valid message was copied, false if a proper data stream wasn't captured
 */
bool StratoLogger::readForAltitude(){

	// begin by checking if anything was received from the StratoLogger to be parsed
	if(!(stratoLogger.available() > 0)){
		return false;
	}

	resetSerialBuff();	// zero the buffer
	while(stratoLogger.available() > 0){
		lastSerialBuff[stratoLogger.available()] = stratoLogger.read();
	}

	uint16_t altitude = 0;
	bool goodLF = false;	// keeping track of the message new line character
	uint8_t onDigit = 0;	// which digit place should get filled in?

	int zeros = 0;

//	// StratoLogger goes up to 10,000ft range, 5 digits
//	uint8_t digit1 = 0; // ones place
//	uint8_t digit2 = 0; // tens place
//	uint8_t digit3 = 0; // hundreds place
//	uint8_t digit4 = 0; // thousands place
//	uint8_t digit5 = 0; // ten thousands place

	for(uint8_t i = 0; i < sizeof(lastSerialBuff); i++){		// should be <LF> in [1] and <CR> in [2]
		uint8_t val = parseStratoLoggerChar(lastSerialBuff[i]);	// [0] assumed to be zero, drain buffer entirely
//		Serial.print(F("val: ")); Serial.println(val);

//		if val == 0 do nothing, handled by initializing local variables to zero

		if(val == ASCII_LF){	// [1] is LF (10) captured ?
			goodLF = true;
			onDigit = 0;
		}
		if(val == ASCII_CR && goodLF){	// is CR (13) captured after LF (10) ?
			goodLF = false;
			altitude = 0;	// reset altitude
			onDigit = 1;	// reset the message
		}

		else if(onDigit == 1){	// [3] is first digit (10^0)
			//digit1 = val;
			altitude += val;
			onDigit = 2;	// up the digit
		}
		else if(onDigit == 2){	// is second digit? (10^1)
			//digit2 = val;
			altitude += (val * 10);
			onDigit = 3;	// up the digit
		}
		else if(onDigit == 3){	// is third digit? (10^2)
			//digit3 = val;
			altitude += (val * 100);
			onDigit = 4;	// up the digit
		}
		else if(onDigit == 4){	// is fourth digit? (10^3)
			//digit4 = val;
			altitude += (val * 1000);
			onDigit = 5;
		}
		else if(onDigit == 5){	// is fifth digit? (10^4)
			//digit5 = val;
			altitude += (val * 10000);
			onDigit = 6;	// shouldn't ever go beyond reaching this point
		}
	}

	if(onDigit == 0){	// beginning of valid message was never reached, invalid data
		//Serial.print(F("onDigit: "));Serial.println(onDigit);
		bad = true;
		return false;	//
	}
	else if(bad){
			bad = false;
			return false;
		}

	currentAltReading[1] = altitude;		// LSB end of the altitude reading
	currentAltReading[0] = altitude >>8; 	// MSB end of the altitude reading


//	if(goodLF && goodCR){
//		validSerialMsg = true;
//	} else{Serial.println("Bad one");}	// !!! LOTS OF DEBUGGING !!!
	//Serial.print(F("valid?: "));Serial.println(validSerialMsg);	// !!! DEBUG LYYYFEEEEE !!!
	//Serial.print(F("digit1: "));Serial.println(digit1);
	//Serial.print(F("digit2: "));Serial.println(digit2);
//	altitude = digit1*(1) + digit2*(10) + digit3*(100) + digit4*(1000) + digit5*(10000);
//	Serial.print(F("Combined: ")); Serial.println(altitude);
//
//	currentAltReading[1] = altitude;		// LSB end of the altitude reading
//	currentAltReading[0] = altitude >>8; 	// MSB end of the altitude reading

	return true; // function copied new parsed data to the currentAltReading successfully
}


/*
 * Parse a StratoLogger serial output character for real-time altitude reading
 * @param serialRead is the data read from the StratoLogger, can have the form of
 *     the ASCII <CR>(deci 13) and <LF>(deci 10) characters or an altitude reading digit
 * @return the latest altitude character reading
 */
uint8_t StratoLogger::parseStratoLoggerChar(uint8_t serialRead){

	if(serialRead == 255){  //No character available [softSerial.available() = 0]
		return 255;			// a -1 for unsigned (b11111111)
	}
	else if(serialRead == 0){
		return 0;
	}
	else if(serialRead == ASCII_LF){ // ASCII LF character received
		return ASCII_LF;			 // caught StratoLogger end of data character format
	}
	else if(serialRead == ASCII_CR){ // ASCII CR character
		return ASCII_CR;			 // caught StratoLogger end of data character format
	}
	else if(serialRead == ASCII_dash){ // ASCII "-"
		return 0;	//TODO: handle this character somehow
	}
	else{ //Assumed to be an altitude reading decimal
		return asciiToDeci(serialRead);
	}
}

uint8_t StratoLogger::asciiToDeci(uint8_t asciiNum){

	if(asciiNum == ASCII_0){	// ASCII "0"
		return 0;
	} else if(asciiNum == ASCII_1){	// ASCII "1"
		return 1;
	} else if(asciiNum == ASCII_2){	// ASCII "2"
		return 2;
	} else if(asciiNum == ASCII_3){	// ASCII "3"
		return 3;
	} else if(asciiNum == ASCII_4){	// ASCII "4"
		return 4;
	} else if(asciiNum == ASCII_5){	// ASCII "5"
		return 5;
	} else if(asciiNum == ASCII_6){	// ASCII "6"
		return 6;
	} else if(asciiNum == ASCII_7){	// ASCII "7"
		return 7;
	} else if(asciiNum == ASCII_8){	// ASCII "8"
		return 8;
	} else if(asciiNum == ASCII_9){	// ASCII "9"
		return 9;
	} else { return asciiNum;}
}


void StratoLogger::debug() {

    //Serial.println("StratoLogger::debug");
    Serial.print("Readings: ");		Serial.println(readingsCount);

}

void StratoLogger::push(uint8_t* buf, uint8_t len) {
    // Peripheral StratoLogger doesn't take in data
}

void StratoLogger::pull(uint8_t* buf, uint8_t len) {

	if(!readForAltitude()){
		Serial.println(F("No good"));
	}

	else {Serial.println(F("Success"));}

	if (buf && len){	// if one of these is zero it doesn't exist

		noInterrupts(); //ATOMIC_BLOCK_START;
		memcpy(buf, currentAltReading, sizeof(currentAltReading));
		interrupts(); //ATOMIC_BLOCK_END;
	}
}

/*
 * Pull data from the StratoLogger
 * @param buf is the buffer to which the altitude data will be copied,
 * 		should be 2 bytes for int16_t data type
 *
 */
void StratoLogger::pull(uint8_t* buf){
	this->pull(buf, 2);		// two bytes, altitude reading is in 10,000ft range
}


void StratoLogger::copylastSerialBuff(uint8_t* buf){
//	resetSerialBuff();
//
//	while(stratoLogger.available() > 0){
//		lastSerialBuff[stratoLogger.available()] = stratoLogger.read();
//	}
	noInterrupts(); //ATOMIC_BLOCK_START;
	memcpy(buf, lastSerialBuff, sizeof(lastSerialBuff));
	interrupts(); //ATOMIC_BLOCK_END;
}


/**
 * Clear the serial available buffer and fill it with zeros
 */
void StratoLogger::resetSerialBuff(){
	for(uint8_t i = 0; i < sizeof(lastSerialBuff); i++){
		lastSerialBuff[i] = 0;
	}
}

uint16_t StratoLogger::getReadingsCount(){
	return readingsCount;
}

int StratoLogger::getSerialBuffAvail(){
	return serialBuffAvail;
}

int StratoLogger::serAvailVal(){

	return stratoLogger.available();
}

bool StratoLogger::getIsValidSerialMsg(){
	return validSerialMsg;
}
