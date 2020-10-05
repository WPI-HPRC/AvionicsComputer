################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\src\peripheral\DataLogger.cpp \
..\src\peripheral\Gps.cpp \
..\src\peripheral\GyroAccel.cpp \
..\src\peripheral\LoRaCustom.cpp \
..\src\peripheral\OutputSerial.cpp \
..\src\peripheral\StratoLogger.cpp 

LINK_OBJ += \
.\src\peripheral\DataLogger.cpp.o \
.\src\peripheral\Gps.cpp.o \
.\src\peripheral\GyroAccel.cpp.o \
.\src\peripheral\LoRaCustom.cpp.o \
.\src\peripheral\OutputSerial.cpp.o \
.\src\peripheral\StratoLogger.cpp.o 

CPP_DEPS += \
.\src\peripheral\DataLogger.cpp.d \
.\src\peripheral\Gps.cpp.d \
.\src\peripheral\GyroAccel.cpp.d \
.\src\peripheral\LoRaCustom.cpp.d \
.\src\peripheral\OutputSerial.cpp.d \
.\src\peripheral\StratoLogger.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
src\peripheral\DataLogger.cpp.o: ..\src\peripheral\DataLogger.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\peripheral\Gps.cpp.o: ..\src\peripheral\Gps.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\peripheral\GyroAccel.cpp.o: ..\src\peripheral\GyroAccel.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\peripheral\LoRaCustom.cpp.o: ..\src\peripheral\LoRaCustom.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\peripheral\OutputSerial.cpp.o: ..\src\peripheral\OutputSerial.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

src\peripheral\StratoLogger.cpp.o: ..\src\peripheral\StratoLogger.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\RBE\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR     -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\eightanaloginputs" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SPI\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\SoftwareSerial\src" -I"C:\RBE\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\Wire\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


