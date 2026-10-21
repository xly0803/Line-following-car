################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Hardware/MSPM0/%.o: ../Hardware/MSPM0/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Hardware" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Debug" -I"C:/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_10_00_04/source" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/System" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Hardware/MSPM0" -gdwarf-3 -Wall -MMD -MP -MF"Hardware/MSPM0/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Hardware/MSPM0/interrupt.o: ../Hardware/MSPM0/interrupt.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2100/ccs/tools/compiler/ti-cgt-armllvm_5.1.1.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Hardware" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Debug" -I"C:/ti/mspm0_sdk_2_10_00_04/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_10_00_04/source" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/System" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Hardware/MSPM0" -I"C:/Users/xuliangyun/workspace_ccstheia/xunjicar/Hardware/IMU660RB" -gdwarf-3 -Wall -MMD -MP -MF"Hardware/MSPM0/$(basename $(<F)).d_raw" -MT"Hardware/MSPM0/$(basename\ $(<F)).o"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


