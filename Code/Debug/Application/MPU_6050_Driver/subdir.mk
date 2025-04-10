################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/MPU_6050_Driver/mpu6050.c 

OBJS += \
./Application/MPU_6050_Driver/mpu6050.o 

C_DEPS += \
./Application/MPU_6050_Driver/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
Application/MPU_6050_Driver/%.o Application/MPU_6050_Driver/%.su Application/MPU_6050_Driver/%.cyclo: ../Application/MPU_6050_Driver/%.c Application/MPU_6050_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-MPU_6050_Driver

clean-Application-2f-MPU_6050_Driver:
	-$(RM) ./Application/MPU_6050_Driver/mpu6050.cyclo ./Application/MPU_6050_Driver/mpu6050.d ./Application/MPU_6050_Driver/mpu6050.o ./Application/MPU_6050_Driver/mpu6050.su

.PHONY: clean-Application-2f-MPU_6050_Driver

