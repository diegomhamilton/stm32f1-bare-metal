TARGET = main

BUILD_DIR = ./build

.SECONDEXPANSION:
.PRECIOUS: $(BUILD_DIR)/ $(BUILD_DIR)%/

# Define the linker script location and chip architecture.
#LD_SCRIPT = stm32.ld
LD_SCRIPT = board/stm32f103xb.ld
#STARTUP_CODE = board/startup_stm32f103xb.s
MCU_SPEC  = cortex-m3

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr/local
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
#ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
#CFLAGS += -g
CFLAGS += -O3
CFLAGS += --specs=nosys.specs
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nano.specs

# Linker directives.
LFLAGS += -nostdlib
#LFLAGS += -lgcc
LFLAGS += -nostartfiles
LFLAGS += --cref -Map $(BUILD_DIR)/$(TARGET).map
LFLAGS += -T./$(LD_SCRIPT)

STARTUP_SRC	= startup_stm32f103xb.s
C_SRC    	= user_init.c
APP_SRC    	= main.c

include board/Makefile
include os/Makefile
include hal/Makefile

OBJS = $(BUILD_DIR)/$(STARTUP_SRC:.s=.o)
OBJS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SRC:.c=.o)))
OBJS += $(BUILD_DIR)/$(APP_SRC:.c=.o)

.PHONY: all, list
all: $(BUILD_DIR)/$(TARGET).bin

list: $(C_SRC) $(APP_SRC) board/$(STARTUP_SRC)
	$(CC) -g -Wa,-adhln $(CFLAGS) $(INCLUDE_DIRS) $^ > $(BUILD_DIR)/$(TARGET).lst

$(BUILD_DIR)/%.o: board/%.s | $$(@D)/
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: board/%.S | $$(@D)/
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c | $$(@D)/
	$(CC) -c $(CFLAGS) $(INCLUDE_DIRS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(LD) $^ $(LFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

$(BUILD_DIR)/:
	mkdir -p $@

$(BUILD_DIR)%/:
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
