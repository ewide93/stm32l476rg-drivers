# Include module-specific makefiles
DRIVERS_PATH := $(ROOT)/drivers
include $(DRIVERS_PATH)/Drivers.mk

APP_PATH := $(ROOT)/app
include $(APP_PATH)/App.mk

COMMON_PATH := $(ROOT)/common
include $(COMMON_PATH)/Common.mk

EXTERNAL_PATH := $(ROOT)/external
include $(EXTERNAL_PATH)/External.mk

OSAL_PATH := $(ROOT)/osal
include $(OSAL_PATH)/Osal.mk
