PROGNAME=sdd
BUILD_DIR = build
TARGET_DEVICE = STM32F100xB

#OBJECTS=main.o handlers.o startup_stm32f10x_md.o hal.o systick.o gpios.o syscalls.o

F_SYSCLK  = 24000000UL
F_PCLK1   = 24000000UL
F_SYSTICK =     1000UL


DEBUG = 0
OPT = -Os

#######################################
# C sources
#######################################
C_SOURCES = \
system.c \
main.c \
gpios.c \
systick.c \
usart1.c \
spi1.c \

#ustimer.c \
#handlers.c


# ASM sources
ASM_SOURCES = \
startup_stm32f100xb.s
#startup_stm32f10x_md.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
#AS = $(PREFIX)as
#LD = $(PREFIX)ld
CP = $(PREFIX)objcopy
#OD = $(PREFIX)objdump
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)


# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = -DSTM32F100xB

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES = -I.

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -DF_SYSCLK=$(F_SYSCLK)
CFLAGS += -DF_PCLK1=$(F_PCLK1)
CFLAGS += -DF_SYSTICK=$(F_SYSTICK)

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F100C8Tx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU)
LDFLAGS += -specs=nosys.specs -specs=nano.specs
#LDFLAGS += -fno-exceptions -fno-rtti
#LDFLAGS += -u_printf_float
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += $(LIBDIR)
LDFLAGS += $(LIBS)
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(PROGNAME).map,--cref
LDFLAGS += -Wl,--gc-sections

# Для добавления возможности рабоать с рациональными числами удали символ '#' в начале строки
#LDFLAGS += -u _printf_float

# default action: build all
all: $(BUILD_DIR)/$(PROGNAME).elf $(BUILD_DIR)/$(PROGNAME).hex $(BUILD_DIR)/$(PROGNAME).bin

COLORIZE  = "\033[01;38;05;192m"
BACKCOLOR = "\033[0;0;0m"


#######################################
# build the application
#######################################
# list of objects
# default action: build all
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
#	@echo "Компиляция" $(COLORIZE) "$<" $(BACKCOLOR)
	@echo $(COLORIZE) "$<" $(BACKCOLOR)
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
#	@echo $(COLORIZE)"Ассемблирование "$(BACKCOLOR)
	@echo $(COLORIZE)"$<"$(BACKCOLOR)
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(PROGNAME).elf: $(OBJECTS) Makefile
	@echo $(COLORIZE)"Сборка "$(BACKCOLOR)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo $(COLORIZE)"Результат:"$(BACKCOLOR)
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo $(COLORIZE)"Извлекаем HEX-код"$(BACKCOLOR)
	@@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo $(COLORIZE)"Извлекаем бинарный код"$(BACKCOLOR)
	@@$(BIN) $< $@

$(BUILD_DIR):
	@mkdir $@		



#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
	

#######################################
# Clean
#######################################
clean:
	@-rm -fR $(BUILD_DIR)
#	-rm -f *.o *.elf *.bin *.lst *.d
	@echo $(COLORIZE)"Хоп! И чисто."$(BACKCOLOR)


#######################################
# Load
#######################################
load: $(BUILD_DIR)/$(PROGNAME).bin
	@echo $(COLORIZE)"Заливаем в камень..."$(BACKCOLOR)
#	stm32flash -w register.bin /dev/ttyUSB0
#	st-flash write /dev/stlinkv1_3 $(PROGNAME).bin 0x08000000
	st-flash write $(BUILD_DIR)/$(PROGNAME).bin 0x08000000
