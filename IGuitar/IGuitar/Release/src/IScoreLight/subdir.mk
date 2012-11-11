################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IScoreLight/Engines.cpp 

OBJS += \
./src/IScoreLight/Engines.o 

CPP_DEPS += \
./src/IScoreLight/Engines.d 


# Each subdirectory must supply rules for building sources it contributes
src/IScoreLight/%.o: ../src/IScoreLight/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


