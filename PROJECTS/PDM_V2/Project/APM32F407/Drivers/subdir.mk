################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/adc_task.c \
../Drivers/can_task.c \
../Drivers/data.c \
../Drivers/din_task.c \
../Drivers/flash_task.c \
../Drivers/lua_lib.c \
../Drivers/lua_task.c \
../Drivers/usb_task.c 

C_DEPS += \
./Drivers/adc_task.d \
./Drivers/can_task.d \
./Drivers/data.d \
./Drivers/din_task.d \
./Drivers/flash_task.d \
./Drivers/lua_lib.d \
./Drivers/lua_task.d \
./Drivers/usb_task.d 

OBJS += \
./Drivers/adc_task.o \
./Drivers/can_task.o \
./Drivers/data.o \
./Drivers/din_task.o \
./Drivers/flash_task.o \
./Drivers/lua_lib.o \
./Drivers/lua_task.o \
./Drivers/usb_task.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c Drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\LUA" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -mfloat-abi=hard -mfpu=vfpv4-d16 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/adc_task.d ./Drivers/adc_task.o ./Drivers/can_task.d ./Drivers/can_task.o ./Drivers/data.d ./Drivers/data.o ./Drivers/din_task.d ./Drivers/din_task.o ./Drivers/flash_task.d ./Drivers/flash_task.o ./Drivers/lua_lib.d ./Drivers/lua_lib.o ./Drivers/lua_task.d ./Drivers/lua_task.o ./Drivers/usb_task.d ./Drivers/usb_task.o

.PHONY: clean-Drivers

