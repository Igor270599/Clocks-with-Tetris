################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Tetris/Tetris.c 

OBJS += \
./Application/Tetris/Tetris.o 

C_DEPS += \
./Application/Tetris/Tetris.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Tetris/%.o Application/Tetris/%.su Application/Tetris/%.cyclo: ../Application/Tetris/%.c Application/Tetris/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-Tetris

clean-Application-2f-Tetris:
	-$(RM) ./Application/Tetris/Tetris.cyclo ./Application/Tetris/Tetris.d ./Application/Tetris/Tetris.o ./Application/Tetris/Tetris.su

.PHONY: clean-Application-2f-Tetris

