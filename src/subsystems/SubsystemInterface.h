/*
 * SubsystemInterface.h
 *
 * Interface structure for a subsystem
 *
 * Created on: Dec 19, 2020
 * Author: Peter Dentch
 */

#ifndef SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_
#define SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_

#include "../loops/Looper.h"




class SubsystemInterface {

public:

	SubsystemInterface(){}
//		virtual ~SubsystemInterface(){}


	virtual void subsystemInit() = 0;

	virtual void zeroSensors() = 0;

	virtual void registerEnabledLoops(Looper * enabledLooper) = 0;

	virtual void printOutput() = 0;


};


#endif /* SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_ */
