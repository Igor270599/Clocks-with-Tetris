################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/MP3_Player_TD5580/MP3_Player.c 

OBJS += \
./Application/MP3_Player_TD5580/MP3_Player.o 

C_DEPS += \
./Application/MP3_Player_TD5580/MP3_Player.d 


# Each subdirectory must supply rules for building sources it contributes
Application/MP3_Player_TD5580/%.o Application/MP3_Player_TD5580/%.su Application/MP3_Player_TD5580/%.cyclo: ../Application/MP3_Player_TD5580/%.c Application/MP3_Player_TD5580/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-MP3_Player_TD5580

clean-Application-2f-MP3_Player_TD5580:
	-$(RM) ./Application/MP3_Player_TD5580/MP3_Player.cyclo ./Application/MP3_Player_TD5580/MP3_Player.d ./Application/MP3_Player_TD5580/MP3_Player.o ./Application/MP3_Player_TD5580/MP3_Player.su

.PHONY: clean-Application-2f-MP3_Player_TD5580

