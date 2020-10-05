#include "Arduino.h"
#include "src/peripheral/LoRaCustom.h"
#include "src/peripheral/StratoLogger.h"
#include "src/peripheral/GyroAccel.h"

/*	----- PINS IN USE UNDER THE HOOD -----
 *
 *		these are hard-coded for the radio SPI and shouldn't ever change
 *	RF95 LoRa SLAVE_SELECT 		4
 *	RF95 LoRa INTERRUPT_PIN 	3
 *
 */

	// Global fields
LoRaComms myRadio;

uint8_t message[12];	//
//uint8_t recvMsg[12];
//uint8_t len = sizeof(recvMsg);

//int count = 0;
//unsigned long time;

StratoLogger strat;
uint16_t alt;
uint8_t altitude[2];
uint16_t lastTxGood = 0;
uint8_t test[8]; // for strato buffer

GyroAccel mpu6050;
uint16_t gyroAccData[6];

// the setup routine runs once when you press reset:
void setup() {

	Serial.begin(9600);		// Arduino complains if this isn't done in setup


	myRadio.init();			// All SPI setup/pin binding takes place inside the class
//	message[0] = 0;

	mpu6050.gyroCalibrate();

	Serial.println(F("Setup ran"));

}

// the loop routine runs over and over again forever:
void loop() {

		mpu6050.pull(message, 12);


		//strat.pull(altitude);
//		int val = strat.getSerialBuffAvail();
//		Serial.println(val);
//		uint16_t val = strat.getReadingsCount();
//		Serial.println(val);

//		strat.copylastSerialBuff(test);
//		Serial.print(test[0]); Serial.print(" "); Serial.print(test[1]); Serial.print(" "); Serial.print(test[2]); Serial.print(" ");
//		Serial.print(test[3]); Serial.print(" "); Serial.print(test[4]); Serial.print(" "); Serial.print(test[5]); Serial.print(" ");
//		Serial.print(test[6]); Serial.print(" "); Serial.println(test[7]);


		//message[0] = altitude[0];	// beginning of array is MSB
		//message[1] = altitude[1];	// higher index in array is LSB

//		if(val != -1){// && val != 13 && val != 10){
//			message[0] = val;
//		}

//		message[0] = test[0]; message[1] = test[1]; message[2] = test[2];
//		message[3] = test[3]; message[4] = test[4]; message[5] = test[5];
//		message[6] = test[6]; message[7] = test[7];

//		Serial.print(altitude[0]); Serial.print(" ");Serial.println(altitude[1]);

		//alt = altitude[0] <<8| altitude[1];	// shift MSB to beginning byte, append with LSB end byte
//		Serial.print("Altitude: "); Serial.println(alt);

//		time = millis();

//		while(myRadio.getTxGood() != lastTxGood){}	//TODO: delay for transmitting, is this viable?
//		if(!strat.getIsValidSerialMsg())
//			alt = 0;	//TODO: is this viable??

		if(myRadio.getMode() == RadioState::RHModeIdle){
			myRadio.send(message, 12);

			Serial.println(message[0]);
			//Serial.println(alt);
			//Serial.println(F("Sent"));

//			lastTxGood++;	// set to the next TxGood count which the radio should soon have

//			Serial.print("mode is: "); Serial.println(myRadio.getMode());
//			Serial.print("count: "); Serial.println(myRadio._interruptCount);
//			Serial.print("RadioTxGood: "); Serial.println(myRadio.getTxGood());


			//message[0] = count;
			//count++;
		}

//		Serial.println(F("Looped -------------"));
		//delay(40);


//	if(myRadio.getReceived(recvMsg, &len)){		// if valid buffer is set true, new message is available to copy!!!
//
//		Serial.println(recvMsg[0]);//Serial.println(recvMsg[1]); Serial.println(recvMsg[2]); Serial.println(recvMsg[3]);
//		//Serial.println(recvMsg[4]);Serial.println(recvMsg[5]); Serial.println(recvMsg[6]); Serial.println(recvMsg[7]);
//		//Serial.println(recvMsg[8]);Serial.println(recvMsg[9]); Serial.println(recvMsg[10]); Serial.println(recvMsg[11]);
//
//		Serial.println(myRadio.getLastSNR());
//		Serial.println(myRadio.getLastRSSI());
//
//		//Serial.print("RX Good: ");
//		Serial.println(myRadio.getRxGood());
//
////		Serial.print("*"); Serial.println(millis() - time);
//	}







//	Serial.println(F("----------------------"));
//	Serial.println(myRadio.getLastSNR());
//	Serial.println(myRadio.getLastRSSI());

//	Serial.print("buff: "); Serial.println(myRadio.available());

//	Serial.print("mode is: "); Serial.println(myRadio.getMode());

//	Serial.print("x good: "); Serial.println(myRadio.getRxGood());

//	Serial.print("count: "); Serial.println(myRadio._interruptCount);
//	delay(500);

}


//}
