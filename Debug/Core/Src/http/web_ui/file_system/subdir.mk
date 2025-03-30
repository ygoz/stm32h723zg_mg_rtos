################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/http/web_ui/file_system/packed_fs.c 

OBJS += \
./Core/Src/http/web_ui/file_system/packed_fs.o 

C_DEPS += \
./Core/Src/http/web_ui/file_system/packed_fs.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/http/web_ui/file_system/%.o Core/Src/http/web_ui/file_system/%.su Core/Src/http/web_ui/file_system/%.cyclo: ../Core/Src/http/web_ui/file_system/%.c Core/Src/http/web_ui/file_system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-http-2f-web_ui-2f-file_system

clean-Core-2f-Src-2f-http-2f-web_ui-2f-file_system:
	-$(RM) ./Core/Src/http/web_ui/file_system/packed_fs.cyclo ./Core/Src/http/web_ui/file_system/packed_fs.d ./Core/Src/http/web_ui/file_system/packed_fs.o ./Core/Src/http/web_ui/file_system/packed_fs.su

.PHONY: clean-Core-2f-Src-2f-http-2f-web_ui-2f-file_system

