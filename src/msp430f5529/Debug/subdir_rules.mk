################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_20.2.3.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs1011/ccs/ccs_base/msp430/include" --include_path="/Users/hannes/Documents/GitHub/ekg-project/src/msp430f5529/includes/config" --include_path="/Users/hannes/Documents/GitHub/ekg-project/src/msp430f5529/includes/driverlib" --include_path="/Users/hannes/Documents/GitHub/ekg-project/src/msp430f5529" --include_path="/Applications/ti/ccs1011/ccs/tools/compiler/ti-cgt-msp430_20.2.3.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


