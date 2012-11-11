################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IScoreLight/Editor/Editor.cpp 

OBJS += \
./src/IScoreLight/Editor/Editor.o 

CPP_DEPS += \
./src/IScoreLight/Editor/Editor.d 


# Each subdirectory must supply rules for building sources it contributes
src/IScoreLight/Editor/%.o: ../src/IScoreLight/Editor/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


