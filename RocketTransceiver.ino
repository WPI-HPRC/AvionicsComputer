#include "Arduino.h"





// The setup routine runs once when you press reset:
void setup() {

	Serial.begin(9600);		// issues sometime arise if this isn't done in setup

	while(!Serial);			// loop until serial port is opened, stalls program






	Serial.println(F("Setup ran"));
}



// The loop routine runs over and over again forever:
void loop() {





}
