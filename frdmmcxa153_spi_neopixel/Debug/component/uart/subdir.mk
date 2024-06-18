################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/fsl_adapter_lpuart.c 

C_DEPS += \
./component/uart/fsl_adapter_lpuart.d 

OBJS += \
./component/uart/fsl_adapter_lpuart.o 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c component/uart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXA153VLH -DCPU_MCXA153VLH_cm33_nodsp -DMCUXPRESSO_SDK -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\source" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\drivers" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\device" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\utilities" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\component\uart" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\component\lists" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\CMSIS" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\board" -I"C:\Users\Jerry\Documents\MCUXpressoIDE_11.9.0_2144\workspace\frdmmcxa153_lpspi_edma_b2b_transfer_master\frdmmcxa153\driver_examples\lpspi\edma_b2b_transfer\master" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33+nodsp -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-uart

clean-component-2f-uart:
	-$(RM) ./component/uart/fsl_adapter_lpuart.d ./component/uart/fsl_adapter_lpuart.o

.PHONY: clean-component-2f-uart

