# Source files
SRC += $(DRIVERS_PATH)/startup.c
SRC += $(DRIVERS_PATH)/systick.c
SRC += $(DRIVERS_PATH)/pin.c
SRC += $(DRIVERS_PATH)/clock_control.c
SRC += $(DRIVERS_PATH)/flash.c
SRC += $(DRIVERS_PATH)/digital.c
SRC += $(DRIVERS_PATH)/uart.c
SRC += $(DRIVERS_PATH)/crc.c
SRC += $(DRIVERS_PATH)/exti.c

# Include paths
INC += $(DRIVERS_PATH)