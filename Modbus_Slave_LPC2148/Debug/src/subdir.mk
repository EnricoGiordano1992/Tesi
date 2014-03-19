################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/mb.c \
../src/mbascii.c \
../src/mbcrc.c \
../src/mbfunccoils.c \
../src/mbfuncdiag.c \
../src/mbfuncdisc.c \
../src/mbfuncholding.c \
../src/mbfuncinput.c \
../src/mbfuncother.c \
../src/mbrtu.c \
../src/mbutils.c \
../src/port.c \
../src/portevent.c \
../src/portserial.c \
../src/porttimer.c 

S_SRCS += \
../src/cr_startup_lpc21.s 

OBJS += \
./src/cr_startup_lpc21.o \
./src/main.o \
./src/mb.o \
./src/mbascii.o \
./src/mbcrc.o \
./src/mbfunccoils.o \
./src/mbfuncdiag.o \
./src/mbfuncdisc.o \
./src/mbfuncholding.o \
./src/mbfuncinput.o \
./src/mbfuncother.o \
./src/mbrtu.o \
./src/mbutils.o \
./src/port.o \
./src/portevent.o \
./src/portserial.o \
./src/porttimer.o 

C_DEPS += \
./src/main.d \
./src/mb.d \
./src/mbascii.d \
./src/mbcrc.d \
./src/mbfunccoils.d \
./src/mbfuncdiag.d \
./src/mbfuncdisc.d \
./src/mbfuncholding.d \
./src/mbfuncinput.d \
./src/mbfuncother.d \
./src/mbrtu.d \
./src/mbutils.d \
./src/port.d \
./src/portevent.d \
./src/portserial.d \
./src/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -DDEBUG -D__CODE_RED -g3 -mcpu=arm7tdmi -D__NEWLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DDEBUG -D__CODE_RED -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=arm7tdmi -D__NEWLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


