################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_adc_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_adc_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_can_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_can_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_dma_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_dma_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_flash_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_flash_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_gpio_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_gpio_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_i2c_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_rtc.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_spi_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_timers_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_timers_ch32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_wdt_apm32.c \
C:/Users/i.dymov/Desktop/WORK/HAL/hal_wdt_ch32.c 

C_DEPS += \
./HAL/hal_adc_apm32.d \
./HAL/hal_adc_ch32.d \
./HAL/hal_can_apm32.d \
./HAL/hal_can_ch32.d \
./HAL/hal_dma_apm32.d \
./HAL/hal_dma_ch32.d \
./HAL/hal_flash_apm32.d \
./HAL/hal_flash_ch32.d \
./HAL/hal_gpio_apm32.d \
./HAL/hal_gpio_ch32.d \
./HAL/hal_i2c_apm32.d \
./HAL/hal_rtc.d \
./HAL/hal_spi_ch32.d \
./HAL/hal_timers_apm32.d \
./HAL/hal_timers_ch32.d \
./HAL/hal_wdt_apm32.d \
./HAL/hal_wdt_ch32.d 

OBJS += \
./HAL/hal_adc_apm32.o \
./HAL/hal_adc_ch32.o \
./HAL/hal_can_apm32.o \
./HAL/hal_can_ch32.o \
./HAL/hal_dma_apm32.o \
./HAL/hal_dma_ch32.o \
./HAL/hal_flash_apm32.o \
./HAL/hal_flash_ch32.o \
./HAL/hal_gpio_apm32.o \
./HAL/hal_gpio_ch32.o \
./HAL/hal_i2c_apm32.o \
./HAL/hal_rtc.o \
./HAL/hal_spi_ch32.o \
./HAL/hal_timers_apm32.o \
./HAL/hal_timers_ch32.o \
./HAL/hal_wdt_apm32.o \
./HAL/hal_wdt_ch32.o 


# Each subdirectory must supply rules for building sources it contributes
HAL/hal_adc_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_adc_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_adc_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_adc_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_can_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_can_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_can_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_can_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_dma_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_dma_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_dma_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_dma_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_flash_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_flash_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_flash_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_flash_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_gpio_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_gpio_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_gpio_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_gpio_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_i2c_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_i2c_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_rtc.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_rtc.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_spi_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_spi_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_timers_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_timers_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_timers_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_timers_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_wdt_apm32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_wdt_apm32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/hal_wdt_ch32.o: C:/Users/i.dymov/Desktop/WORK/HAL/hal_wdt_ch32.c HAL/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O1 -ffunction-sections -fdata-sections -Wall -g3 -DAPM32F40X -I"C:\Users\i.dymov\Desktop\WORK\DRIVER" -I"C:\Users\i.dymov\Desktop\WORK\USB_DEV" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Core\Inc" -I"C:\Users\i.dymov\Desktop\WORK\Middlewares\APM32_USB_Library\Device\Class\CustomHID\Inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\Device\Geehy\APM32F4xx\Include" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\Drivers" -I"C:\Users\i.dymov\Desktop\WORK\PROJECTS\PDM_V2\Project\system" -I"C:\Users\i.dymov\Desktop\WORK\HAL" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\APM32F4xx_StdPeriphDriver\inc" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Libraries\CMSIS\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Boards" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\Include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\include" -I"C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project\..\..\..\Middlewares\FreeRTOS\FreeRTOSv202012.00\FreeRTOS\Source\portable\GCC\ARM_CM4F" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-HAL

clean-HAL:
	-$(RM) ./HAL/hal_adc_apm32.d ./HAL/hal_adc_apm32.o ./HAL/hal_adc_ch32.d ./HAL/hal_adc_ch32.o ./HAL/hal_can_apm32.d ./HAL/hal_can_apm32.o ./HAL/hal_can_ch32.d ./HAL/hal_can_ch32.o ./HAL/hal_dma_apm32.d ./HAL/hal_dma_apm32.o ./HAL/hal_dma_ch32.d ./HAL/hal_dma_ch32.o ./HAL/hal_flash_apm32.d ./HAL/hal_flash_apm32.o ./HAL/hal_flash_ch32.d ./HAL/hal_flash_ch32.o ./HAL/hal_gpio_apm32.d ./HAL/hal_gpio_apm32.o ./HAL/hal_gpio_ch32.d ./HAL/hal_gpio_ch32.o ./HAL/hal_i2c_apm32.d ./HAL/hal_i2c_apm32.o ./HAL/hal_rtc.d ./HAL/hal_rtc.o ./HAL/hal_spi_ch32.d ./HAL/hal_spi_ch32.o ./HAL/hal_timers_apm32.d ./HAL/hal_timers_apm32.o ./HAL/hal_timers_ch32.d ./HAL/hal_timers_ch32.o ./HAL/hal_wdt_apm32.d ./HAL/hal_wdt_apm32.o ./HAL/hal_wdt_ch32.d ./HAL/hal_wdt_ch32.o

.PHONY: clean-HAL

