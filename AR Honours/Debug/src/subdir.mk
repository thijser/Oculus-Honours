################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AR\ Honours.cpp 

OBJS += \
./src/AR\ Honours.o 

CPP_DEPS += \
./src/AR\ Honours.d 


# Each subdirectory must supply rules for building sources it contributes
src/AR\ Honours.o: ../src/AR\ Honours.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/AR Honours.d" -MT"src/AR\ Honours.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


