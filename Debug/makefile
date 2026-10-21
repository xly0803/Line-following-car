################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS

GEN_OPTS__FLAG := @"./device.opt" 
GEN_CMDS__FLAG := -Wl,-l"./device_linker.cmd" 

ORDERED_OBJS += \
"./empty.o" \
"./ti_msp_dl_config.o" \
"./startup_mspm0g350x_ticlang.o" \
"./Hardware/AngleClosedloop.o" \
"./Hardware/Encoder.o" \
"./Hardware/FollowLine.o" \
"./Hardware/Key.o" \
"./Hardware/MPU6050.o" \
"./Hardware/Motor.o" \
"./Hardware/MyI2C.o" \
"./Hardware/OLED.o" \
"./Hardware/OLED_Data.o" \
"./Hardware/PID.o" \
"./Hardware/PWM.o" \
"./Hardware/IMU660RB/FusionAhrs.o" \
"./Hardware/IMU660RB/FusionCompass.o" \
"./Hardware/IMU660RB/FusionOffset.o" \
"./Hardware/IMU660RB/imu660rb.o" \
"./Hardware/IMU660RB/lsm6dsr_reg.o" \
"./Hardware/MSPM0/clock.o" \
"./Hardware/MSPM0/interrupt.o" \
"./System/Delay.o" \
$(GEN_CMDS__FLAG) \
-Wl,-ldevice.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Hardware/subdir_vars.mk
-include Hardware/IMU660RB/subdir_vars.mk
-include Hardware/MSPM0/subdir_vars.mk
-include System/subdir_vars.mk
-include subdir_rules.mk
-include Hardware/subdir_rules.mk
-include Hardware/IMU660RB/subdir_rules.mk
-include Hardware/MSPM0/subdir_rules.mk
-include System/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
xunjicar.out 

EXE_OUTPUTS__QUOTED += \
"xunjicar.out" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "xunjicar.out"

# Tool invocations
xunjicar.out: $(OBJS) $(GEN_CMDS)
	@echo 'Arm Linker - building target: "$@"'
	"C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang.exe" @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -gdwarf-3 -Wall -Wl,-m"xunjicar.map" -Wl,-i"C:/ti/mspm0_sdk_2_10_00_04/source" -Wl,-i"C:/Users/xuliangyun/workspace_ccstheia/xunjicar" -Wl,-i"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Debug/syscfg" -Wl,-i"C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="xunjicar_linkInfo.xml" -Wl,--rom_model -o "xunjicar.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "empty.o" "ti_msp_dl_config.o" "startup_mspm0g350x_ticlang.o" "Hardware\AngleClosedloop.o" "Hardware\Encoder.o" "Hardware\FollowLine.o" "Hardware\Key.o" "Hardware\MPU6050.o" "Hardware\Motor.o" "Hardware\MyI2C.o" "Hardware\OLED.o" "Hardware\OLED_Data.o" "Hardware\PID.o" "Hardware\PWM.o" "Hardware\IMU660RB\FusionAhrs.o" "Hardware\IMU660RB\FusionCompass.o" "Hardware\IMU660RB\FusionOffset.o" "Hardware\IMU660RB\imu660rb.o" "Hardware\IMU660RB\lsm6dsr_reg.o" "Hardware\MSPM0\clock.o" "Hardware\MSPM0\interrupt.o" "System\Delay.o" 
	-$(RM) "empty.d" "ti_msp_dl_config.d" "startup_mspm0g350x_ticlang.d" "Hardware\AngleClosedloop.d" "Hardware\Encoder.d" "Hardware\FollowLine.d" "Hardware\Key.d" "Hardware\MPU6050.d" "Hardware\Motor.d" "Hardware\MyI2C.d" "Hardware\OLED.d" "Hardware\OLED_Data.d" "Hardware\PID.d" "Hardware\PWM.d" "Hardware\IMU660RB\FusionAhrs.d" "Hardware\IMU660RB\FusionCompass.d" "Hardware\IMU660RB\FusionOffset.d" "Hardware\IMU660RB\imu660rb.d" "Hardware\IMU660RB\lsm6dsr_reg.d" "Hardware\MSPM0\clock.d" "Hardware\MSPM0\interrupt.d" "System\Delay.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

