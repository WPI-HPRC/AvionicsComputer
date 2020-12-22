# WPI HPRC Avionics Computer
The code base is a firmware designed in the Arduino environment with C++ to opperate avionics computers within a high power rocket.  In general, these systems estimate the vehicle's state, log data, and control other vehicle subsystems.  This includes flight control systems, ground stations, and some payloads. 

## Structure
At the base level of Avionics Computer is Peripherals.   These interface directly with the hardware.  They access sensors and actuators via serial communications like SPI and I2C as well as using PWM and GPIO pins.  No additonal functions beyond what is required to read or opperate the Peripherals are included.  Examples include the GPS, flash memory, and motor.

Peripherals are utilized by Subystems.  Subsystems are responsible for specific functions of the computer and manage their own state.  For example, the Airbrakes subsystem is responsible for ensuring the vehicle reaches a target altitude precisely.  It runs a control algorithm with input from the state estimator and actuates servos to apply the control algorithm.  The Airbrakes subsystem uses state estimator data to determine when it is safe to activate and when its job is done.

Loops are used for frequently executed code.  Every Subsystem has its own loop, all though there are also independent loops such as the state estimator.  AvionicsComputer is managed by an overarching SystemManager object which holds Subsystems and Loops.  It uses the looper to execute loops at a set frequency.  Finally, the Constants file is used to manage the overall configuration of the system.  It defines peripheral pin addresses, code and physical constants, and active subsystems.

## Contributing
Avionics Computer is written using the Sloeber Plugin for Eclipse.  The project can be added within the IDE.

