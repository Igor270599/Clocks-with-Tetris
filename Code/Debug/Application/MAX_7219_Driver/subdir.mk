################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/MAX_7219_Driver/MAX_7219.c 

OBJS += \
./Application/MAX_7219_Driver/MAX_7219.o 

C_DEPS += \
./Application/MAX_7219_Driver/MAX_7219.d 


# Each subdirectory must supply rules for building sources it contributes
Application/MAX_7219_Driver/%.o Application/MAX_7219_Driver/%.su Application/MAX_7219_Driver/%.cyclo: ../Application/MAX_7219_Driver/%.c Application/MAX_7219_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-MAX_7219_Driver

clean-Application-2f-MAX_7219_Driver:
	-$(RM) ./Application/MAX_7219_Driver/MAX_7219.cyclo ./Application/MAX_7219_Driver/MAX_7219.d ./Application/MAX_7219_Driver/MAX_7219.o ./Application/MAX_7219_Driver/MAX_7219.su

.PHONY: clean-Application-2f-MAX_7219_Driver

