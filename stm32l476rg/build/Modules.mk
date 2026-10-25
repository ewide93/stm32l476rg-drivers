# Include module-specific makefiles
DRIVERS_PATH := $(ROOT)/stm32l476rg/drivers
include $(DRIVERS_PATH)/Drivers.mk

APP_PATH := $(ROOT)/stm32l476rg/app
include $(APP_PATH)/App.mk

COMMON_PATH := $(ROOT)/stm32l476rg/common
include $(COMMON_PATH)/Common.mk

EXTERNAL_PATH := $(ROOT)/stm32l476rg/external
include $(EXTERNAL_PATH)/External.mk
