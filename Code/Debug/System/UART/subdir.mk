################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../System/UART/UART.c 

OBJS += \
./System/UART/UART.o 

C_DEPS += \
./System/UART/UART.d 


# Each subdirectory must supply rules for building sources it contributes
System/UART/%.o System/UART/%.su System/UART/%.cyclo: ../System/UART/%.c System/UART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-System-2f-UART

clean-System-2f-UART:
	-$(RM) ./System/UART/UART.cyclo ./System/UART/UART.d ./System/UART/UART.o ./System/UART/UART.su

.PHONY: clean-System-2f-UART

