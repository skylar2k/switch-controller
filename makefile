MCU          = atmega32u4
ARCH         = AVR8
BOARD		 = USBKEY
F_CPU        = 16000000
F_USB        = $(F_CPU)
AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT = usb:2341:0037
OPTIMIZATION = s
TARGET       = src/gamepad
SRCDIR := src
SRC = $(wildcard $(SRCDIR)/*.c) $(LUFA_SRC_USB)
LUFA_PATH    = ./lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk

test_script: scripts/test.script
	@echo xxd $@
	@echo "unsigned char script[] = { " > src/script.h
	@cat $^ | xxd -i - >> src/script.h
	@echo "};" >> src/script.h
	@echo "unsigned int script_len = `wc -c $^ | awk 'END{print $$1}'`;" >> src/script.h