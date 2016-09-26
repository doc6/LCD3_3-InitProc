################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
LCD3_3-InitProc.lss \

FLASH_IMAGE += \
LCD3_3-InitProc.hex \

EEPROM_IMAGE += \
LCD3_3-InitProc.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: LCD3_3-InitProc.elf secondary-outputs

# Tool invocations
LCD3_3-InitProc.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,LCD3_3-InitProc.map -mmcu=atmega328p -o "LCD3_3-InitProc.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD3_3-InitProc.lss: LCD3_3-InitProc.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S LCD3_3-InitProc.elf  >"LCD3_3-InitProc.lss"
	@echo 'Finished building: $@'
	@echo ' '

LCD3_3-InitProc.hex: LCD3_3-InitProc.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex LCD3_3-InitProc.elf  "LCD3_3-InitProc.hex"
	@echo 'Finished building: $@'
	@echo ' '

LCD3_3-InitProc.eep: LCD3_3-InitProc.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex LCD3_3-InitProc.elf  "LCD3_3-InitProc.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: LCD3_3-InitProc.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p LCD3_3-InitProc.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) LCD3_3-InitProc.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
