################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IScoreLight/ECOMachine/PetriNet/Arc.cpp \
../src/IScoreLight/ECOMachine/PetriNet/ExtendedInt.cpp \
../src/IScoreLight/ECOMachine/PetriNet/PetriNet.cpp \
../src/IScoreLight/ECOMachine/PetriNet/PetriNetItem.cpp \
../src/IScoreLight/ECOMachine/PetriNet/PetriNetNode.cpp \
../src/IScoreLight/ECOMachine/PetriNet/Place.cpp \
../src/IScoreLight/ECOMachine/PetriNet/PriorityTransitionAction.cpp \
../src/IScoreLight/ECOMachine/PetriNet/ThreadSafeList.cpp \
../src/IScoreLight/ECOMachine/PetriNet/Token.cpp \
../src/IScoreLight/ECOMachine/PetriNet/Transition.cpp \
../src/IScoreLight/ECOMachine/PetriNet/TransitionAction.cpp \
../src/IScoreLight/ECOMachine/PetriNet/TransitionBitArray.cpp 

OBJS += \
./src/IScoreLight/ECOMachine/PetriNet/Arc.o \
./src/IScoreLight/ECOMachine/PetriNet/ExtendedInt.o \
./src/IScoreLight/ECOMachine/PetriNet/PetriNet.o \
./src/IScoreLight/ECOMachine/PetriNet/PetriNetItem.o \
./src/IScoreLight/ECOMachine/PetriNet/PetriNetNode.o \
./src/IScoreLight/ECOMachine/PetriNet/Place.o \
./src/IScoreLight/ECOMachine/PetriNet/PriorityTransitionAction.o \
./src/IScoreLight/ECOMachine/PetriNet/ThreadSafeList.o \
./src/IScoreLight/ECOMachine/PetriNet/Token.o \
./src/IScoreLight/ECOMachine/PetriNet/Transition.o \
./src/IScoreLight/ECOMachine/PetriNet/TransitionAction.o \
./src/IScoreLight/ECOMachine/PetriNet/TransitionBitArray.o 

CPP_DEPS += \
./src/IScoreLight/ECOMachine/PetriNet/Arc.d \
./src/IScoreLight/ECOMachine/PetriNet/ExtendedInt.d \
./src/IScoreLight/ECOMachine/PetriNet/PetriNet.d \
./src/IScoreLight/ECOMachine/PetriNet/PetriNetItem.d \
./src/IScoreLight/ECOMachine/PetriNet/PetriNetNode.d \
./src/IScoreLight/ECOMachine/PetriNet/Place.d \
./src/IScoreLight/ECOMachine/PetriNet/PriorityTransitionAction.d \
./src/IScoreLight/ECOMachine/PetriNet/ThreadSafeList.d \
./src/IScoreLight/ECOMachine/PetriNet/Token.d \
./src/IScoreLight/ECOMachine/PetriNet/Transition.d \
./src/IScoreLight/ECOMachine/PetriNet/TransitionAction.d \
./src/IScoreLight/ECOMachine/PetriNet/TransitionBitArray.d 


# Each subdirectory must supply rules for building sources it contributes
src/IScoreLight/ECOMachine/PetriNet/%.o: ../src/IScoreLight/ECOMachine/PetriNet/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


