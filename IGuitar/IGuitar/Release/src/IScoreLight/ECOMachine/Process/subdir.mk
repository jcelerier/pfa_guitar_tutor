################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IScoreLight/ECOMachine/Process/MultiTypeCurve.cpp \
../src/IScoreLight/ECOMachine/Process/NetworkMessageCurves.cpp \
../src/IScoreLight/ECOMachine/Process/Process.cpp \
../src/IScoreLight/ECOMachine/Process/SendNetworkMessageProcess.cpp \
../src/IScoreLight/ECOMachine/Process/StringParser.cpp \
../src/IScoreLight/ECOMachine/Process/StringTypeFunctions.cpp 

OBJS += \
./src/IScoreLight/ECOMachine/Process/MultiTypeCurve.o \
./src/IScoreLight/ECOMachine/Process/NetworkMessageCurves.o \
./src/IScoreLight/ECOMachine/Process/Process.o \
./src/IScoreLight/ECOMachine/Process/SendNetworkMessageProcess.o \
./src/IScoreLight/ECOMachine/Process/StringParser.o \
./src/IScoreLight/ECOMachine/Process/StringTypeFunctions.o 

CPP_DEPS += \
./src/IScoreLight/ECOMachine/Process/MultiTypeCurve.d \
./src/IScoreLight/ECOMachine/Process/NetworkMessageCurves.d \
./src/IScoreLight/ECOMachine/Process/Process.d \
./src/IScoreLight/ECOMachine/Process/SendNetworkMessageProcess.d \
./src/IScoreLight/ECOMachine/Process/StringParser.d \
./src/IScoreLight/ECOMachine/Process/StringTypeFunctions.d 


# Each subdirectory must supply rules for building sources it contributes
src/IScoreLight/ECOMachine/Process/%.o: ../src/IScoreLight/ECOMachine/Process/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


