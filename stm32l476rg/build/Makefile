# -------------------------------------------------------------------------------------
# Paths to navigate project structure
# -------------------------------------------------------------------------------------
TARGET := STM32L476RG
ROOT := ..
OBJ_DIR := obj
OUT_DIR := out
LINKER_SCRIPT := $(TARGET).ld

# -------------------------------------------------------------------------------------
# Gather & arrange variables for build system
# -------------------------------------------------------------------------------------
SRC :=
INC :=

# Include modules
include Modules.mk

# Expand search for source files
VPATH := $(dir $(SRC))

# Object files
OBJ := $(patsubst %.c,%.o,$(notdir $(SRC)))

# Dependency files
DEP := $(patsubst %.o,$(OBJ_DIR)/%.d,$(OBJ))

# -------------------------------------------------------------------------------------
# Configuration of cross-compilation toolchain
# -------------------------------------------------------------------------------------

# Microcontroller specific options
MCUFLAGS := -mcpu=cortex-m4
MCUFLAGS += -mfpu=fpv4-sp-d16
MCUFLAGS += -mfloat-abi=hard
MCUFLAGS += -mthumb

# Cross compilation toolchain
PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
LD := $(PREFIX)gcc

# Compilation options
CFLAGS := $(MCUFLAGS)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fstack-usage
CFLAGS += -MMD -MP
CFLAGS += -c
CFLAGS += -std=c11
CFLAGS += $(addprefix -I,$(INC))
CFLAGS += -DSTM32L476xx
CFLAGS += -ggdb3

# Optimization level
OPT := -Os

# Compiler warnings
WFLAGS := -Wall
WFLAGS += -Wextra
WFLAGS += -Wpedantic
WFLAGS += -Werror
WFLAGS += -Wunused-variable
WFLAGS += -Wunused-parameter
WFLAGS += -Wstrict-prototypes
WFLAGS += -Wshadow
WFLAGS += -Wold-style-definition
WFLAGS += -Wredundant-decls
WFLAGS += -Wstack-usage=128
WFLAGS += -Wmissing-include-dirs
WFLAGS += -Wjump-misses-init
WFLAGS += -Wlogical-op
WFLAGS += -Wdouble-promotion

# Linker options
LDFLAGS := $(MCUFLAGS)
LDFLAGS += -T$(LINKER_SCRIPT)
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -nolibc
LDFLAGS += -lgcc
LDFLAGS += -Wl,-Map=$(OUT_DIR)/$(TARGET).map
LDFLAGS += -Wl,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += $(addprefix -I,$(INC))

# -------------------------------------------------------------------------------------
# Linter configuration
# -------------------------------------------------------------------------------------

# Linter options
LINTER := cppcheck
LINTER_CACHE_DIR = cppcheck_cache
LINT_ARM_CFG := "C:/Program Files/Cppcheck/platforms/arm32-wchar_t4.xml"
LINTFLAGS := --enable=all
LINTFLAGS += --force
LINTFLAGS += --quiet
LINTFLAGS += --std=c11
LINTFLAGS += --platform=$(LINT_ARM_CFG)
LINTFLAGS += --cppcheck-build-dir=$(LINTER_CACHE_DIR)
LINTFLAGS += --output-file=cppcheck_output.txt
LINT_INC := $(addprefix -I ,$(INC))
LINT_SUPPRESS := --suppressions-list=cppcheck_suppressions.txt

# Only check GCC
LINT_PREPROCESSOR_DEF := -D__GNUC__
LINT_PREPROCESSOR_UNDEF := -U__ICCARM__
LINT_PREPROCESSOR_UNDEF += -U__TASKING__
LINT_PREPROCESSOR_UNDEF += -U__TI_ARM__
LINT_PREPROCESSOR_UNDEF += -U__ti__
LINT_PREPROCESSOR_UNDEF += -U__CC_ARM__

# Only check STM32l476xx
LINT_PREPROCESSOR_DEF += -DSTM32L476xx
LINT_PREPROCESSOR_UNDEF += -USTM32L412xx
LINT_PREPROCESSOR_UNDEF += -USTM32L422xx
LINT_PREPROCESSOR_UNDEF += -USTM32L431xx
LINT_PREPROCESSOR_UNDEF += -USTM32L432xx
LINT_PREPROCESSOR_UNDEF += -USTM32L433xx
LINT_PREPROCESSOR_UNDEF += -USTM32L442xx
LINT_PREPROCESSOR_UNDEF += -USTM32L443xx
LINT_PREPROCESSOR_UNDEF += -USTM32L451xx
LINT_PREPROCESSOR_UNDEF += -USTM32L452xx
LINT_PREPROCESSOR_UNDEF += -USTM32L462xx
LINT_PREPROCESSOR_UNDEF += -USTM32L471xx
LINT_PREPROCESSOR_UNDEF += -USTM32L475xx
LINT_PREPROCESSOR_UNDEF += -USTM32L485xx
LINT_PREPROCESSOR_UNDEF += -USTM32L486xx
LINT_PREPROCESSOR_UNDEF += -USTM32L496xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4A6xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4P5xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4Q5xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4R5xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4R7xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4R9xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4S5xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4S7xx
LINT_PREPROCESSOR_UNDEF += -USTM32L4S9xx

# Ignore FreeRTOS files
LINT_IGNORE := -i$(EXTERNAL_PATH)/FreeRTOS

# -------------------------------------------------------------------------------------
# Default rule
# -------------------------------------------------------------------------------------
.PHONY: all
all: $(OUT_DIR)/$(TARGET).elf

# -------------------------------------------------------------------------------------
# Rule to flash the microcontroller using OpenOCD
# -------------------------------------------------------------------------------------
flash: $(OUT_DIR)/$(TARGET).elf
	@openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "program $^ verify reset exit"

# -------------------------------------------------------------------------------------
# Rule to produce .elf file
# -------------------------------------------------------------------------------------
$(OUT_DIR)/$(TARGET).elf: $(addprefix $(OBJ_DIR)/,$(OBJ))
	@echo "Linking $(notdir $^)"
	@$(LD) $(LDFLAGS) $^ -o $@

# -------------------------------------------------------------------------------------
# Rule to produce .o files
# -------------------------------------------------------------------------------------
$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) $(WFLAGS) $(OPT) $< -o $@

# -------------------------------------------------------------------------------------
# Rule to clean build directory
# -------------------------------------------------------------------------------------
.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OBJ_DIR)/*.d
	@rm -rf $(OBJ_DIR)/*.su
	@rm -rf $(OUT_DIR)/*.map
	@rm -rf $(OUT_DIR)/*.elf

# -------------------------------------------------------------------------------------
# Rule for running linter
# -------------------------------------------------------------------------------------
.PHONY: lint
lint:
	@$(LINTER) $(LINTFLAGS) $(LINT_PREPROCESSOR_DEF) $(LINT_PREPROCESSOR_UNDEF) $(LINT_IGNORE) $(LINT_INC) $(LINT_SUPPRESS) $(dir $(SRC))

# -------------------------------------------------------------------------------------
# Include dependency files
# -------------------------------------------------------------------------------------
-include $(DEP)