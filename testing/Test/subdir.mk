################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Test/test.c 

C_DEPS += \
./Test/test.d 

OBJS += \
./Test/test.o 


# Each subdirectory must supply rules for building sources it contributes
Test/%.o: ../Test/%.c Test/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Test

clean-Test:
	-$(RM) ./Test/test.d ./Test/test.o

.PHONY: clean-Test

