################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Source/system_apm32f4xx.c 

S_UPPER_SRCS += \
C:/Users/i.dymov/Desktop/WORK/Libraries/Device/Geehy/APM32F4xx/Source/gcc/startup_apm32f40x.S 

C_DEPS += \
./CMSIS/system_apm32f4xx.d 

OBJS += \
./CMSIS/startup_apm32f40x.o \
./CMSIS/system_apm32f4xx.o 

S_UPPER_DEPS += \
./CMSIS/startup_apm32f40x.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/startup_apm32f40x.o: C:/Users/i.dymov/Desktop/WORK/Libraries/Device/Geehy/APM32F4xx/Source/gcc/startup_apm32f40x.S CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/system_apm32f4xx.o: C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Source/system_apm32f4xx.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\LUA" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -mfloat-abi=hard -mfpu=vfpv4-d16 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/startup_apm32f40x.d ./CMSIS/startup_apm32f40x.o ./CMSIS/system_apm32f4xx.d ./CMSIS/system_apm32f4xx.o

.PHONY: clean-CMSIS

