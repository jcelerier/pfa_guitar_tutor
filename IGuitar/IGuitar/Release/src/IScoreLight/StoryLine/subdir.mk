################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IScoreLight/StoryLine/StoryLine.cpp 

OBJS += \
./src/IScoreLight/StoryLine/StoryLine.o 

CPP_DEPS += \
./src/IScoreLight/StoryLine/StoryLine.d 


# Each subdirectory must supply rules for building sources it contributes
src/IScoreLight/StoryLine/%.o: ../src/IScoreLight/StoryLine/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


