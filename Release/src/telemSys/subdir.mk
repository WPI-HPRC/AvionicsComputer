################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\src\telemSys\EBaySys.cpp \
..\src\telemSys\GroundStationSys.cpp \
..\src\telemSys\RetentionSys.cpp 

LINK_OBJ += \
.\src\telemSys\EBaySys.cpp.o \
.\src\telemSys\GroundStationSys.cpp.o \
.\src\telemSys\RetentionSys.cpp.o 

CPP_DEPS += \
.\src\telemSys\EBaySys.cpp.d \
.\src\telemSys\GroundStationSys.cpp.d \
.\src\telemSys\RetentionSys.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
src\telemSys\EBaySys.cpp.o: ..\src\telemSys\EBaySys.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\telemSys\GroundStationSys.cpp.o: ..\src\telemSys\GroundStationSys.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\telemSys\RetentionSys.cpp.o: ..\src\telemSys\RetentionSys.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


