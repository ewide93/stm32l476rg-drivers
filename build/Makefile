# Paths to navigate project structure
TARGET := STM32L476RG
OBJ_DIR := obj
OUT_DIR := out
SRC_DIR := ../src
INC_DIR := ../inc
CMSIS_DIR := $(INC_DIR)/CMSIS
ST_DIR := $(INC_DIR)/ST
LINKER_SCRIPT := $(TARGET).ld

# Microcontroller specific options
MCUFLAGS := -mcpu=cortex-m4
MCUFLAGS += -mfpu=fpv4-sp-d16
MCUFLAGS += -mfloat-abi=hard
MCUFLAGS += -mthumb

# Cross compilation toolchain
PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
LD := $(PREFIX)gcc

# Source files
include Sources.mk

# Object files
OBJ := $(patsubst %.c,$(OBJ_DIR)/%.o,$(subst $(SRC_DIR)/,,$(SRC)))

# Dependency files
DEP := $(patsubst %.o,%.d,$(OBJ))

# Header files
INC := -I$(INC_DIR)
INC += -I$(CMSIS_DIR)
INC += -I$(ST_DIR)

# Compilation options
CFLAGS := $(MCUFLAGS)
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fstack-usage
CFLAGS += -MMD -MP
CFLAGS += -c
CFLAGS += -std=c11
CFLAGS += $(INC)
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

# Linker options
LDFLAGS := $(MCUFLAGS)
LDFLAGS += -T$(LINKER_SCRIPT)
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -lgcc
LDFLAGS += -Wl,-Map=$(OUT_DIR)/$(TARGET).map
LDFLAGS += -Wl,--cref
LDFLAGS += -Wl,--gc-sections
LDFLAGS += $(INC)

# Linter options.
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
LINT_INC := -I $(INC_DIR)
LINT_INC += -I $(ST_DIR)
LINT_INC += -I $(CMSIS_DIR)
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

# Default rule
.PHONY: all
all: $(OUT_DIR)/$(TARGET).elf

flash: $(OUT_DIR)/$(TARGET).elf
	openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "program $^ verify reset exit"

# Rule to produce .elf file
$(OUT_DIR)/$(TARGET).elf: $(OBJ)
	@echo "Linking $^"
	@$(LD) $(LDFLAGS) $^ -o $@

# Rule to produce .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(WFLAGS) $(OPT) $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OBJ_DIR)/*.d
	@rm -rf $(OUT_DIR)/*.map
	@rm -rf $(OUT_DIR)/*.elf

# Rule for running linter.
.PHONY: lint
lint:
	@$(LINTER) $(LINTFLAGS) $(LINT_PREPROCESSOR_DEF) $(LINT_PREPROCESSOR_UNDEF) $(LINT_INC) $(LINT_SUPPRESS) $(SRC_DIR)

-include $(DEP)