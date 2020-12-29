/**
 * AvionicsComputer.ino
 * Created on: Dec 20, 2020
 * Author: Peter Dentch
 *
 * Main .ino program file
 * Code developed for Teensy 3.2
 *
 */

#include "Arduino.h"

#include "SystemManager.h"


SystemManager * sysMan;


// The setup routine runs once when you press reset:
void setup() {

	sysMan = new SystemManager();

}


// The loop routine runs over and over again forever:
void loop() {

	sysMan->mainLoop();

}
