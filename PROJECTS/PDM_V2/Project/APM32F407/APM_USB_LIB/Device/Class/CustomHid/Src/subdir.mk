################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/i.dymov/Desktop/WORK/Middlewares/APM32_USB_Library/Device/Class/CustomHID/Src/usbd_customhid.c 

C_DEPS += \
./APM_USB_LIB/Device/Class/CustomHid/Src/usbd_customhid.d 

OBJS += \
./APM_USB_LIB/Device/Class/CustomHid/Src/usbd_customhid.o 


# Each subdirectory must supply rules for building sources it contributes
APM_USB_LIB/Device/Class/CustomHid/Src/usbd_customhid.o: C:/Users/i.dymov/Desktop/WORK/Middlewares/APM32_USB_Library/Device/Class/CustomHID/Src/usbd_customhid.c APM_USB_LIB/Device/Class/CustomHid/Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\LUA" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-APM_USB_LIB-2f-Device-2f-Class-2f-CustomHid-2f-Src

clean-APM_USB_LIB-2f-Device-2f-Class-2f-CustomHid-2f-Src:
	-$(RM) ./APM_USB_LIB/Device/Class/CustomHid/Src/usbd_customhid.d ./APM_USB_LIB/Device/Class/CustomHid/Src/usbd_customhid.o

.PHONY: clean-APM_USB_LIB-2f-Device-2f-Class-2f-CustomHid-2f-Src

