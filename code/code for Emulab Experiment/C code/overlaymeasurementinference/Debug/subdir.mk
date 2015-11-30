################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../core.c \
../dynarray.c \
../filereader.c \
../iterator.c \
../linkstructure.c 

OBJS += \
./core.o \
./dynarray.o \
./filereader.o \
./iterator.o \
./linkstructure.o 

C_DEPS += \
./core.d \
./dynarray.d \
./filereader.d \
./iterator.d \
./linkstructure.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


