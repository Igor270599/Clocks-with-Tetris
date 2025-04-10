################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Display/Display.c 

OBJS += \
./Application/Display/Display.o 

C_DEPS += \
./Application/Display/Display.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Display/%.o Application/Display/%.su Application/Display/%.cyclo: ../Application/Display/%.c Application/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-Display

clean-Application-2f-Display:
	-$(RM) ./Application/Display/Display.cyclo ./Application/Display/Display.d ./Application/Display/Display.o ./Application/Display/Display.su

.PHONY: clean-Application-2f-Display

