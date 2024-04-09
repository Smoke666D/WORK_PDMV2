################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/subdir.mk
-include StdPeriphDriver/subdir.mk
-include LUA/subdir.mk
-include HW_LIB/subdir.mk
-include HAL/subdir.mk
-include FREE_RTOS/portable/MemMang/subdir.mk
-include FREE_RTOS/portable/GCC/subdir.mk
-include FREE_RTOS/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/subdir.mk
-include Application/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := PDM_V2
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
PDM_V2.hex \

SECONDARY_SIZE += \
PDM_V2.siz \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: PDM_V2.elf secondary-outputs

# Tool invocations
PDM_V2.elf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU Arm Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O3 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -g3 -T "C:/Users/i.dymov/Desktop/WORK/PROJECTS/PDM_V2/Project/APM32F4xxxG_FLASH.ld" -Xlinker --gc-sections -Wl,-Map,"PDM_V2.map" --specs=nano.specs --specs=nosys.specs -mfloat-abi=hard -mfpu=vfpv4-d16 -o "PDM_V2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

PDM_V2.hex: PDM_V2.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "PDM_V2.elf"  "PDM_V2.hex"
	@echo 'Finished building: $@'
	@echo ' '

PDM_V2.siz: PDM_V2.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU Arm Cross Print Size'
	arm-none-eabi-size --format=berkeley "PDM_V2.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) PDM_V2.elf PDM_V2.hex PDM_V2.siz
	-@echo ' '

post-build:
	-arm-none-eabi-objcopy -O binary "PDM_V2.elf" "PDM_V2.bin"; arm-none-eabi-objdump -D "PDM_V2.elf" > "PDM_V2.dump"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
