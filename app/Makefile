PROJECT = blink-led
BUILD_DIR = bin

#SHARED_DIR = ../my-common-code
CFILES = src/main.c
CFILES += src/uart.c
CFILES += src/buffer.c
#AFILES += api-asm.S

# TODO - you will need to edit these two lines!
DEVICE=stm32f103C6T6A
OOCD_FILE = board/stm32f103c8_blue_pill.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

clean:
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/src/*.o
	rm -f $(BUILD_DIR)/src/*.d