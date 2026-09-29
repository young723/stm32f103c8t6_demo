###############################################################################
# Makefile for STM32F103C8T6  (ARM GCC + OpenOCD)
#
#   make            编译并链接生成 .elf / .hex / .bin
#   make flash      通过 OpenOCD 烧录 .elf 到芯片
#   make debug      启动 OpenOCD GDB server (等待 GDB 连接)
#   make erase      擦除整片 Flash
#   make clean      清理构建目录
#
# 从 Keil MDK-ARM 工程自动转换，使用 GCC 版启动文件。
###############################################################################

######################################
# target
######################################
TARGET = stm32f103c8t6

######################################
# building variables
######################################
BUILD_DIR = build

######################################
# toolchain
######################################
CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size
GDB     = arm-none-eabi-gdb

######################################
# MCU flags (Cortex-M3, STM32F103C8)
######################################
MCU = -mcpu=cortex-m3 -mthumb

######################################
# C defines  (from .uvprojx)
######################################
DEFS = -DUSE_HAL_DRIVER -DSTM32F103xB
DEFS += -DUSE_MAKEFILE
DEFS += -DGAME_DEMO
# DEFS += -DUSE_DUAL_OLED

######################################
# include paths  (from .uvprojx)
######################################
INCS = \
    -ICore/Inc \
    -IDrivers/STM32F1xx_HAL_Driver/Inc \
    -IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
    -IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
    -IDrivers/CMSIS/Include \
    -IMiscDrivers/oled \
    -IApp/oled

######################################
# C flags
######################################
CFLAGS = $(MCU) $(DEFS) $(INCS)
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wextra
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -Og -g3
CFLAGS += -Wunused -Wno-unused-parameter

######################################
# Assembly flags
######################################
ASFLAGS = $(MCU) $(DEFS) $(INCS)
ASFLAGS += -x assembler-with-cpp

######################################
# Linker flags
######################################
LDSCRIPT = STM32F103C8Tx_FLASH.ld
LDFLAGS = $(MCU)
LDFLAGS += -T$(LDSCRIPT)
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref
LDFLAGS += -Wl,--print-memory-usage
LDFLAGS += -Wl,--no-warn-rwx-segments
LDFLAGS += --specs=nano.specs --specs=nosys.specs

######################################
# C source files  (from .uvprojx)
######################################
C_SOURCES = \
    Core/Src/main.c \
    Core/Src/stm32f1xx_it.c \
    Core/Src/stm32f1xx_hal_msp.c \
    Core/Src/bsp_hardware.c \
    Core/Src/bsp_i2c.c \
    Core/Src/bsp_spi.c \
    Core/Src/bsp_sw_i2c.c \
    Core/Src/bsp_sw_spi.c \
    Core/Src/qst_log.c \
    Core/Src/syscalls.c \
    Core/Src/sysmem.c \
    Core/Src/system_stm32f1xx.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
    Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
    MiscDrivers/oled/oled.c \
    App/oled/game_dino.c \
    App/oled/gif_cat.c

######################################
# Assembly source files
#   使用 Drivers 中自带的 GCC 版启动文件 (原 MDK-ARM/ 下的 .s 仅适用于 ARMCC)
######################################
ASM_SOURCES = \
    Drivers/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xb.s

######################################
# OpenOCD config
######################################
OCD_CFG  = openocd.cfg

###############################################################################
# Build rules
###############################################################################

# Object & dependency files
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# Default target
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).lst size

# Create build directory
$(BUILD_DIR):
	@mkdir -p $@

# Compile C sources
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "  CC    $<"
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

# Assemble S sources
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "  AS    $<"
	$(AS) -c $(ASFLAGS) $< -o $@

# Link
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(LDSCRIPT) | $(BUILD_DIR)
	@echo "  LD    $@"
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	$(OBJDUMP) -h -S $@ > $(BUILD_DIR)/$(TARGET).lst

# Hex
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo "  HEX   $@"
	$(OBJCOPY) -O ihex $< $@

# Bin
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "  BIN   $@"
	$(OBJCOPY) -O binary -S $< $@

# Disassembly listing (standalone, already generated by link rule too)
$(BUILD_DIR)/$(TARGET).lst: $(BUILD_DIR)/$(TARGET).elf
	@echo "  LST   $@"
	$(OBJDUMP) -h -S $< > $@

# Size report
size: $(BUILD_DIR)/$(TARGET).elf
	@echo ""
	@echo "===== Memory Usage ====="
	$(SIZE) $<
	@echo ""

###############################################################################
# OpenOCD targets
###############################################################################

# Flash the .elf
flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f $(OCD_CFG) \
	    -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

# Flash the .hex (alternative)
flash-hex: $(BUILD_DIR)/$(TARGET).hex
	openocd -f $(OCD_CFG) \
	    -c "program $(BUILD_DIR)/$(TARGET).hex verify reset exit"

# Start OpenOCD GDB server for debugging
debug: $(BUILD_DIR)/$(TARGET).elf
	openocd -f $(OCD_CFG) \
	    -c "init" -c "halt"

# GDB client (connect to running OpenOCD server)
gdb: $(BUILD_DIR)/$(TARGET).elf
	$(GDB) -ex "target extended-remote localhost:3333" \
	       -ex "monitor reset halt" \
	       -ex "load" \
	       -ex "monitor reset halt" \
	       $(BUILD_DIR)/$(TARGET).elf

# Erase entire chip
erase:
	openocd -f $(OCD_CFG) -c "init" -c "halt" -c "flash erase_sector 0 0 last" -c "reset" -c "exit"

###############################################################################
# Clean
###############################################################################
clean:
	@echo "  CLEAN $(BUILD_DIR)"
	@rm -rf $(BUILD_DIR)

###############################################################################
# Phony targets
###############################################################################
.PHONY: all clean flash flash-hex debug gdb erase size
