################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chord/chord.c \
../src/chord/chordcor.c \
../src/chord/ehpcp.c \
../src/chord/fft.c 

OBJS += \
./src/chord/chord.o \
./src/chord/chordcor.o \
./src/chord/ehpcp.o \
./src/chord/fft.o 

C_DEPS += \
./src/chord/chord.d \
./src/chord/chordcor.d \
./src/chord/ehpcp.d \
./src/chord/fft.d 


# Each subdirectory must supply rules for building sources it contributes
src/chord/%.o: ../src/chord/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


