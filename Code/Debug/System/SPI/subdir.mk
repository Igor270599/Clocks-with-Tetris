################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../System/SPI/SPI.c 

OBJS += \
./System/SPI/SPI.o 

C_DEPS += \
./System/SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
System/SPI/%.o System/SPI/%.su System/SPI/%.cyclo: ../System/SPI/%.c System/SPI/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-System-2f-SPI

clean-System-2f-SPI:
	-$(RM) ./System/SPI/SPI.cyclo ./System/SPI/SPI.d ./System/SPI/SPI.o ./System/SPI/SPI.su

.PHONY: clean-System-2f-SPI

