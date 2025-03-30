################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/http/FOTA/firmware_update.c 

OBJS += \
./Core/Src/http/FOTA/firmware_update.o 

C_DEPS += \
./Core/Src/http/FOTA/firmware_update.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/http/FOTA/%.o Core/Src/http/FOTA/%.su Core/Src/http/FOTA/%.cyclo: ../Core/Src/http/FOTA/%.c Core/Src/http/FOTA/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-http-2f-FOTA

clean-Core-2f-Src-2f-http-2f-FOTA:
	-$(RM) ./Core/Src/http/FOTA/firmware_update.cyclo ./Core/Src/http/FOTA/firmware_update.d ./Core/Src/http/FOTA/firmware_update.o ./Core/Src/http/FOTA/firmware_update.su

.PHONY: clean-Core-2f-Src-2f-http-2f-FOTA

