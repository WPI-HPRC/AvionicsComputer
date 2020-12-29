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



	virtual void stop();

	virtual void zeroSensors();

	virtual void registerEnabledLoops(Looper enabledLooper);

	virtual void debugOutput();


};


#endif /* SRC_SUBSYSTEMS_SUBSYSTEMINTERFACE_H_ */
