################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.c 

COMPILED_SRCS += \
libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.src 

C_DEPS += \
libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.d 

OBJS += \
libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.o 


# Each subdirectory must supply rules for building sources it contributes
libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.src: ../libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.c libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/subdir.mk
	cctc -cs --dep-file="$(*F).d" --misrac-version=2004 -D__CPU__=tc37x "-fD:/ADS/ADS_TC377/Seekfree_TC377_Opensource_Library/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=1 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.o: libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.src libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/subdir.mk
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Stm-2f-Std

clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Stm-2f-Std:
	-$(RM) libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.d libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.o libraries/infineon_libraries/iLLD/TC37A/Tricore/Stm/Std/IfxStm.src

.PHONY: clean-libraries-2f-infineon_libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Stm-2f-Std

