# -------------------------------------------------------------------------------------
# Paths to navigate project structure.
# -------------------------------------------------------------------------------------
UNITY_ROOT_DIR := ../../Unity
UNITY_SRC_DIR := $(UNITY_ROOT_DIR)/src
UNITY_SRC := $(UNITY_SRC_DIR)/unity.c
UNIT_TEST_BUILD_DIR := build
UNIT_TEST_RESULTS := $(UNIT_TEST_BUILD_DIR)/results.txt

APP_DIR := ../app
COMMON_DIR := ../common
DRIVERS_DIR := ../drivers

# -------------------------------------------------------------------------------------
# Define unit test runner executables.
# -------------------------------------------------------------------------------------
TESTRUNNERS := $(UNIT_TEST_BUILD_DIR)/test_fifo.exe
TESTRUNNERS += $(UNIT_TEST_BUILD_DIR)/test_mempool.exe
TESTRUNNERS += $(UNIT_TEST_BUILD_DIR)/test_cobs_codec.exe

# -------------------------------------------------------------------------------------
# Native toolchain configuration.
# -------------------------------------------------------------------------------------
CC := gcc

CFLAGS := -DUNIT_TEST
CFLAGS += -std=c11
CFLAGS += -I$(UNITY_SRC_DIR)
CFLAGS += -I$(COMMON_DIR)
CFLAGS += -I$(APP_DIR)
CFLAGS += -I$(DRIVERS_DIR)

WFLAGS := -Wall
WFLAGS += -Wextra
WFLAGS += -Wpedantic
WFLAGS += -Werror

# -------------------------------------------------------------------------------------
# Rule to build all test runners.
# -------------------------------------------------------------------------------------
.PHONY: all
all: $(TESTRUNNERS)
	@echo "Successfully compiled test runners..."
	@$(foreach RUNNER,$^,$(shell $(RUNNER) >> $(UNIT_TEST_RESULTS) 2>&1 || true))

# -------------------------------------------------------------------------------------
# Rule to build test runner for fifo module unit tests.
# -------------------------------------------------------------------------------------
$(UNIT_TEST_BUILD_DIR)/test_fifo.exe: test_fifo.c $(COMMON_DIR)/fifo.c $(UNITY_SRC)
	@echo "Compiling unit test runner $(notdir $@)..."
	@$(CC) $(CFLAGS) $(WFLAGS) $^ -o $@

# -------------------------------------------------------------------------------------
# Rule to build test runner for mempool module unit tests.
# -------------------------------------------------------------------------------------
$(UNIT_TEST_BUILD_DIR)/test_mempool.exe: test_mempool.c $(COMMON_DIR)/mempool.c $(UNITY_SRC)
	@echo "Compiling unit test runner $(notdir $@)..."
	@$(CC) $(CFLAGS) -Wno-unused-variable $(WFLAGS) $^ -o $@

# -------------------------------------------------------------------------------------
# Rule to build test runner for COBS codec module unit tests.
# -------------------------------------------------------------------------------------
$(UNIT_TEST_BUILD_DIR)/test_cobs_codec.exe: test_cobs_codec.c $(APP_DIR)/cobs_codec.c $(UNITY_SRC)
	@echo "Compiling unit test runner $(notdir $@)..."
	@$(CC) $(CFLAGS) -Wno-unused-variable $(WFLAGS) $^ -o $@


# -------------------------------------------------------------------------------------
# Rule to clean build directory.
# -------------------------------------------------------------------------------------
.PHONY: clean
clean:
	@echo "Cleaning up unit test build directory..."
	@rm -rf $(UNIT_TEST_BUILD_DIR)/*.exe
	@rm -rf $(UNIT_TEST_BUILD_DIR)/*.txt
